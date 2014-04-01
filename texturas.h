// repositorio de texturas
// es utilizado por el modulo escena para mostrar las texturas generadas

#include<textura.h>

class Texturas
{
    public:
        Texturas();

        //void debug(void);
    
        // Devuelve un array de punteros a textura que utiliza preview
        Textura** add_textura();

        // Devuelve un array de punteros a textura que utiliza escena
        Textura** get_texturas();

        // Devuelve la textura i en el array de texturas de la escena
        Textura* get_textura(int);

    private:
        Textura** texs;
        Textura* textura;
};
