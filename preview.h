#include <QWidget>
#include <QGLWidget>
#include <QLineEdit>
#include <QLabel>
#include <QTimer>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>

#include <previewdrawer.h>

// cantidad de texturas en la base
#define CANT 3

#include<iostream>
using namespace std;

class Preview : public QWidget
{
        Q_OBJECT

    public:
        Preview(QWidget *p, Textura*);
        ~Preview() {};

    public slots:
        void guardar();

    signals:
        void end_preview(void);

    private:
        QWidget* parent;
        Previewdrawer *previewdrawer;
        Textura* textura;

};
