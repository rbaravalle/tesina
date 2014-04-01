#include<QGLWidget>
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include <textura.h>

#define CANT_T_PARAMETERS 4
#define CANT_C_PARAMETERS 2
#define CANT_F_PARAMETERS 14


class Previewdrawer : public QGLWidget
{
	Q_OBJECT

	public:
        Previewdrawer(QWidget *p, Textura* t):QGLWidget(p) {
            parent = p;
            textura = t;
            sig = t->get_siguiente();
            if(sig)
                sigsig = sig->get_siguiente();
            setear_valores();
        };
        ~Previewdrawer() {};
        QSize minimumSizeHint() const;
        QSize sizeHint() const;

	public slots:

        void setear_valores(void);

        void cambiarAmplitud1(int);
        void cambiarAmplitud2(int);
        void cambiarAmplitud3(int);

        void cambiarTurb1(int);
        void cambiarTurb2(int);
        void cambiarTurb3(int);

        void cambiarAngulo1(int);
        void cambiarAngulo2(int);
        void cambiarAngulo3(int);

        void cambiarFrecuencia1(int);
        void cambiarFrecuencia2(int);
        void cambiarFrecuencia3(int);

        void cambiarUsa1();
        void cambiarUsa2();
        void cambiarUsa3();

        void cambiarIntensidad1(int);
        void cambiarIntensidad2(int);
        void cambiarIntensidad3(int);

        void cambiarDesplazamiento_u1(int);
        void cambiarDesplazamiento_u2(int);
        void cambiarDesplazamiento_u3(int);

        void cambiarDesplazamiento_v1(int);
        void cambiarDesplazamiento_v2(int);
        void cambiarDesplazamiento_v3(int);

        void cambiarCombina1(int);
        void cambiarCombina2(int);

        void cambiarBase1(int);
        void cambiarBase2(int);
        void cambiarBase3(int);

        void cambiarVertice11(QString);
        void cambiarVertice12(QString);
        void cambiarVertice13(QString);

        void cambiarVertice21(QString);
        void cambiarVertice22(QString);
        void cambiarVertice23(QString);

        void cambiarVertice31(QString);
        void cambiarVertice32(QString);
        void cambiarVertice33(QString);

        void cambiarVertice41(QString);
        void cambiarVertice42(QString);
        void cambiarVertice43(QString);

        void cambiarTipoTurb1(int);
        void cambiarTipoTurb2(int);
        void cambiarTipoTurb3(int);

        void cambiarFrecuenciaV1(int);
        void cambiarFrecuenciaV2(int);
        void cambiarFrecuenciaV3(int);

        void cambiarAlpha1(int);
        void cambiarAlpha2(int);
        void cambiarAlpha3(int);

        void cambiarBeta1(int);
        void cambiarBeta2(int);
        void cambiarBeta3(int);

        void cambiarGama1(int);
        void cambiarGama2(int);
        void cambiarGama3(int);

        void cambiarColorR(int);
        void cambiarColorG(int);
        void cambiarColorB(int);

        void cambiarAntialias(void);

	private:
        QWidget* parent;
        Textura *textura, *sig, *sigsig;

		void initializeGL();
		void resizeGL(int width, int height);
		void paintGL();
		void keyboard(unsigned char c, int x, int y);
        void _cargar_texturas();
        void _checkForCgError(const char *);

};
