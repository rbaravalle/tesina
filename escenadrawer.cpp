#include <stdio.h>
#include <stdlib.h>
#include <math.h>     /* for sqrt, sin, cos, and fabs */
#include <assert.h>   /* for assert */

#include <escenadrawer.h>
//#include <spotnoiseE.h>
//#include <spotnoise2E.h>
//#include <spotnoise3E.h>

#include <QtOpenGL/QGLWidget>
#include <GL/glu.h>

#include<iostream>
using namespace std;


#include <QTimer>

//***************//
// CODIGO OPENGL //  
//***************//

#define NX 256
#define NY 256
#define MAX_FPS -1

#define tamanio (NX*NY)

static CGcontext   myCgContext;

static CGprogram   myCgFragmentProgram,
                   myCgVertexProgram;

static CGprofile   myCgFragmentProfile,
                   myCgVertexProfile;

static CGparameter cg_modelViewProj;

static const char *myProgramName = "escena shader",
                *vertex_program_name = "/home/rodrigo/frameworkF/vertexE.cg",
                *fragment_program_name = "/home/rodrigo/frameworkF/fragment.cg";

enum {
  TO_TEXTURA_0 = 0,
  TO_TEXTURA_1 = 1,
  TO_TEXTURA_2 = 2
};


static GLubyte texturas[CANT_T_PARAMETERS][tamanio];
GLuint texObjE[CANT_T_PARAMETERS];

CGparameter texturesParametersE[CANT_T_PARAMETERS];
CGparameter combinaParametersE[CANT_C_PARAMETERS];
CGparameter fragmentParametersE[CANT_F_PARAMETERS];
CGparameter colorParametersE;
CGparameter antialiasParameterE;


const char* tNamesParametersE[] = { 
	          "textura0",
   	          "textura1",
	          "textura2",
              "filtermap" };

const char* cNamesParametersE[] = {
              "combina1",
              "combina2",
};

// Nombres de los parametros en el fragment shader
const char* fNamesParametersE[] = { 
	          "usa",
	          "amplitud",
	          "turb",
	          "escala_u",
	          "intensidad",
              "base",
	          "angulos",
              "desplazamiento_u",
              "desplazamiento_v",
              "tipo_turb",
              "escala_v",
              "alpha",
              "beta",
              "gama"
};

// parametros dinamicos de las texturas, valores por default
float fArrayParametersE[][CANT_T_PARAMETERS-1] = {
    {1,1,1}, // usa
    {1,1,1}, // amplitud
    {0,0,0}, // turb
    {20,20,20}, // escala_u
    {1,1,1}, // intensidad
    {0,0,0}, // base
    {1,0,0}, // angulos
    {0,0,0}, // desplazamiento_u
    {0,0,0}, // desplazamiento_v
    {0,0,0}, // tipo turbulencia
    {1,1,1}, // escala_v
    {1,1,1}, // alpha
    {1,1,1}, // beta
    {1,1,1}, // gama
};

int cArrayParametersE[CANT_C_PARAMETERS] = {
    0,0, // funciones de combinacion    
};

float colorArrayParametersE[] = {
    0.5,0.5,0.5 // color (rgb)
};

int antialiasE = 1;

int cuadros = 0;
int fps = 0;

/************************************/
// Codigo para pasar la modelviewproj al programa
// El codigo esta copiado del ejemplo ..vertex.. de cg

static void buildPerspectiveMatrix(double, double,
                                   double, double, float m[16]);

static const double myPi = 3.14159265358979323846;

float eye_x = 0.0,eye_y = 0.0,eye_z = 7.0;
static float myProjectionMatrix[16];

static void buildPerspectiveMatrix(double fieldOfView,
                                   double aspectRatio,
                                   double zNear, double zFar,
                                   float m[16])
{
  double sine, cotangent, deltaZ;
  double radians = fieldOfView / 2.0 * myPi / 180.0;
  
  deltaZ = zFar - zNear;
  sine = sin(radians);
  /* Should be non-zero to avoid division by zero. */
  assert(deltaZ);
  assert(sine);
  assert(aspectRatio);
  cotangent = cos(radians) / sine;

  m[0*4+0] = cotangent / aspectRatio;
  m[0*4+1] = 0.0;
  m[0*4+2] = 0.0;
  m[0*4+3] = 0.0;
  
  m[1*4+0] = 0.0;
  m[1*4+1] = cotangent;
  m[1*4+2] = 0.0;
  m[1*4+3] = 0.0;
  
  m[2*4+0] = 0.0;
  m[2*4+1] = 0.0;
  m[2*4+2] = -(zFar + zNear) / deltaZ;
  m[2*4+3] = -2 * zNear * zFar / deltaZ;
  
  m[3*4+0] = 0.0;
  m[3*4+1] = 0.0;
  m[3*4+2] = -1;
  m[3*4+3] = 0;
}

