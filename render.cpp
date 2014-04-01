// encapsula la implementacion
// cualquier implementacion heredara de esta clase

#include<stdio.h>
#include<stdlib.h>
#include <math.h>     /* for sqrt, sin, cos, and fabs */
#include <assert.h>   /* for assert */

// Clases FWK
#include "render.h"
#include "textura.h"


/*#include "spotnoise.h"
#include "spotnoise2.h"
#include "spotnoise3.h"*/

// Cg, OpenGL
#include <GL/glut.h>

#include <iostream>
using namespace std;

enum {
  TO_TEXTURA_0 = 0,
  TO_TEXTURA_1 = 1,
  TO_TEXTURA_2 = 2
};

CGcontext   myCgContext;

CGprofile   myCgFragmentProfile,
            myCgVertexProfile;

CGparameter cg_modelViewProj;

GLubyte texturas[CANT_T_PARAMETERS][tamanio];
GLuint texObjR[CANT_T_PARAMETERS];

CGparameter texturesParametersR[CANT_T_PARAMETERS];
CGparameter combinaParametersR[CANT_C_PARAMETERS];
CGparameter fragmentParametersR[CANT_F_PARAMETERS];

const char *myProgramName = "01_vertex_program",
                        *vertex_program_name = "vertexE.cg",
                        *fragment_program_name = "fragment.cg";

const char* tNamesParametersR[] = { 
	                  "textura0",
           	          "textura1",
	                  "textura2", };

const char* cNamesParametersR[] = {
              "combina1",
              "combina2",
};

// Nombres de los parametros en el fragment shader
const char* fNamesParametersR[] = { 
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
              "gama",
};


