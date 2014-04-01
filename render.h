#ifndef RENDER_H
#define RENDER_H

class Textura;

#define CANT_T_PARAMETERS 3
#define CANT_C_PARAMETERS 2
#define CANT_F_PARAMETERS 14


#define NX 256
#define NY 256

#define tamanio (NX*NY)

#include <Cg/cg.h>
#include <Cg/cgGL.h>

class Render
{
    public:
        Render(Textura*);
        //void render(int argc,char ** argv);
        void init(void);
        void paint(void);
        void debug(void);
        void resize(int, int);

        void _checkForCgError(const char *situation);

    private:
        Textura* textura;
        void setear_valores_textura(void);

        CGprogram vertex, fragment;      


};
#endif

