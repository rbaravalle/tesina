#include <QWidget>
#include <QGLWidget>
#include<QMenuBar>
#include<preview.h>
#include<escenadrawer.h>

#include <QTimer>


class Escena : public QWidget
    {
        Q_OBJECT

    public:
        Escena();

    public slots:
        void agregar_textura();
        void modificar_textura();
        void eliminar_textura();

        // llamado por el modulo preview
        void actualizar();
        void cambiarFPS( int );
        void cambiarEye( float,float,float );

    private:	
        Preview* preview;
        Escenadrawer *escenadrawer;
        QMenuBar* menu;

        QComboBox *texsCombo;

        // Listas de texturas para cada material
        QList<Textura *> tex1;
        QList<Textura *> tex2;
        QList<Textura *> tex3;

        int actual;

        QTimer* timer;
        QLabel *tiempoLabel;
        QLabel *eyePosLabel;
        QString *tiempoStr;
        QString *fpsStr;

        QString *eyePosAuxStr;
        QString *eyeXPosStr;
        QString *eyeYPosStr;
        QString *eyeZPosStr;

        void keyPressEvent( QKeyEvent *e );

};