// parametros dinamicos de las texturas, valores por default
float fArrayParametersR[][CANT_T_PARAMETERS] = {
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

int cArrayParametersR[CANT_C_PARAMETERS] = {
    0,0, // funciones de combinacion    
};


void Render::_checkForCgError(const char *situation)
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


/************************************/
// Codigo para pasar la modelviewproj al programa
// El codigo esta copiado del ejemplo ..vertex.. de cg

/* Forward declared routine used by reshape callback. */
static void buildPerspectiveMatrix(double, double,
                                   double, double, float m[16]);

static const double myPi = 3.14159265358979323846;

static float myEyeAngle = 0;   /* Angle eye rotates around scene. */
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




Render::Render(Textura* t)
{
    textura = t;
}

void Render::setear_valores_textura(void) {

    fArrayParametersR[0][0] = textura->get_usa();
    fArrayParametersR[1][0] = textura->get_amplitud();
    fArrayParametersR[2][0] = textura->get_turbulencia();
    fArrayParametersR[3][0] = textura->get_frecuencia();
    fArrayParametersR[4][0] = textura->get_intensidad();
    fArrayParametersR[5][0] = textura->get_base();
    fArrayParametersR[6][0] = textura->get_angulo();
    fArrayParametersR[7][0] = textura->get_desplazamiento_x();
    fArrayParametersR[8][0] = textura->get_desplazamiento_y();
    fArrayParametersR[9][0] = textura->get_tipo_turb();
    fArrayParametersR[10][0] = textura->get_frecuencia_v();
    fArrayParametersR[11][0] = textura->get_alpha();
    fArrayParametersR[12][0] = textura->get_beta();
    fArrayParametersR[13][0] = textura->get_gama();
    
    Textura *t = textura->get_siguiente();

    if(t != NULL) { cArrayParametersR[0] = textura->get_combina(); }

    int i = 1;
    while(t != NULL) {
        fArrayParametersR[0][i] = t->get_usa();
        fArrayParametersR[1][i] = t->get_amplitud();
        fArrayParametersR[2][i] = t->get_turbulencia();
        fArrayParametersR[3][i] = t->get_frecuencia();
        fArrayParametersR[4][i] = t->get_intensidad();
        fArrayParametersR[5][i] = t->get_base();
        fArrayParametersR[6][i] = t->get_angulo();
        fArrayParametersR[7][i] = t->get_desplazamiento_x();
        fArrayParametersR[8][i] = t->get_desplazamiento_y();
        fArrayParametersR[9][i] = t->get_tipo_turb();
        fArrayParametersR[10][i] = t->get_frecuencia_v();
        fArrayParametersR[11][i] = t->get_alpha();
        fArrayParametersR[12][i] = t->get_beta();
        fArrayParametersR[13][i] = t->get_gama();

        Textura* t2 = t;
        t = t->get_siguiente();

        if(i <= CANT_T_PARAMETERS-2) {
            if( (cArrayParametersR[i] = t2->get_combina() ) == NO_COMB && t != NULL) {
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
}

void Render::init() {
    glClearColor(0.3, 0.3, 0.3, 0.0);

    glEnable(GL_DEPTH_TEST);

    for(int i = 0; i< tamanio; i++)
    {
/*        texturas[0][i] = (GLubyte)text0[i];
        texturas[1][i] = (GLubyte)text1[i];
        texturas[2][i] = (GLubyte)text2[i];*/
    }

    glutInitWindowSize(700, 700);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures( CANT_T_PARAMETERS, texObjR );

    ///////////
    for(int i = 0; i < CANT_T_PARAMETERS; i++) {
        glBindTexture(GL_TEXTURE_2D, texObjR[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_LUMINANCE,
            NX, NY, GL_LUMINANCE, GL_UNSIGNED_BYTE, texturas[i]);
    }

    ///////////


    myCgContext = cgCreateContext();
    _checkForCgError("creating context");
    cgGLSetDebugMode(CG_FALSE);
    cgSetParameterSettingMode(myCgContext, CG_DEFERRED_PARAMETER_SETTING);

    myCgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
    cgGLSetOptimalOptions(myCgVertexProfile);
    _checkForCgError("selecting vertex profile");

    vertex =
        cgCreateProgramFromFile(
          myCgContext,              /* Cg runtime context */
          CG_SOURCE,                /* Program in human-readable form */
          vertex_program_name,
          myCgVertexProfile,        /* Profile: latest fragment profile */
          "main",                   /* Entry function name */
          NULL); /* No extra compiler options */
    _checkForCgError("creating vertex program from string");
    cgGLLoadProgram(vertex);
    _checkForCgError("loading vertex program");

    myCgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
    cgGLSetOptimalOptions(myCgFragmentProfile);
    _checkForCgError("selecting fragment profile");

    /* Specify fragment program with a string. */
    fragment =
        cgCreateProgramFromFile(
          myCgContext,              /* Cg runtime context */
          CG_SOURCE,                /* Program in human-readable form */
          fragment_program_name,
          myCgFragmentProfile,      /* Profile: latest fragment profile */
          "main",                   /* Entry function name */
          NULL); /* No extra compiler options */

    _checkForCgError("creating fragment program from string");
    cgGLLoadProgram(fragment);
    _checkForCgError("loading fragment program");

    // binding con parametros del fragment shader
    for (int i = 0; i < CANT_T_PARAMETERS; i++) {
	    texturesParametersR[i] = cgGetNamedParameter(fragment, (char*)tNamesParametersR[i]);
        _checkForCgError("could not get texture parameter");

        cgGLSetTextureParameter(texturesParametersR[i], texObjR[i]);
        _checkForCgError("setting textura texture");
    }

    // binding con parametros del fragment shader
    for (int i = 0; i < CANT_F_PARAMETERS; i++) {
	    fragmentParametersR[i] = cgGetNamedParameter(fragment, (char*)fNamesParametersR[i]);
        _checkForCgError("could not get fragment parameter");
    }

    // binding con parametros del fragment shader (combinacion de texturas)
    for (int i = 0; i < CANT_C_PARAMETERS; i++) {
	    combinaParametersR[i] = cgGetNamedParameter(fragment, (char*)cNamesParametersR[i]);
        _checkForCgError("could not get combina parameter");
    }

    cg_modelViewProj = cgGetNamedParameter(vertex, "modelViewProj");
    _checkForCgError("could not get modelViewProj parameter");

}

void Render::paint()
{
 /*   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i< CANT_T_PARAMETERS; i++) {
        cgGLEnableTextureParameter(texturesParametersR[i]);
        _checkForCgError("enabling textures");
    }

    setear_valores_textura();

    float translateMatrix[16], rotateMatrix[16],
        modelMatrix[16], viewMatrix[16],
        modelViewMatrix[16], modelViewProjMatrix[16];

    makeRotateMatrix(0, 1, 1, 1, rotateMatrix);
    makeTranslateMatrix(0, 0, 0, translateMatrix);
    multMatrix(modelMatrix, translateMatrix, rotateMatrix);

    multMatrix(modelViewMatrix, viewMatrix, modelMatrix);

    multMatrix(modelViewProjMatrix, myProjectionMatrix, modelViewMatrix);

    // paso al shader
    cgSetMatrixParameterfr(cg_modelViewProj, modelViewProjMatrix);

    buildLookAtMatrix(13*sin(myEyeAngle), 0, 13*cos(myEyeAngle),  
                    0, 0, 0, 
                    0, 1, 0, 
                    viewMatrix);

    for (int i = 0; i< CANT_T_PARAMETERS; i++) {
        cgGLEnableTextureParameter(texturesParametersR[i]);
        _checkForCgError("enabling textures");
    }

    cgGLEnableProfile(myCgVertexProfile);
    _checkForCgError("enabling vertex profile");

    cgGLBindProgram(fragment);
    _checkForCgError("binding fragment program");

    cgGLEnableProfile(myCgFragmentProfile);
    _checkForCgError("enabling fragment profile");

    glClearColor(0.0, 0.0, 0.0, 0);

    // paso al shader
    for(int i = 0; i < CANT_F_PARAMETERS; i++) {
        cgGLSetParameterArray1f(fragmentParametersR[i],
                                         0,
                                         CANT_T_PARAMETERS,
                                         fArrayParametersR[i]);
    }

    for(int i = 0; i < CANT_C_PARAMETERS; i++) {
        cgGLSetParameter1f(combinaParametersR[i],                                       
                                         cArrayParametersR[i]);
    }

    cgUpdateProgramParameters(vertex);
    cgUpdateProgramParameters(fragment);*/

    // texture mapping
/*    for (int i = 0; i  < CANT_T_PARAMETERS; i++) {
        glBindTexture(GL_TEXTURE_2D, texObjR[i]);*/

        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);
            glVertex3f(textura->get_v1()[0], textura->get_v1()[1], textura->get_v1()[2]);
            glTexCoord2f(0.0, 1.0);
            glVertex3f(textura->get_v2()[0], textura->get_v2()[1], textura->get_v2()[2]);
            glTexCoord2f(1.0, 1.0);
            glVertex3f(textura->get_v3()[0], textura->get_v3()[1], textura->get_v3()[2]);
            glTexCoord2f(1.0, 0.0);
            glVertex3f(textura->get_v4()[0], textura->get_v4()[1], textura->get_v4()[2]);
        glEnd();
    //}

/*    glFlush();

    cgGLDisableProfile(myCgVertexProfile);
    _checkForCgError("disabling vertex profile");

    cgGLDisableProfile(myCgFragmentProfile);
    _checkForCgError("disabling fragment profile");*/

}

void Render::resize(int width, int height)
{
  double aspectRatio = (float) width / (float) height;
  double fieldOfView = 15;

  buildPerspectiveMatrix(fieldOfView, aspectRatio,
                         1.0, 20.0,
                         myProjectionMatrix);
  glViewport(0, 0, width, height);
}



void Render::debug()
{
    textura->debug();
}


/*
void Render::render(int argc,char ** argv)
{
	glutInit(&argc, argv);

	glutInitWindowSize(winH, winW);

	glutCreateWindow ("FRAMEWORK");
	
	glutDisplayFunc (paint);

	glutReshapeFunc (resize);
	
	initialize();
	
	glutMainLoop();
}*/
