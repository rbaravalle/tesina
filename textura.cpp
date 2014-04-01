#include "textura.h"

#include "string.h"

#include<iostream>
using namespace std;

// constructor
Textura::Textura() {
    intensidad = 1;
    amplitud = 1;
    angulo = 0;
    frecuencia = 20;
    frecuencia_v = 1;
    turbulencia = 0;
    desplazamiento_x = 0;
    desplazamiento_y = 0;
    blendFactor = 0.5;


    v1[0] = -1.0; v1[1] = -1.0; v1[2] = 0.0;
    v2[0] = -1.0; v2[1] =  1.0; v2[2] = 0.0;
    v3[0] =  1.0; v3[1] =  1.0; v3[2] = 0.0;
    v4[0] =  1.0; v4[1] = -1.0; v4[2] = 0.0;

    alpha = 1;
    beta = 1;
    gama = 1;

    color[0] = color[1] = color[2] = 0.5;

    tipo_turb = 0;
    base = TEX1;
    usa = 1;
    combina = ADD;
    siguiente = NULL;
    antialias = 1;
}

// setters y getters
Textura* Textura::set_intensidad(float i) {
    intensidad = i;
    return this;
}

Textura* Textura::set_amplitud(float a) {
    amplitud = a;
    return this;
}

Textura* Textura::set_turbulencia(float t) {
    turbulencia = t;
    return this;
}

Textura* Textura::set_angulo(float a) {
    angulo = a;
    return this;
}


Textura* Textura::set_frecuencia(float f) {
    frecuencia = f;
    return this;
}

Textura* Textura::set_frecuencia_v(float f) {
    frecuencia_v = f;
    return this;
}

Textura* Textura::set_desplazamiento_x(float dx) {
    desplazamiento_x = dx;
    return this;
}

Textura* Textura::set_desplazamiento_y(float dy) {
    desplazamiento_y = dy;
    return this;
}

Textura* Textura::set_base(int b) {
    base = b;
    return this;
}

Textura* Textura::set_usa(int u) {
    usa = u;
    return this;
}

Textura* Textura::set_combina(int c) {
    combina = c;
    return this;
}

Textura* Textura::set_siguiente(Textura* s) {
    siguiente = s;
    return this;
}

Textura* Textura::set_v1(float* v) {
    v1[0] = v[0];
    v1[1] = v[1];
    v1[2] = v[2];
    return this;
}

Textura* Textura::set_v2(float* v) {
    v2[0] = v[0];
    v2[1] = v[1];
    v2[2] = v[2];
    return this;
}

Textura* Textura::set_v3(float* v) {
    v3[0] = v[0];
    v3[1] = v[1];
    v3[2] = v[2];
    return this;
}

Textura* Textura::set_v4(float* v) {
    v4[0] = v[0];
    v4[1] = v[1];
    v4[2] = v[2];
    return this;
}

Textura* Textura::set_tipo_turb(int t) {
    tipo_turb = t;
    return this;
}

Textura* Textura::set_alpha(float p) {
    alpha = p;
    return this;
}

Textura* Textura::set_beta(float p) {
    beta = p;
    return this;
}

Textura* Textura::set_gama(float p) {
    gama = p;
    return this;
}

Textura* Textura::set_color_r(float r) {
    color[0] = r;
    return this;
}

Textura* Textura::set_color_g(float g) {
    color[1] = g;
    return this;
}

Textura* Textura::set_color_b(float b) {
    color[2] = b;
    return this;
}

Textura* Textura::set_antialias(int a) {
    antialias = a;
    return this;
}

int Textura::get_antialias() {
    return antialias;
}


Textura* Textura::get_siguiente() {
    return siguiente;
}

float Textura::get_amplitud() {
    return amplitud;
}

float Textura::get_turbulencia() {
    return turbulencia;
}

float Textura::get_frecuencia() {
    return frecuencia;
}

float Textura::get_frecuencia_v() {
    return frecuencia_v;
}

int Textura::get_base() {
    return base;
}

int Textura::get_usa() {
    return usa;
}

int Textura::get_combina() {
    return combina;
}

float Textura::get_intensidad() {
    return intensidad;
}

float Textura::get_angulo(void) {
    return angulo;
}

float Textura::get_desplazamiento_x(void) {
    return desplazamiento_x;
}

float Textura::get_desplazamiento_y(void) {
    return desplazamiento_y;
}


float* Textura::get_v1(void) {
    return v1;
}

float* Textura::get_v2(void) {
    return v2;
}

float* Textura::get_v3(void) {
    return v3;
}

float* Textura::get_v4(void) {
    return v4;
}