/* Build a row-major (C-style) 4x4 matrix transform based on the
   parameters for gluLookAt. */
static void buildLookAtMatrix(double eyex, double eyey, double eyez,
                              double centerx, double centery, double centerz,
                              double upx, double upy, double upz,
                              float m[16])
{
  double x[3], y[3], z[3], mag;

  /* Difference eye and center vectors to make Z vector. */
  z[0] = eyex - centerx;
  z[1] = eyey - centery;
  z[2] = eyez - centerz;
  /* Normalize Z. */
  mag = sqrt(z[0]*z[0] + z[1]*z[1] + z[2]*z[2]);
  if (mag) {
    z[0] /= mag;
    z[1] /= mag;
    z[2] /= mag;
  }

  /* Up vector makes Y vector. */
  y[0] = upx;
  y[1] = upy;
  y[2] = upz;

  /* X vector = Y cross Z. */
  x[0] =  y[1]*z[2] - y[2]*z[1];
  x[1] = -y[0]*z[2] + y[2]*z[0];
  x[2] =  y[0]*z[1] - y[1]*z[0];

  /* Recompute Y = Z cross X. */
  y[0] =  z[1]*x[2] - z[2]*x[1];
  y[1] = -z[0]*x[2] + z[2]*x[0];
  y[2] =  z[0]*x[1] - z[1]*x[0];

  /* Normalize X. */
  mag = sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);
  if (mag) {
    x[0] /= mag;
    x[1] /= mag;
    x[2] /= mag;
  }

  /* Normalize Y. */
  mag = sqrt(y[0]*y[0] + y[1]*y[1] + y[2]*y[2]);
  if (mag) {
    y[0] /= mag;
    y[1] /= mag;
    y[2] /= mag;
  }

  /* Build resulting view matrix. */
  m[0*4+0] = x[0];  m[0*4+1] = x[1];
  m[0*4+2] = x[2];  m[0*4+3] = -x[0]*eyex + -x[1]*eyey + -x[2]*eyez;

  m[1*4+0] = y[0];  m[1*4+1] = y[1];
  m[1*4+2] = y[2];  m[1*4+3] = -y[0]*eyex + -y[1]*eyey + -y[2]*eyez;

  m[2*4+0] = z[0];  m[2*4+1] = z[1];
  m[2*4+2] = z[2];  m[2*4+3] = -z[0]*eyex + -z[1]*eyey + -z[2]*eyez;

  m[3*4+0] = 0.0;   m[3*4+1] = 0.0;  m[3*4+2] = 0.0;  m[3*4+3] = 1.0;
}

static void makeRotateMatrix(float angle,
                             float ax, float ay, float az,
                             float m[16])
{
  float radians, sine, cosine, ab, bc, ca, tx, ty, tz;
  float axis[3];
  float mag;

  axis[0] = ax;
  axis[1] = ay;
  axis[2] = az;
  mag = sqrt(axis[0]*axis[0] + axis[1]*axis[1] + axis[2]*axis[2]);
  if (mag) {
    axis[0] /= mag;
    axis[1] /= mag;
    axis[2] /= mag;
  }

  radians = angle * myPi / 180.0;
  sine = sin(radians);
  cosine = cos(radians);
  ab = axis[0] * axis[1] * (1 - cosine);
  bc = axis[1] * axis[2] * (1 - cosine);
  ca = axis[2] * axis[0] * (1 - cosine);
  tx = axis[0] * axis[0];
  ty = axis[1] * axis[1];
  tz = axis[2] * axis[2];

  m[0]  = tx + cosine * (1 - tx);
  m[1]  = ab + axis[2] * sine;
  m[2]  = ca - axis[1] * sine;
  m[3]  = 0.0f;
  m[4]  = ab - axis[2] * sine;
  m[5]  = ty + cosine * (1 - ty);
  m[6]  = bc + axis[0] * sine;
  m[7]  = 0.0f;
  m[8]  = ca + axis[1] * sine;
  m[9]  = bc - axis[0] * sine;
  m[10] = tz + cosine * (1 - tz);
  m[11] = 0;
  m[12] = 0;
  m[13] = 0;
  m[14] = 0;
  m[15] = 1;
}

