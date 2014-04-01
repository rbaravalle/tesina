#include<QGLWidget>
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <GL/glut.h>
#include <QtOpenGL>
#include <QTimer>
#include <QList>

#include <QKeyEvent>

#include<textura.h>

#define CANT_T_PARAMETERS 4
#define CANT_C_PARAMETERS 2
#define CANT_F_PARAMETERS 14


class Escenadrawer : public QGLWidget
{
	Q_OBJECT

	public:
        Escenadrawer(QWidget *parent, QList<Textura*> *l):QGLWidget(parent) { texs = l; };
        ~Escenadrawer() {};
        QSize minimumSizeHint() const;
        QSize sizeHint() const;

	public slots:
        void update(); // se llama cada segundo
        void keyP( int k );

    signals:
        void cambiarFPS( int );
        void cambiarEye( float,float,float );
		

	private:
        QList<Textura*> *texs;
		void initializeGL();
		void resizeGL(int width, int height);
		void paintGL();
        //void _cargar_texturas();
        void _checkForCgError(const char *);
        void setear_valores_textura(Textura*);


        QTimer *timer;

};