int Textura::get_tipo_turb(void) {
    return tipo_turb;
}

float Textura::get_alpha(void) {
    return alpha;
}

float Textura::get_beta(void) {
    return beta;
}

float Textura::get_gama(void) {
    return gama;
}


float Textura::get_color_r(void) {
    return color[0];
}

float Textura::get_color_g(void) {
    return color[1];
}

float Textura::get_color_b(void) {
    return color[2];
}

// funciones de combinacion
Textura* Textura::add(Textura* t) {
    
    if(siguiente == NULL) {
        combina = ADD;
        siguiente = t;
        return this;
    }
    else  {  
        return siguiente->add(t);
    }
}

Textura* Textura::sub(Textura* t) {
    
    if(siguiente == NULL) {
        combina = SUB;
        siguiente = t;
        return this;
    }
    else  {  
        return siguiente->sub(t);
    }
}

Textura* Textura::blend(Textura* t, float b) {
    
    if(b > 1 || b < 0) { cout << "El valor debe estar entre 0 y 1" << endl; }
   
    if(siguiente == NULL) {
        combina = LERP;
        blendFactor = b;
        siguiente = t;
        return this;
    }
    else  {  
        return siguiente->blend(t,b);
    }
}

// combinamos el material con una textura mas
Textura* Textura::mul(Textura* t) {
    
    if(siguiente == NULL) {
        combina = MUL;
        siguiente = t;
        return this;
    }
    else  {  
        return siguiente->mul(t);
    }
}

// combinamos el material con una textura mas
Textura* Textura::sobre(Textura* t) {
    
    if(siguiente == NULL) {
        combina = SOBRE;
        siguiente = t;
        return this;
    }
    else  {  
        return siguiente->sobre(t);
    }
}



// muestra como esta compuesto el material
void Textura::debug() {

    cout << toStringBase(base);

    char* combinacion = toStringCombina(combina);


    if(siguiente != NULL)  { 
        cout << "->" << combinacion;
        if(strcmp(combinacion,(char*)"LERP") == 0 ) { cout << "(" << blendFactor << ")"; }
        cout << "->";
        siguiente->debug();
    }

    else cout << endl;
}

// imprime los valores seteados en la textura
// para debug
void Textura::debug_props() {

    cout << "Base: " << toStringBase(base) << endl;
    cout << "Usa: " << usa << endl;
    cout << "Combina: " << combina << endl;
    cout << "Intensidad: " << intensidad << endl;
    cout << "Amplitud: " << amplitud << endl;
    cout << "Angulo: " << angulo << endl;
    cout << "Frecuencia: " << frecuencia << endl;
    cout << "Frecuencia v: " << frecuencia_v << endl;
    cout << "Turbulencia: " << turbulencia << endl;
    cout << "Desplazamiento x: " << desplazamiento_x << endl;
    cout << "Desplazamiento y: " << desplazamiento_y << endl;
    cout << "Vertice 1: " << v1[0] << "," << v1[1] << "," << v1[2] << endl;
    cout << "Vertice 2: " << v2[0] << "," << v2[1] << "," << v2[2] << endl;
    cout << "Vertice 3: " << v3[0] << "," << v3[1] << "," << v3[2] << endl;
    cout << "Vertice 4: " << v4[0] << "," << v4[1] << "," << v4[2] << endl;
    cout << "Tipo Turbulencia: " << tipo_turb << endl;
    cout << "Alpha: " << alpha << endl;
    cout << "Beta: " << beta << endl;
    cout << "Gama: " << gama << endl;
    cout << "RGB: (" << color[0] << ", " << color[1] << ", " << color[2] << ")" << endl;

}


// para mostrar en pantalla
char* Textura::toStringBase(int b) {
    switch (b)  {
        case 0 : return (char*)"Base 1";
        case 1 : return (char*)"Base 2";
        case 2 : return (char*)"Base 3";
        case 3 : return (char*)"Spot Noise 1";
        case 4 : return (char*)"Spot Noise 2";
        case 5 : return (char*)"Spot Noise 2";
        default : return (char*)"error B";
    }
}

char* Textura::toStringCombina(int c) {
    switch (c)  {
        case ADD : return (char*)"ADD";
        case SUB : return (char*)"SUB";
        case LERP : return (char*)"LERP";
        case SOBRE : return (char*)"SOBRE";
        case MUL : return (char*)"MUL";
        default : return (char*)"error C";
    }
}

void Textura::renderInit() {
    r = new Render(this);
    r->init();
//    delete r;
}

void Textura::render() {
    r = new Render(this);
    r->paint();
    delete r;
}

void Textura::resize(int w, int h) {
    r->resize(w,h);
//    delete r;
}