static void makeTranslateMatrix(float x, float y, float z, float m[16])
{
  m[0]  = 1;  m[1]  = 0;  m[2]  = 0;  m[3]  = x;
  m[4]  = 0;  m[5]  = 1;  m[6]  = 0;  m[7]  = y;
  m[8]  = 0;  m[9]  = 0;  m[10] = 1;  m[11] = z;
  m[12] = 0;  m[13] = 0;  m[14] = 0;  m[15] = 1;
}

/* Simple 4x4 matrix by 4x4 matrix multiply. */
static void multMatrix(float dst[16],
                       const float src1[16], const float src2[16])
{
  float tmp[16];
  int i, j;

  for (i=0; i<4; i++) {
    for (j=0; j<4; j++) {
      tmp[i*4+j] = src1[i*4+0] * src2[0*4+j] +
                   src1[i*4+1] * src2[1*4+j] +
                   src1[i*4+2] * src2[2*4+j] +
                   src1[i*4+3] * src2[3*4+j];
    }
  }
  /* Copy result to dst (so dst can also be src1 or src2). */
  for (i=0; i<16; i++)
    dst[i] = tmp[i];
}




/***********************************/



//*******************//
// FIN CODIGO OPENGL //
//*******************//


// RUTINAS DE LA CLASE

// contador de fps
void Escenadrawer::update()
{
    fps = cuadros;
    cuadros = 0;
    emit cambiarFPS(fps);
    emit cambiarEye(eye_x,eye_y,eye_z);
    timer->start();
}

void Escenadrawer::_checkForCgError(const char *situation)
{
  CGerror error;
  const char *string = cgGetLastErrorString(&error);

  if (error != CG_NO_ERROR) {
    printf("%s: %s: %s\n",
      myProgramName, situation, string);
    if (error == CG_COMPILER_ERROR) {
      printf("%s\n", cgGetLastListing(myCgContext));
    }
    exit(1);
  }
}

// tamanio de la pantalla
QSize Escenadrawer::minimumSizeHint() const
{
     return QSize(700, 700);
}

QSize Escenadrawer::sizeHint() const
{
     return QSize(700, 700);
}

