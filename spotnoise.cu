#include "cuda.h"
#include "cuda_runtime_api.h"

#define NX 256
#define NY 256

#define tamanio (NX*NY)


// Cuda code

// Complex multiplication
static __device__ __host__ inline cufftComplex ComplexMul(cufftComplex a, cufftComplex b)
{
    cufftComplex c;

    c.x = (a.x * b.x - a.y * b.y) * (1.0f / (float)(tamanio));
    c.y = (a.x * b.y + a.y * b.x) * (1.0f / (float)(tamanio));
    return c;
}

// Complex pointwise multiplication
static __global__ void ComplexPointwiseMulAndScale(cufftComplex* a, cufftComplex* b, int size)
{
    const int numThreads = blockDim.x * gridDim.x;
    const int threadID = blockIdx.x * blockDim.x + threadIdx.x;

    for (int i = threadID; i < size; i += numThreads)
        a[i] = ComplexMul(a[i], b[i]);  
} 

void spotNoise(int* spotParam, GLubyte* result)
{


	cufftHandle plan, planNoise;

	cufftReal * texturaCuda = (cufftReal*)malloc(sizeof(cufftReal)*tamanio);
	cufftReal * noiseCuda =   (cufftReal*)malloc(sizeof(cufftReal)*tamanio);

	int i;
 	for(i = 0; i<tamanio; i++)
	{
			texturaCuda[i] = (cufftReal)spotParam[i];
			
			int random = (rand() % 256 );
					
			noiseCuda[i] = (cufftReal)random;
			
	}

	cufftReal *idata, *idataNoise;
	cufftComplex *odata, *odataNoise;


	cudaMalloc((void**)&idata, sizeof(cufftReal)*tamanio);
	cudaMalloc((void**)&idataNoise, sizeof(cufftReal)*tamanio);

	cudaMalloc((void**)&odata, sizeof(cufftComplex)*tamanio);
	cudaMalloc((void**)&odataNoise, sizeof(cufftComplex)*tamanio);

	cufftResult res = cufftPlan2d(&plan, NX, NY, CUFFT_R2C);

	// copio al device la imagen
	cudaMemcpy(idata, texturaCuda, sizeof(cufftReal)*tamanio,
                              cudaMemcpyHostToDevice);

	cufftExecR2C( plan, idata, odata );

	cufftPlan2d(&planNoise, NX, NY, CUFFT_R2C);

	// copio al device el ruido
	cudaMemcpy(idataNoise, noiseCuda, sizeof(cufftReal)*tamanio,
                              cudaMemcpyHostToDevice);

	cufftExecR2C( planNoise, idataNoise, odataNoise );


	// multiplico en el dominio de las frecuencias (En la GPU)
	// un grid, un block, 256 threads
	ComplexPointwiseMulAndScale<<<32, 256>>>(odata, odataNoise, tamanio);
	// en odata, la transformada que queremos

	cufftHandle planInv;
	cufftPlan2d(&planInv, NX, NY, CUFFT_C2R);

	cufftExecC2R( planInv, odata, idata );

	cudaMemcpy(texturaCuda, idata, sizeof(cufftReal)*tamanio,
                              cudaMemcpyDeviceToHost);


	
 	for(i = 0; i<tamanio; i++)
	{	
		result[i] = (int)(texturaCuda[i]/tamanio);			
	}


	/* Destroy the CUFFT plan. */
	cufftDestroy(plan);
	cufftDestroy(planInv);
	cudaFree(idata); cudaFree(odata);
	cudaFree(idataNoise); cudaFree(odataNoise);

}