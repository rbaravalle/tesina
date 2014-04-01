#include <stdio.h>
#include <stdlib.h>
#include <math.h>     /* for sqrt, sin, cos, and fabs */
#include <assert.h>   /* for assert */

#include<previewdrawer.h>
//#include <spotnoise.h>
//#include <spotnoise2.h>
//#include <spotnoise3.h>

#include<QtOpenGL/QGLWidget>
#include <GL/glu.h>

#include<iostream>
using namespace std;

//***************//
// CODIGO OPENGL //  
//***************//

#define NX 256
#define NY 256

#define tamanio (NX*NY)

int winW = 500; /* window width */
int winH = 500; /* window height */

static CGcontext   myCgContext;

static CGprogram   myCgFragmentProgram,
                    myCgVertexProgram;

static CGprofile   myCgFragmentProfile,
                   myCgVertexProfile;

static const char *myProgramName = "preview shader",
                *vertex_program_name = "/home/rodrigo/frameworkF/vertex.cg",
                *fragment_program_name = "/home/rodrigo/frameworkF/fragment.cg";

enum {
  TO_TEXTURA_0 = 0,
  TO_TEXTURA_1 = 1,
  TO_TEXTURA_2 = 2
};

static GLubyte texturas[CANT_T_PARAMETERS][tamanio];
GLuint texObj[CANT_T_PARAMETERS];

CGparameter texturesParameters[CANT_T_PARAMETERS];
CGparameter combinaParameters[CANT_C_PARAMETERS];
CGparameter fragmentParameters[CANT_F_PARAMETERS];
CGparameter colorParameter;
CGparameter antialiasParameter;

const char* tNamesParameters[] = { 
	          "textura0",
   	          "textura1",
	          "textura2",
              "filtermap" };

const char* cNamesParameters[] = {
              "combina1",
              "combina2",
};