void Escenadrawer::initializeGL()
{   

    timer = new QTimer(this);
    timer->start();
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));

    glClearColor(0, 0, 0, 0.0);

    glEnable(GL_DEPTH_TEST);

    /*for(int i = 0; i < tamanio; i++)
    {
        texturas[0][i] = (GLubyte)text0E[i];
        texturas[1][i] = (GLubyte)text1E[i];
        texturas[2][i] = (GLubyte)text2E[i];
    }*/

    glutInitWindowSize(700, 700);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures( CANT_T_PARAMETERS, texObjE );

    ///////////
    for(int i = 0; i < CANT_T_PARAMETERS-1; i++) {
        glBindTexture(GL_TEXTURE_2D, texObjE[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_LUMINANCE,
            NX, NY, GL_LUMINANCE, GL_UNSIGNED_BYTE, texturas[i]);
    }

    /*ANTIALIASING*/
    glBindTexture(GL_TEXTURE_2D, texObjE[CANT_T_PARAMETERS-1]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    #define LEVELS 9
    for (int level = 0; level < LEVELS; ++level) {
      int res = (1 << (LEVELS-1-level));
      int log2Width = 16;
      //cout << "en level: " << level << ", guardamos: " << 16*level << endl;
      for (int i = 0; i < tamanio ; ++i)
        texturas[CANT_T_PARAMETERS-1][i] = 16*level;
        glTexImage2D(GL_TEXTURE_2D, level, 1, res, res, 0,
                     GL_RED, GL_UNSIGNED_BYTE, texturas[CANT_T_PARAMETERS-1]);
    }

    /*ANTIALIASING*/

    ///////////


    myCgContext = cgCreateContext();
    _checkForCgError("creating context");
    cgGLSetDebugMode(CG_FALSE);
    cgSetParameterSettingMode(myCgContext, CG_DEFERRED_PARAMETER_SETTING);

    myCgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
    cgGLSetOptimalOptions(myCgVertexProfile);
    _checkForCgError("selecting vertex profile");

    myCgVertexProgram =
        cgCreateProgramFromFile(
          myCgContext,              
          CG_SOURCE,                
          vertex_program_name,
          myCgVertexProfile,        
          "main",
          NULL);
    _checkForCgError("creating vertex program from string");
    cgGLLoadProgram(myCgVertexProgram);
    _checkForCgError("loading vertex program");

    cgGLEnableProfile(CG_PROFILE_ARBVP1);
    myCgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
    cgGLSetOptimalOptions(myCgFragmentProfile);
    _checkForCgError("selecting fragment profile");

    myCgFragmentProgram =
        cgCreateProgramFromFile(
          myCgContext,             
          CG_SOURCE,               
          fragment_program_name,
          myCgFragmentProfile,
          "main",
           NULL); 

    _checkForCgError("creating fragment program from string");
    cgGLLoadProgram(myCgFragmentProgram);
    _checkForCgError("loading fragment program");

    // binding con parametros del fragment shader
    for (int i = 0; i < CANT_T_PARAMETERS; i++) {
	    texturesParametersE[i] = cgGetNamedParameter(myCgFragmentProgram, (char*)tNamesParametersE[i]);
        _checkForCgError("could not get texture parameter");

        cgGLSetTextureParameter(texturesParametersE[i], texObjE[i]);
        _checkForCgError("setting textura texture");
    }

    // binding con parametros del fragment shader
    for (int i = 0; i < CANT_F_PARAMETERS; i++) {
	    fragmentParametersE[i] = cgGetNamedParameter(myCgFragmentProgram, (char*)fNamesParametersE[i]);
        _checkForCgError("could not get fragment parameter");
    }

    // binding con parametros del fragment shader (combinacion de texturas)
    for (int i = 0; i < CANT_C_PARAMETERS; i++) {
	    combinaParametersE[i] = cgGetNamedParameter(myCgFragmentProgram, (char*)cNamesParametersE[i]);
        _checkForCgError("could not get combina parameter");
    }


    //textura->renderInit();

    colorParametersE = cgGetNamedParameter(myCgFragmentProgram, "color");
    _checkForCgError("could not get color parameter");

    antialiasParameterE = cgGetNamedParameter(myCgFragmentProgram, "antialias");
    _checkForCgError("could not get antialias parameter");

    cg_modelViewProj = cgGetNamedParameter(myCgVertexProgram, "modelViewProj");
    _checkForCgError("could not get modelViewProj parameter");

    /////////


}

void Escenadrawer::paintGL()
{
    if(cuadros > MAX_FPS) { timer->stop(); }
    else {
        cuadros++;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float translateMatrix[16], rotateMatrix[16],
        modelMatrix[16], viewMatrix[16],
        modelViewMatrix[16], modelViewProjMatrix[16];

    buildLookAtMatrix(eye_x, eye_y, eye_z, 
                    0, 0, 0, 
                    0, 1, 0, 
                    viewMatrix);

    for (int i = 0; i< CANT_T_PARAMETERS; i++) {
        cgGLEnableTextureParameter(texturesParametersE[i]);
        _checkForCgError("enabling textures");
    }

    cgGLEnableProfile(myCgVertexProfile);
    _checkForCgError("enabling vertex profile");

    cgGLBindProgram(myCgFragmentProgram);
    _checkForCgError("binding fragment program");

    cgGLEnableProfile(myCgFragmentProfile);
    _checkForCgError("enabling fragment profile");

    glClearColor(0.0, 0.0, 0.0, 0);

    makeRotateMatrix(0, 1, 1, 1, rotateMatrix);
    makeTranslateMatrix(0, 0, 0, translateMatrix);
    multMatrix(modelMatrix, translateMatrix, rotateMatrix);

    // modelViewMatrix = viewMatrix * modelMatrix;
    multMatrix(modelViewMatrix, viewMatrix, modelMatrix);

    // modelViewProj = projectionMatrix * modelViewMatrix
    multMatrix(modelViewProjMatrix, myProjectionMatrix, modelViewMatrix);

    // Set matrix parameter with row-major matrix.
    cgSetMatrixParameterfr(cg_modelViewProj, modelViewProjMatrix);

    //textura->render();

    QList<Textura*>::iterator i = texs->end();
    while (i != texs->begin()) {
         --i;
         setear_valores_textura(*i);
    }
 
    glFlush();

    cgGLDisableProfile(myCgVertexProfile);
    _checkForCgError("disabling vertex profile");

    cgGLDisableProfile(myCgFragmentProfile);
    _checkForCgError("disabling fragment profile");


}

void Escenadrawer::setear_valores_textura(Textura* textura) {


    fArrayParametersE[0][0] = textura->get_usa();
    fArrayParametersE[1][0] = textura->get_amplitud();
    fArrayParametersE[2][0] = textura->get_turbulencia();
    fArrayParametersE[3][0] = textura->get_frecuencia();
    fArrayParametersE[4][0] = textura->get_intensidad();
    fArrayParametersE[5][0] = textura->get_base();
    fArrayParametersE[6][0] = textura->get_angulo();
    fArrayParametersE[7][0] = textura->get_desplazamiento_x();
    fArrayParametersE[8][0] = textura->get_desplazamiento_y();
    fArrayParametersE[9][0] = textura->get_tipo_turb();
    fArrayParametersE[10][0] = textura->get_frecuencia_v();
    fArrayParametersE[11][0] = textura->get_alpha();
    fArrayParametersE[12][0] = textura->get_beta();
    fArrayParametersE[13][0] = textura->get_gama();
   
    colorArrayParametersE[0] = textura->get_color_r();
    colorArrayParametersE[1] = textura->get_color_g();
    colorArrayParametersE[2] = textura->get_color_b();

    antialiasE = textura->get_antialias();
    
    Textura *t = textura->get_siguiente();

    if(t != NULL) { cArrayParametersE[0] = textura->get_combina(); }

    int i = 1;
    while(t != NULL) {
        fArrayParametersE[0][i] = t->get_usa();
        fArrayParametersE[1][i] = t->get_amplitud();
        fArrayParametersE[2][i] = t->get_turbulencia();
        fArrayParametersE[3][i] = t->get_frecuencia();
        fArrayParametersE[4][i] = t->get_intensidad();
        fArrayParametersE[5][i] = t->get_base();
        fArrayParametersE[6][i] = t->get_angulo();
        fArrayParametersE[7][i] = t->get_desplazamiento_x();
        fArrayParametersE[8][i] = t->get_desplazamiento_y();
        fArrayParametersE[9][i] = t->get_tipo_turb();
        fArrayParametersE[10][i] = t->get_frecuencia_v();
        fArrayParametersE[11][i] = t->get_alpha();
        fArrayParametersE[12][i] = t->get_beta();
        fArrayParametersE[13][i] = t->get_gama();

        Textura* t2 = t;
        t = t->get_siguiente();

        if(i <= CANT_T_PARAMETERS-3) {
            if( (cArrayParametersE[i] = t2->get_combina() ) == NO_COMB && t != NULL) {
                    cout << "error en combinacion" << endl; 
                    return;
            }
        }

        i++;

        if(t && i >= CANT_T_PARAMETERS ) {
            cout << "Maximo " << CANT_T_PARAMETERS << " texturas:" << i << endl;
            break;
        }

    }


    for(int i = 0; i < CANT_F_PARAMETERS; i++) {
        cgGLSetParameterArray1f(fragmentParametersE[i],
                                         0,
                                         CANT_T_PARAMETERS-1,
                                         fArrayParametersE[i]);
    }

    for(int i = 0; i < CANT_C_PARAMETERS; i++) {
        cgGLSetParameter1f(combinaParametersE[i],                                       
                                         cArrayParametersE[i]);
    }


    cgGLSetParameterArray1f(colorParametersE, 0,3,
                        colorArrayParametersE);

    cgGLSetParameter1f(antialiasParameterE, antialiasE);



    cgUpdateProgramParameters(myCgVertexProgram);
    cgUpdateProgramParameters(myCgFragmentProgram);

    // texture mapping
    for (int i = 0; i  < CANT_T_PARAMETERS; i++) {
        glBindTexture(GL_TEXTURE_2D, texObjE[i]);

        textura->render();
//        glBegin(GL_QUADS);
            /*glTexCoord2f(0.0, 0.0);
            glVertex3f(textura->get_v1()[0], textura->get_v1()[1], textura->get_v1()[2]);
            glTexCoord2f(0.0, 1.0);
            glVertex3f(textura->get_v2()[0], textura->get_v2()[1], textura->get_v2()[2]);
            glTexCoord2f(1.0, 1.0);
            glVertex3f(textura->get_v3()[0], textura->get_v3()[1], textura->get_v3()[2]);
            glTexCoord2f(1.0, 0.0);
            glVertex3f(textura->get_v4()[0], textura->get_v4()[1], textura->get_v4()[2]);*/

  //      glEnd();
    }


}


void Escenadrawer::resizeGL(int width, int height)
{
    double aspectRatio = (float) width / (float) height;
    double fieldOfView = 65; 

    buildPerspectiveMatrix(fieldOfView, aspectRatio,
                         0.1, 100.0, 
                         myProjectionMatrix);
    glViewport(0, 0, width, height);
   //textura->resize(width,height);
}

void Escenadrawer::keyP( int k ) {
    float step = 0.2f;
    switch(k) {
        case 65: eye_x += step;
                 break;
        case 68: eye_x -= step;
                 break;
        case 71: eye_y += step;
                 break;
        case 72: eye_y -= step;
                 break;
        case 83: eye_z += step; 
                 break;
        case 87: eye_z -= step;
                 break;
    }
    emit cambiarEye(eye_x,eye_y,eye_z);
    updateGL();

}

