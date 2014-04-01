#ifndef TEXTURAS_H
#define TEXTURAS_H

// abstraccion de un material
// puede combinarse con otras texturas para producir otro material

// uso
// #include "textura.h"
// Textura t, t2;
// t.set_intensidad(0.8);
// t.blend(&t2,0.8);
// t.debug();

// known issues :
//    * no usar texturas mas de une vez en una combinacion (loops infinitos)
//    * las texturas se combinan de a una a la vez, en forma de lista

// texturas en la base
#define TEX1   0
#define TEX2   1
#define TEX3   2
#define TEX4   3
#define TEX5   4

// combinaciones
#define NO_COMB -1
#define ADD   0
#define SUB   1
#define LERP  2
#define SOBRE 3
#define MUL   4

#include "render.h"

class Textura
{
    public:
        // textura inicial
        Textura();

        Textura* add(Textura*);
        Textura* sub(Textura*);
        Textura* mul(Textura*);
        Textura* sobre(Textura*);
        Textura* blend(Textura*,float);

        Textura* set_intensidad(float);
        Textura* set_amplitud(float);
        Textura* set_turbulencia(float);
        Textura* set_angulo(float);
        Textura* set_frecuencia(float);
        Textura* set_frecuencia_v(float);
        Textura* set_desplazamiento_x(float);
        Textura* set_desplazamiento_y(float);
        Textura* set_v1(float*);
        Textura* set_v2(float*);
        Textura* set_v3(float*);
        Textura* set_v4(float*);

        // definicion de la textura
        Textura* set_base(int);
        Textura* set_usa(int);
        Textura* set_combina(int);
        Textura* set_antialias(int);
        Textura* set_tipo_turb(int);
        Textura* set_siguiente(Textura*);
        
        Textura* set_alpha(float);
        Textura* set_beta(float);
        Textura* set_gama(float);

        Textura* set_color_r(float);
        Textura* set_color_g(float);
        Textura* set_color_b(float);

        // pasar al shader el Material
        void renderInit();
        void render();
        void resize(int, int);

        int get_base();
        int get_usa();
        int get_combina();
        int get_antialias();
        float get_amplitud(void);
        float get_turbulencia(void);
        float get_frecuencia(void);
        float get_frecuencia_v(void);
        float get_intensidad(void);
        float get_angulo(void);
        float get_desplazamiento_x(void);
        float get_desplazamiento_y(void);
        float* get_v1(void);
        float* get_v2(void);
        float* get_v3(void);
        float* get_v4(void);
        int get_tipo_turb(void);

        float get_alpha(void);
        float get_beta(void);
        float get_gama(void);

        float get_color_r(void);
        float get_color_g(void);
        float get_color_b(void);

        Textura* get_siguiente(void);

        void debug(void);
        void debug_props(void);

    private:
        int base;
        int usa;
        int combina;
        int antialias;
        float intensidad;
        float amplitud;
        float angulo;
        float frecuencia;
        float frecuencia_v;
        float turbulencia;
        float desplazamiento_x;
        float desplazamiento_y;
        float blendFactor;

        // utilizados en la ecuacion general del modelo
        float alpha, beta, gama;

        float color[3];//, color_g, color_b;

        // vertices en la escena
        float v1[3], v2[3], v3[3], v4[3];
        int tipo_turb;

        Textura* siguiente;

        // debug
        char* toStringBase(int);
        char* toStringCombina(int);

        Render* r;

};

#endif