// Nombres de los parametros en el fragment shader
const char* fNamesParameters[] = { 
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
float fArrayParameters[][CANT_T_PARAMETERS-1] = {
    {1,1,1}, // usa
    {1,1,1}, // amplitud
    {0,0,0}, // turb
    {20,20,20}, // escala_u
    {1,1,1}, // intensidad
    {0,0,0}, // base
    {0,0,0}, // angulos
    {0,0,0}, // desplazamiento_u
    {0,0,0}, // desplazamiento_v
    {0,0,0}, // tipo turbulencia
    {1,1,1}, // escala_v
    {1,1,1}, // alpha
    {1,1,1}, // beta
    {1,1,1}, // gama
};

int cArrayParameters[CANT_C_PARAMETERS] = {
    0,0, // funciones de combinacion    
};

float colorArrayParameters[3] = {
    0.5,0.5,0.5 // color (rgb)
};


float vertices[4][3] = {
    {-1.0, -1.0, 0.0},
    {-1.0,  1.0, 0.0},
    { 1.0,  1.0, 0.0},
    { 1.0, -1.0, 0.0}
};

int antialias = 1;

//*******************//
// FIN CODIGO OPENGL //
//*******************//


// RUTINAS DE LA CLASE

void Previewdrawer::_checkForCgError(const char *situation)
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

/*void Previewdrawer::_cargar_texturas()
{
    for(int i = 0; i< tamanio; i++)
    {
        texturas[0][i] = (GLubyte)text0[i];
        texturas[1][i] = (GLubyte)text1[i];
        texturas[2][i] = (GLubyte)text2[i];
    }

}*/

void Previewdrawer::cambiarUsa1()
{
    fArrayParameters[0][0] =  fArrayParameters[0][0] == 1 ? 0 : 1;
    textura->set_usa(fArrayParameters[0][0]);
    updateGL();
}

void Previewdrawer::cambiarUsa2()
{
    fArrayParameters[0][1] =  fArrayParameters[0][1] == 1 ? 0 : 1;
    if(sig) sig->set_usa(fArrayParameters[0][1]);
    updateGL();
}

void Previewdrawer::cambiarUsa3()
{
    fArrayParameters[0][2] =  fArrayParameters[0][2] == 1 ? 0 : 1;
    if(sigsig) sigsig->set_usa(fArrayParameters[0][2]);
    updateGL();
}

void Previewdrawer::cambiarAmplitud1(int amplitudParam)
{
    fArrayParameters[1][0] = (float)amplitudParam/100;
    textura->set_amplitud((float)amplitudParam/100);
    updateGL();
}

void Previewdrawer::cambiarAmplitud2(int amplitudParam)
{
    fArrayParameters[1][1] = (float)amplitudParam/100;
    if(sig) sig->set_amplitud((float)amplitudParam/100);
    updateGL();
}

void Previewdrawer::cambiarAmplitud3(int amplitudParam)
{
    fArrayParameters[1][2] = (float)amplitudParam/100;
    if(sigsig) sigsig->set_amplitud((float)amplitudParam/100);
    updateGL();
}

void Previewdrawer::cambiarTurb1(int turbParam)
{
    fArrayParameters[2][0] = (float)turbParam/100;
    textura->set_turbulencia((float)turbParam/100);
    updateGL();
}

void Previewdrawer::cambiarTurb2(int turbParam)
{
    fArrayParameters[2][1] = (float)turbParam/100;
    if(sig) sig->set_turbulencia((float)turbParam/100);
    updateGL();
}

void Previewdrawer::cambiarTurb3(int turbParam)
{
    fArrayParameters[2][2] = (float)turbParam/100;
    if(sigsig) sigsig->set_turbulencia((float)turbParam/100);
    updateGL();
}

void Previewdrawer::cambiarFrecuencia1(int frec)
{
    fArrayParameters[3][0] = (float)frec;
    textura->set_frecuencia((float)frec);
    updateGL();
}

void Previewdrawer::cambiarFrecuencia2(int frec)
{
    fArrayParameters[3][1] = (float)frec;
    if(sig) sig->set_frecuencia((float)frec);
    updateGL();}

void Previewdrawer::cambiarFrecuencia3(int frec)
{
    fArrayParameters[3][2] = (float)frec;
    if(sigsig) sigsig->set_frecuencia((float)frec);
    updateGL();
}


void Previewdrawer::cambiarFrecuenciaV1(int frec)
{
    fArrayParameters[10][0] = (float)frec;
    textura->set_frecuencia_v((float)frec);
    updateGL();
}

void Previewdrawer::cambiarFrecuenciaV2(int frec)
{
    fArrayParameters[10][1] = (float)frec;
    if(sig) sig->set_frecuencia_v((float)frec);
    updateGL();}

void Previewdrawer::cambiarFrecuenciaV3(int frec)
{
    fArrayParameters[10][2] = (float)frec;
    if(sigsig) sigsig->set_frecuencia_v((float)frec);
    updateGL();
}

void Previewdrawer::cambiarIntensidad1(int intens)
{
    fArrayParameters[4][0] = (float)intens/100;
    textura->set_intensidad((float)intens/100);
    updateGL();
}

void Previewdrawer::cambiarIntensidad2(int intens)
{
    fArrayParameters[4][1] = (float)intens/100;
    if(sig) sig->set_intensidad((float)intens/100);
    updateGL();
}

void Previewdrawer::cambiarIntensidad3(int intens)
{
    fArrayParameters[4][2] = (float)intens/100;
    if(sigsig) sigsig->set_intensidad((float)intens/100);
    updateGL();
}


void Previewdrawer::cambiarBase1(int b)
{
    fArrayParameters[5][0] = b;
    textura->set_base(b);
    updateGL();
}

void Previewdrawer::cambiarBase2(int b)
{
    fArrayParameters[5][1] = b;
    if(sig) sig->set_base(b);
    updateGL();
}


void Previewdrawer::cambiarBase3(int b)
{
    fArrayParameters[5][2] = b;
    if(sigsig) sigsig->set_base(b);
    updateGL();
}


////////

void Previewdrawer::cambiarAngulo1(int angParam)
{
    fArrayParameters[6][0] = (float)angParam/100;
    textura->set_angulo((float)angParam/100);
    updateGL();
}

void Previewdrawer::cambiarAngulo2(int angParam)
{
    fArrayParameters[6][1] = (float)angParam/100;
    if(sig) sig->set_angulo((float)angParam/100);
    updateGL();
}

void Previewdrawer::cambiarAngulo3(int angParam)
{
    fArrayParameters[6][2] = (float)angParam/100;
    if(sigsig) sigsig->set_angulo((float)angParam/100);
    updateGL();
}

void Previewdrawer::cambiarDesplazamiento_u1(int d)
{
    fArrayParameters[7][0] = (float)d/100;
    textura->set_desplazamiento_x((float)d/100);
    updateGL();
}

void Previewdrawer::cambiarDesplazamiento_u2(int d)
{
    fArrayParameters[7][1] = (float)d/100;
    if(sig) sig->set_desplazamiento_x((float)d/100);
    updateGL();
}

void Previewdrawer::cambiarDesplazamiento_u3(int d)
{
    fArrayParameters[7][2] = (float)d/100;
    if(sigsig) sigsig->set_desplazamiento_x((float)d/100);
    updateGL();
}

void Previewdrawer::cambiarDesplazamiento_v1(int d)
{
    fArrayParameters[8][0] = (float)d/100;
    textura->set_desplazamiento_y((float)d/100);
    updateGL();
}

void Previewdrawer::cambiarDesplazamiento_v2(int d)
{
    fArrayParameters[8][1] = (float)d/100;
    if(sig) sig->set_desplazamiento_y((float)d/100);
    updateGL();
}

void Previewdrawer::cambiarDesplazamiento_v3(int d)
{
    fArrayParameters[8][2] = (float)d/100;
    if(sigsig) sigsig->set_desplazamiento_y((float)d/100);
    updateGL();
}

void Previewdrawer::cambiarTipoTurb1(int t) {
    fArrayParameters[9][0] = t;
    textura->set_tipo_turb(t);
    updateGL();
}

void Previewdrawer::cambiarTipoTurb2(int t) {
    fArrayParameters[9][1] = t;
    if(sig) sig->set_tipo_turb(t);
    updateGL();
}

void Previewdrawer::cambiarTipoTurb3(int t) {
    fArrayParameters[9][2] = t;
    if(sigsig) sigsig->set_tipo_turb(t);
    updateGL();
}



void Previewdrawer::cambiarCombina1(int d)
{
    cArrayParameters[0] = d;
    textura->set_combina(d);
    updateGL();
}

void Previewdrawer::cambiarCombina2(int d)
{
    cArrayParameters[1] = d;
    if(sig) sig->set_combina(d);
    updateGL();
}

void Previewdrawer::cambiarVertice11(QString f)
{
    float p = f.toFloat();
    vertices[0][0] = p;
    textura->set_v1(vertices[0]);
}

void Previewdrawer::cambiarVertice12(QString f)
{
    float p = f.toFloat();
    vertices[0][1] = p;
    textura->set_v1(vertices[0]);
}

void Previewdrawer::cambiarVertice13(QString f)
{
    float p = f.toFloat();
    vertices[0][2] = p;
    textura->set_v1(vertices[0]);
}

void Previewdrawer::cambiarVertice21(QString f)
{
    float p = f.toFloat();
    vertices[1][0] = p;
    textura->set_v2(vertices[1]);
}

void Previewdrawer::cambiarVertice22(QString f)
{
    float p = f.toFloat();
    vertices[1][1] = p;
    textura->set_v2(vertices[1]);
}

void Previewdrawer::cambiarVertice23(QString f)
{
    float p = f.toFloat();
    vertices[1][2] = p;
    textura->set_v2(vertices[1]);
}

void Previewdrawer::cambiarVertice31(QString f)
{
    float p = f.toFloat();
    vertices[2][0] = p;
    textura->set_v3(vertices[2]);
}

void Previewdrawer::cambiarVertice32(QString f)
{
    float p = f.toFloat();
    vertices[2][1] = p;
    textura->set_v3(vertices[2]);
}

void Previewdrawer::cambiarVertice33(QString f)
{
    float p = f.toFloat();
    vertices[2][2] = p;
    textura->set_v3(vertices[2]);
}

void Previewdrawer::cambiarVertice41(QString f)
{
    float p = f.toFloat();
    vertices[3][0] = p;
    textura->set_v4(vertices[3]);
}

void Previewdrawer::cambiarVertice42(QString f)
{
    float p = f.toFloat();
    vertices[3][1] = p;
    textura->set_v4(vertices[3]);
}

void Previewdrawer::cambiarVertice43(QString f)
{
    float p = f.toFloat();
    vertices[3][2] = p;
    textura->set_v4(vertices[3]);
}


void Previewdrawer::cambiarAlpha1(int a) {
    fArrayParameters[11][0] = (float)a/100;
    textura->set_alpha((float)a/100);
    updateGL();
}

void Previewdrawer::cambiarAlpha2(int a) {
    fArrayParameters[11][1] = (float)a/100;
    if(sig) sig->set_alpha((float)a/100);
    updateGL();
}

void Previewdrawer::cambiarAlpha3(int a) {
    fArrayParameters[11][2] = (float)a/100;
    if(sigsig) sigsig->set_alpha((float)a/100);
    updateGL();
}

void Previewdrawer::cambiarBeta1(int a) {
    fArrayParameters[12][0] = (float)a/100;
    textura->set_beta((float)a/100);
    updateGL();
}

void Previewdrawer::cambiarBeta2(int a) {
    fArrayParameters[12][1] = (float)a/100;
    if(sig) sig->set_beta((float)a/100);
    updateGL();
}

void Previewdrawer::cambiarBeta3(int a) {
    fArrayParameters[12][2] = (float)a/100;
    if(sigsig) sigsig->set_beta((float)a/100);
    updateGL();
}

void Previewdrawer::cambiarGama1(int a) {
    fArrayParameters[13][0] = (float)a/100;
    textura->set_gama((float)a/100);
    updateGL();
}

void Previewdrawer::cambiarGama2(int a) {
    fArrayParameters[13][1] = (float)a/100;
    if(sig) sig->set_gama((float)a/100);
    updateGL();
}

void Previewdrawer::cambiarGama3(int a) {
    fArrayParameters[13][2] = (float)a/100;
    if(sigsig) sigsig->set_gama((float)a/100);
    updateGL();
}

void Previewdrawer::cambiarColorR(int r) {
    colorArrayParameters[0] = (float)r/255;
    textura->set_color_r((float)r/255);
    updateGL();
}

void Previewdrawer::cambiarColorG(int g) {
    colorArrayParameters[1] = (float)g/255;
    textura->set_color_g((float)g/255);
    updateGL();
}

void Previewdrawer::cambiarColorB(int b) {
    colorArrayParameters[2] = (float)b/255;
    textura->set_color_b((float)b/255);
    updateGL();
}

void Previewdrawer::cambiarAntialias() {
    antialias = antialias == 1 ? 0 : 1;
    textura->set_antialias(antialias);
    updateGL();
}



void Previewdrawer::setear_valores(void) {
    fArrayParameters[0][0] = textura->get_usa();
    if(sig) fArrayParameters[0][1] = sig->get_usa();
    if(sigsig) fArrayParameters[0][2] = sigsig->get_usa();

    fArrayParameters[1][0] = textura->get_amplitud();
    if(sig) fArrayParameters[1][1] = sig->get_amplitud();
    if(sigsig) fArrayParameters[1][2] = sigsig->get_amplitud();

    fArrayParameters[2][0] = textura->get_turbulencia();
    if(sig) fArrayParameters[2][1] = sig->get_turbulencia();
    if(sigsig) fArrayParameters[2][2] = sigsig->get_turbulencia();

    fArrayParameters[3][0] = textura->get_frecuencia();
    if(sig) fArrayParameters[3][1] = sig->get_frecuencia();
    if(sigsig) fArrayParameters[3][2] = sigsig->get_frecuencia();

    fArrayParameters[4][0] = textura->get_intensidad();
    if(sig) fArrayParameters[4][1] = sig->get_intensidad();
    if(sigsig) fArrayParameters[4][2] = sigsig->get_intensidad();

    fArrayParameters[5][0] = textura->get_base();
    if(sig) fArrayParameters[5][1] = sig->get_base();
    if(sigsig) fArrayParameters[5][2] = sigsig->get_base();

    fArrayParameters[6][0] = textura->get_angulo();
    if(sig) fArrayParameters[6][1] = sig->get_angulo();
    if(sigsig) fArrayParameters[6][2] = sigsig->get_angulo();

    fArrayParameters[7][0] = textura->get_desplazamiento_x();
    if(sig) fArrayParameters[7][1] = sig->get_desplazamiento_x();
    if(sigsig) fArrayParameters[7][2] = sigsig->get_desplazamiento_x();

    fArrayParameters[8][0] = textura->get_desplazamiento_y();
    if(sig) fArrayParameters[8][1] = sig->get_desplazamiento_y();
    if(sigsig) fArrayParameters[8][2] = sigsig->get_desplazamiento_y();

    fArrayParameters[9][0] = textura->get_tipo_turb();
    if(sig) fArrayParameters[9][1] = sig->get_tipo_turb();
    if(sigsig) fArrayParameters[9][2] = sigsig->get_tipo_turb();

    fArrayParameters[10][0] = textura->get_frecuencia_v();
    if(sig) fArrayParameters[10][1] = sig->get_frecuencia_v();
    if(sigsig) fArrayParameters[10][2] = sigsig->get_frecuencia_v();

    fArrayParameters[11][0] = textura->get_alpha();
    if(sig) fArrayParameters[11][1] = sig->get_alpha();
    if(sigsig) fArrayParameters[11][2] = sigsig->get_alpha();

    fArrayParameters[12][0] = textura->get_beta();
    if(sig) fArrayParameters[12][1] = sig->get_beta();
    if(sigsig) fArrayParameters[12][2] = sigsig->get_beta();

    fArrayParameters[13][0] = textura->get_gama();
    if(sig) fArrayParameters[13][1] = sig->get_gama();
    if(sigsig) fArrayParameters[13][2] = sigsig->get_gama();

    // combinacion
    cArrayParameters[0] = textura->get_combina();
    if(sig) cArrayParameters[1] = sig->get_combina();

    // vertices
    vertices[0][0] = textura->get_v1()[0];
    vertices[0][1] = textura->get_v1()[1];
    vertices[0][2] = textura->get_v1()[2];

    vertices[1][0] = textura->get_v2()[0];
    vertices[1][1] = textura->get_v2()[1];
    vertices[1][2] = textura->get_v2()[2];

    vertices[2][0] = textura->get_v3()[0];
    vertices[2][1] = textura->get_v3()[1];
    vertices[2][2] = textura->get_v3()[2];

    vertices[3][0] = textura->get_v4()[0];
    vertices[3][1] = textura->get_v4()[1];
    vertices[3][2] = textura->get_v4()[2];

    // color
    colorArrayParameters[0] = textura->get_color_r();
    colorArrayParameters[1] = textura->get_color_g();
    colorArrayParameters[2] = textura->get_color_b();

    antialias = textura->get_antialias();

}


// tamanio de la pantalla
QSize Previewdrawer::minimumSizeHint() const
 {
     return QSize(winW, winH);
 }

QSize Previewdrawer::sizeHint() const
 {
     return QSize(winW, winH);
 }

void Previewdrawer::initializeGL()
{   
    
    glClearColor(0, 0, 0, 0.0);

    glEnable(GL_DEPTH_TEST);

    //_cargar_texturas();

    glutInitWindowSize(winW, winH);


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures( CANT_T_PARAMETERS, texObj );


    ///////////
    for(int i = 0; i < CANT_T_PARAMETERS; i++) {
        glBindTexture(GL_TEXTURE_2D, texObj[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_LUMINANCE,
            NX, NY, GL_LUMINANCE, GL_UNSIGNED_BYTE, texturas[i]);
    }

    ///////////

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
          myCgContext,              /* Cg runtime context */
          CG_SOURCE,                /* Program in human-readable form */
          vertex_program_name,
          myCgVertexProfile,        /* Profile: latest fragment profile */
          "main",                   /* Entry function name */
          NULL); /* No extra compiler options */
    _checkForCgError("creating vertex program from string");
    cgGLLoadProgram(myCgVertexProgram);
    _checkForCgError("loading vertex program");

    myCgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
    cgGLSetOptimalOptions(myCgFragmentProfile);
    _checkForCgError("selecting fragment profile");

    /* Specify fragment program with a string. */
    myCgFragmentProgram =
        cgCreateProgramFromFile(
          myCgContext,              /* Cg runtime context */
          CG_SOURCE,                /* Program in human-readable form */
          fragment_program_name,
          myCgFragmentProfile,      /* Profile: latest fragment profile */
          "main",                   /* Entry function name */
          NULL); /* No extra compiler options */

    _checkForCgError("creating fragment program from string");
    cgGLLoadProgram(myCgFragmentProgram);
    _checkForCgError("loading fragment program");

    // binding con parametros del fragment shader
    for (int i = 0; i < CANT_T_PARAMETERS; i++) {
	    texturesParameters[i] = cgGetNamedParameter(myCgFragmentProgram, (char*)tNamesParameters[i]);

        _checkForCgError("could not get texture parameter");

        cgGLSetTextureParameter(texturesParameters[i], texObj[i]);

        _checkForCgError("setting textura texture");

    }

    // binding con parametros del fragment shader
    for (int i = 0; i < CANT_F_PARAMETERS; i++) {
	    fragmentParameters[i] = cgGetNamedParameter(myCgFragmentProgram, (char*)fNamesParameters[i]);

        _checkForCgError("could not get fragment parameter");
    }

    // binding con parametros del fragment shader (combinacion de texturas)
    for (int i = 0; i < CANT_C_PARAMETERS; i++) {
	    combinaParameters[i] = cgGetNamedParameter(myCgFragmentProgram, (char*)cNamesParameters[i]);

        _checkForCgError("could not get combina parameter");
    }

    colorParameter = cgGetNamedParameter(myCgFragmentProgram, "color");

    antialiasParameter = cgGetNamedParameter(myCgFragmentProgram, "antialias");

    setear_valores();

}

void Previewdrawer::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i< CANT_T_PARAMETERS; i++) {
        cgGLEnableTextureParameter(texturesParameters[i]);
        _checkForCgError("enabling textures");
    }

    cgGLEnableProfile(myCgVertexProfile);
    _checkForCgError("enabling vertex profile");

    cgGLBindProgram(myCgFragmentProgram);
    _checkForCgError("binding fragment program");

    cgGLEnableProfile(myCgFragmentProfile);
    _checkForCgError("enabling fragment profile");

    glClearColor(0.0, 0.0, 0.0, 0);

    for(int i = 0; i < CANT_F_PARAMETERS; i++) {
        cgGLSetParameterArray1f(fragmentParameters[i],
                                         0,
                                         CANT_T_PARAMETERS-1,
                                         fArrayParameters[i]);
    }

    for(int i = 0; i < CANT_C_PARAMETERS; i++) {
        cgGLSetParameter1f(combinaParameters[i],                                       
                                         cArrayParameters[i]);
    }

    cgGLSetParameterArray1f(colorParameter,
                            0,
                            3,
                            colorArrayParameters);

    cgGLSetParameter1f(antialiasParameter, antialias);

    cgUpdateProgramParameters(myCgVertexProgram);
    cgUpdateProgramParameters(myCgFragmentProgram);


    for (int i = 0; i  < CANT_T_PARAMETERS-1; i++) {
        glBindTexture(GL_TEXTURE_2D, texObj[i]);

        glBegin(GL_QUADS);
                glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, 0.0);
                glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0, 0.0);
                glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
                glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
        glEnd();
    }
    cgGLDisableProfile(myCgVertexProfile);
    _checkForCgError("disabling vertex profile");

    cgGLDisableProfile(myCgFragmentProfile);
    _checkForCgError("disabling fragment profile");

}

void Previewdrawer::resizeGL(int width, int height)
{
    double aspectRatio = (float) width / (float) height;
    double fieldOfView = 75.0; 
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fieldOfView, aspectRatio,
    0.1,    
    100.0); 
    glMatrixMode(GL_MODELVIEW);

    glViewport(0, 0, width, height);
}

