#include<QGLWidget>
#include<QVBoxLayout>
#include<QMenuBar>
#include<QMenu>
#include<QAction>
#include<escena.h>

#include<QKeyEvent>

#include<iostream>
using namespace std;

void Escena::cambiarFPS( int fps )
{
    tiempoStr->clear();
    tiempoStr->append("FPS: ");
    tiempoLabel->setText( tiempoStr->append(fpsStr->setNum(fps)));
}

void Escena::cambiarEye( float x, float y, float z )
{
    eyePosAuxStr->clear();
    eyePosAuxStr->append("Posicion: x: ");
    eyePosLabel->setText( eyePosAuxStr->append(eyeXPosStr->setNum(x,'f',1))
                                      .append(" y: ")
                                      .append(eyeYPosStr->setNum(y,'f',1))
                                      .append(" z: ")
                                      .append(eyeZPosStr->setNum(z,'f',1)));
}

void Escena::actualizar() {
    escenadrawer->updateGL();
//    cout << "UNO: " << endl; tex1.at(0)->debug_props();
/*    cout << "DOS: " << endl; tex2.at(0)->debug_props();
    cout << "TRES: " << endl; tex3.at(0)->debug_props();*/
}

void Escena::agregar_textura()
{
    // tope
    if(tex1.count() < 10) {
        Textura *t1, *t2, *t3;
        t1 = new Textura;
        t2 = new Textura;
        t3 = new Textura;
        tex1.insert(actual,t1);
        tex2.insert(actual,t2);
        tex3.insert(actual,t3);
        t1->set_siguiente(t2);
        t2->set_siguiente(t3);

        if(preview != NULL) delete preview;
        preview = new Preview(this, t1);
        preview->show();
        QObject::connect(preview, SIGNAL(end_preview()), this, SLOT(actualizar()));

        QString str = "";
        texsCombo->insertItem(actual,str.setNum(actual).prepend("Textura "));
        actual++;
    }
    actualizar();
}

void Escena::modificar_textura()
{
    int i = texsCombo->currentIndex();

    //cout << "ACTUAL:! " << i << endl;

    if(tex1.count() > 0) {
        if(preview != NULL) delete preview;
        preview = new Preview(this, tex1.at(i));
        preview->show();
        QObject::connect(preview, SIGNAL(end_preview()), this, SLOT(actualizar()));
    }
}

void Escena::eliminar_textura()
{
    int i = texsCombo->currentIndex();

    if(i >= 0) {
        tex1.removeAt(i);
        tex2.removeAt(i);
        tex3.removeAt(i);
        texsCombo->removeItem(i);
    }
    actualizar();
}

Escena::Escena()
{
    actual = 0;
    preview = NULL;
    texsCombo = new QComboBox;

    tex1.insert(0, new Textura);
    tex2.insert(0, new Textura);
    tex3.insert(0, new Textura);
    tex1.at(0)->set_siguiente(tex2.at(0));
    tex2.at(0)->set_siguiente(tex3.at(0));

    QString str = "";
    texsCombo->insertItem(actual,str.setNum(actual).prepend("Textura "));
    actual++;

    escenadrawer = new Escenadrawer(this, &tex1);

    QPushButton* botonA = new QPushButton(tr("&Agregar Textura"));
    QPushButton* botonM = new QPushButton(tr("&Modificar Textura"));
    QPushButton* botonE = new QPushButton(tr("&Eliminar Textura"));

    connect(botonA, SIGNAL(clicked()), this, SLOT(agregar_textura()));
    connect(botonM, SIGNAL(clicked()), this, SLOT(modificar_textura()));
    connect(botonE, SIGNAL(clicked()), this, SLOT(eliminar_textura()));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), escenadrawer, SLOT(update()));
    timer->start(1000);

    connect(escenadrawer, SIGNAL(cambiarFPS(int)), this, SLOT(cambiarFPS(int)));
    connect(escenadrawer, SIGNAL(cambiarEye(float,float,float)), this, SLOT(cambiarEye(float,float,float)));

    tiempoLabel = new QLabel;
    eyePosLabel = new QLabel;

    QFont f("Helvetica", 14, QFont::Bold);
    tiempoLabel->setFont(f);
    eyePosLabel->setFont(f);

    tiempoStr = new QString;
    fpsStr = new QString;
    eyePosAuxStr = new QString;
    eyeXPosStr = new QString;
    eyeYPosStr = new QString;
    eyeZPosStr = new QString;

    QGridLayout *mainLayout = new QGridLayout;
    QGridLayout *botones = new QGridLayout;

    botones->addWidget(botonA,0,0);
    botones->addWidget(botonM,0,1);
    botones->addWidget(botonE,0,2);
    botones->addWidget(texsCombo,0,3);
    botones->addWidget(tiempoLabel,0,4);
    botones->addWidget(eyePosLabel,0,5);
    

    setLayout(mainLayout);


    //mainLayout->addWidget(menu,0);
    mainLayout->addLayout(botones,0,0);
    mainLayout->addWidget(escenadrawer,1,0);

    // para que no minimice la ventana al empezar el programa
    mainLayout->setSizeConstraint(QLayout::SetMinimumSize);

    setWindowTitle(tr("Framework de materiales en GPU - Junio 2010 - Rodrigo Baravalle"));
}
void Escena::keyPressEvent( QKeyEvent *e ) {
    escenadrawer->keyP(e->key());
}

