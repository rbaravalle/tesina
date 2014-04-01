#include <preview.h>

#include <QGLWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QGroupBox>
#include <QComboBox>
#include <QTimer>
#include <QLineEdit>

void Preview::guardar() {
    emit end_preview();
    this->close();
}

Preview::Preview(QWidget* p, Textura* t)
{
    parent = p;
    textura = t;
    previewdrawer = new Previewdrawer(this, t);

    Textura* texs[CANT];
    texs[0] = textura;
    for(int i = 1; i < CANT; i++) {
        texs[i] = texs[i-1]->get_siguiente();
    }

    QPushButton* guardarButton = new QPushButton(tr("&Guardar"));
    QLineEdit* lineedit[4][3];
    
    // vertices en la pantalla
    for (int i = 0; i < 4; i++) {
        for(int j = 0; j < 3; j++) {
            lineedit[i][j] = new QLineEdit;
            lineedit[i][j]->setMaxLength(6);
        }    
    }


    QSlider *amplitudSlider[CANT];
    QSlider *turbSlider[CANT];
    QSlider *anguloSlider[CANT];
    QSlider *frecuenciaUSlider[CANT];
    QSlider *frecuenciaVSlider[CANT];
    QSlider *intensidadSlider[CANT];
    QSlider *desplUSlider[CANT];
    QSlider *desplVSlider[CANT];

    QSlider *colorSlider[4];

    QPushButton *usaButtons[CANT];
    QComboBox *baseButtons[CANT];
    QPushButton *antialias;

    // tipo de turbulencia
    QComboBox *turbButtons[CANT];

    QComboBox *combinaButtons[CANT-1];

    QSlider *alphaSlider[CANT];
    QSlider *betaSlider[CANT];
    QSlider *gamaSlider[CANT];


    usaButtons[0] = new QPushButton(tr("Textura &1"));
    usaButtons[1] = new QPushButton(tr("Textura &2"));
    usaButtons[2] = new QPushButton(tr("Textura &3"));

    antialias = new QPushButton(tr("Antialiasing"));
    antialias->setCheckable(true);
    antialias->setChecked(!texs[0]->get_antialias());

    for(int i = 0; i < CANT; i++) {

        turbSlider[i] = new QSlider ();
        turbSlider[i]->setPageStep(1);
        turbSlider[i]->setRange(0,200);
        turbSlider[i]->setValue(texs[i]->get_turbulencia()*100);
        turbSlider[i]->setOrientation(Qt::Horizontal);

        amplitudSlider[i] = new QSlider ();
        amplitudSlider[i]->setPageStep(1);
        amplitudSlider[i]->setRange(0,2000);
        amplitudSlider[i]->setValue(texs[i]->get_amplitud()*100);
        amplitudSlider[i]->setOrientation(Qt::Horizontal);

        anguloSlider[i] = new QSlider ();
        anguloSlider[i]->setPageStep(1);
        anguloSlider[i]->setRange(0,314);
        anguloSlider[i]->setValue(texs[i]->get_angulo()*100);
        anguloSlider[i]->setOrientation(Qt::Horizontal);

        frecuenciaUSlider[i] = new QSlider ();
        frecuenciaUSlider[i]->setPageStep(1);
        frecuenciaUSlider[i]->setRange(0,1000);
        frecuenciaUSlider[i]->setValue(texs[i]->get_frecuencia());
        frecuenciaUSlider[i]->setOrientation(Qt::Horizontal);

        frecuenciaVSlider[i] = new QSlider ();
        frecuenciaVSlider[i]->setPageStep(1);
        frecuenciaVSlider[i]->setRange(0,1000);
        frecuenciaVSlider[i]->setValue(texs[i]->get_frecuencia_v());
        frecuenciaVSlider[i]->setOrientation(Qt::Horizontal);

        intensidadSlider[i] = new QSlider ();
        intensidadSlider[i]->setPageStep(1);
        intensidadSlider[i]->setRange(0,800);
        intensidadSlider[i]->setValue(texs[i]->get_intensidad()*100);
        intensidadSlider[i]->setOrientation(Qt::Horizontal);

        desplUSlider[i] = new QSlider ();
        desplUSlider[i]->setPageStep(1);
        desplUSlider[i]->setRange(0,800);
        desplUSlider[i]->setValue(texs[i]->get_desplazamiento_x()*100);
        desplUSlider[i]->setOrientation(Qt::Horizontal);

        desplVSlider[i] = new QSlider ();
        desplVSlider[i]->setPageStep(1);
        desplVSlider[i]->setRange(0,800);
        desplVSlider[i]->setValue(texs[i]->get_desplazamiento_y()*100);
        desplVSlider[i]->setOrientation(Qt::Horizontal);

        alphaSlider[i] = new QSlider ();
        alphaSlider[i]->setPageStep(1);
        alphaSlider[i]->setRange(0,800);
        alphaSlider[i]->setValue(texs[i]->get_alpha()*100);
        alphaSlider[i]->setOrientation(Qt::Horizontal);

        betaSlider[i] = new QSlider ();
        betaSlider[i]->setPageStep(1);
        betaSlider[i]->setRange(0,800);
        betaSlider[i]->setValue(texs[i]->get_beta()*100);
        betaSlider[i]->setOrientation(Qt::Horizontal);

        gamaSlider[i] = new QSlider ();
        gamaSlider[i]->setPageStep(1);
        gamaSlider[i]->setRange(0,3200);
        gamaSlider[i]->setValue(texs[i]->get_gama()*100);
        gamaSlider[i]->setOrientation(Qt::Horizontal);

        usaButtons[i]->setCheckable(true);
        usaButtons[i]->setChecked(!texs[i]->get_usa());

        baseButtons[i] = new QComboBox();
        baseButtons[i]->insertItem(0,"Base 1");
        baseButtons[i]->insertItem(1,"Base 2");
        baseButtons[i]->insertItem(2,"Base 3");
        baseButtons[i]->insertItem(3,"Spot Noise 1");
        baseButtons[i]->insertItem(4,"Spot Noise 2");
        baseButtons[i]->insertItem(5,"Spot Noise 3");
        baseButtons[i]->setCurrentIndex(texs[i]->get_base());

        turbButtons[i] = new QComboBox();
        turbButtons[i]->insertItem(0,"Turb Noise 1");
        turbButtons[i]->insertItem(1,"Turb Noise 2");
        turbButtons[i]->insertItem(2,"Turb Noise 3");
        turbButtons[i]->setCurrentIndex(texs[i]->get_tipo_turb());
        
    }

    for(int i = 0; i < 3; i++) {
        colorSlider[i] = new QSlider;
        colorSlider[i]->setPageStep(1);
        colorSlider[i]->setRange(0,255);
        colorSlider[i]->setOrientation(Qt::Horizontal);
    }

    colorSlider[0]->setValue(texs[0]->get_color_r()*255);
    colorSlider[1]->setValue(texs[0]->get_color_g()*255);
    colorSlider[2]->setValue(texs[0]->get_color_b()*255);

    QString* temp = new QString;

    lineedit[0][0]->setText(temp->setNum(texs[0]->get_v1()[0]));
    lineedit[0][1]->setText(temp->setNum(texs[0]->get_v1()[1]));
    lineedit[0][2]->setText(temp->setNum(texs[0]->get_v1()[2]));

    lineedit[1][0]->setText(temp->setNum(texs[0]->get_v2()[0]));
    lineedit[1][1]->setText(temp->setNum(texs[0]->get_v2()[1]));
    lineedit[1][2]->setText(temp->setNum(texs[0]->get_v2()[2]));

    lineedit[2][0]->setText(temp->setNum(texs[0]->get_v3()[0]));
    lineedit[2][1]->setText(temp->setNum(texs[0]->get_v3()[1]));
    lineedit[2][2]->setText(temp->setNum(texs[0]->get_v3()[2]));

    lineedit[3][0]->setText(temp->setNum(texs[0]->get_v4()[0]));
    lineedit[3][1]->setText(temp->setNum(texs[0]->get_v4()[1]));
    lineedit[3][2]->setText(temp->setNum(texs[0]->get_v4()[2]));

    delete temp;
    
    for(int i = 0; i < CANT-1; i++) {
        combinaButtons[i] = new QComboBox();
        combinaButtons[i]->insertItem(0,"ADD");
        combinaButtons[i]->insertItem(1,"SUB");
        combinaButtons[i]->insertItem(2,"LERP");
        combinaButtons[i]->insertItem(3,"SOBRE");
        combinaButtons[i]->insertItem(4,"MUL");
        combinaButtons[i]->setCurrentIndex(texs[i]->get_combina());
    }

    QObject::connect(guardarButton, SIGNAL(clicked()),
            this, SLOT(guardar()));

    QObject::connect(antialias, SIGNAL(clicked()),
            previewdrawer, SLOT(cambiarAntialias()));

    QObject::connect(amplitudSlider[0], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarAmplitud1(int)));

    QObject::connect(amplitudSlider[1], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarAmplitud2(int)));

    QObject::connect(amplitudSlider[2], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarAmplitud3(int)));

    QObject::connect(turbSlider[0], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarTurb1(int)));

    QObject::connect(turbSlider[1], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarTurb2(int)));

    QObject::connect(turbSlider[2], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarTurb3(int)));

    QObject::connect(anguloSlider[0], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarAngulo1(int)));

    QObject::connect(anguloSlider[1], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarAngulo2(int)));

    QObject::connect(anguloSlider[2], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarAngulo3(int)));

    QObject::connect(frecuenciaUSlider[0], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarFrecuencia1(int)));

    QObject::connect(frecuenciaUSlider[1], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarFrecuencia2(int)));

    QObject::connect(frecuenciaUSlider[2], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarFrecuencia3(int)));

    QObject::connect(frecuenciaVSlider[0], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarFrecuenciaV1(int)));

    QObject::connect(frecuenciaVSlider[1], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarFrecuenciaV2(int)));

    QObject::connect(frecuenciaVSlider[2], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarFrecuenciaV3(int)));

    QObject::connect( usaButtons[0], SIGNAL(clicked()),
            previewdrawer, SLOT(cambiarUsa1()));

    QObject::connect( usaButtons[1], SIGNAL(clicked()),
            previewdrawer, SLOT(cambiarUsa2()));

    QObject::connect( usaButtons[2], SIGNAL(clicked()),
            previewdrawer, SLOT(cambiarUsa3()));

    QObject::connect(intensidadSlider[0], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarIntensidad1(int)));

    QObject::connect(intensidadSlider[1], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarIntensidad2(int)));

    QObject::connect(intensidadSlider[2], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarIntensidad3(int)));

    QObject::connect(desplUSlider[0], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarDesplazamiento_u1(int)));

    QObject::connect(desplUSlider[1], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarDesplazamiento_u2(int)));

    QObject::connect(desplUSlider[2], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarDesplazamiento_u3(int)));

    QObject::connect(desplVSlider[0], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarDesplazamiento_v1(int)));

    QObject::connect(desplVSlider[1], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarDesplazamiento_v2(int)));

    QObject::connect(desplVSlider[2], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarDesplazamiento_v3(int)));

    QObject::connect(combinaButtons[0], SIGNAL(activated(int)),
            previewdrawer, SLOT(cambiarCombina1(int)));

    QObject::connect(combinaButtons[1], SIGNAL(activated(int)),
            previewdrawer, SLOT(cambiarCombina2(int)));

    QObject::connect(baseButtons[0], SIGNAL(activated(int)),
            previewdrawer, SLOT(cambiarBase1(int)));

    QObject::connect(baseButtons[1], SIGNAL(activated(int)),
            previewdrawer, SLOT(cambiarBase2(int)));

    QObject::connect(baseButtons[2], SIGNAL(activated(int)),
            previewdrawer, SLOT(cambiarBase3(int)));

    QObject::connect(turbButtons[0], SIGNAL(activated(int)),
            previewdrawer, SLOT(cambiarTipoTurb1(int)));

    QObject::connect(turbButtons[1], SIGNAL(activated(int)),
            previewdrawer, SLOT(cambiarTipoTurb2(int)));

    QObject::connect(turbButtons[2], SIGNAL(activated(int)),
            previewdrawer, SLOT(cambiarTipoTurb3(int)));

    QObject::connect(lineedit[0][0], SIGNAL(textChanged(const QString&)),
            previewdrawer, SLOT(cambiarVertice11(QString)));

    QObject::connect(lineedit[0][1], SIGNAL(textChanged(const QString&)),
            previewdrawer, SLOT(cambiarVertice12(QString)));

    QObject::connect(lineedit[0][2], SIGNAL(textChanged(const QString&)),
            previewdrawer, SLOT(cambiarVertice13(QString)));

    QObject::connect(lineedit[1][0], SIGNAL(textChanged(const QString&)),
            previewdrawer, SLOT(cambiarVertice21(QString)));

    QObject::connect(lineedit[1][1], SIGNAL(textChanged(const QString&)),
            previewdrawer, SLOT(cambiarVertice22(QString)));

    QObject::connect(lineedit[1][2], SIGNAL(textChanged(const QString&)),
            previewdrawer, SLOT(cambiarVertice23(QString)));

    QObject::connect(lineedit[2][0], SIGNAL(textChanged(const QString&)),
            previewdrawer, SLOT(cambiarVertice31(QString)));

    QObject::connect(lineedit[2][1], SIGNAL(textChanged(const QString&)),
            previewdrawer, SLOT(cambiarVertice32(QString)));

    QObject::connect(lineedit[2][2], SIGNAL(textChanged(const QString&)),
            previewdrawer, SLOT(cambiarVertice33(QString)));

    QObject::connect(lineedit[3][0], SIGNAL(textChanged(const QString&)),
            previewdrawer, SLOT(cambiarVertice41(QString)));

    QObject::connect(lineedit[3][1], SIGNAL(textChanged(const QString&)),
            previewdrawer, SLOT(cambiarVertice42(QString)));

    QObject::connect(lineedit[3][2], SIGNAL(textChanged(const QString&)),
            previewdrawer, SLOT(cambiarVertice43(QString)));

    QObject::connect(alphaSlider[0], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarAlpha1(int)));

    QObject::connect(alphaSlider[1], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarAlpha2(int)));

    QObject::connect(alphaSlider[2], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarAlpha3(int)));

    QObject::connect(betaSlider[0], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarBeta1(int)));

    QObject::connect(betaSlider[1], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarBeta2(int)));

    QObject::connect(betaSlider[2], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarBeta3(int)));

    QObject::connect(gamaSlider[0], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarGama1(int)));

    QObject::connect(gamaSlider[1], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarGama2(int)));

    QObject::connect(gamaSlider[2], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarGama3(int)));

    QObject::connect(colorSlider[0], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarColorR(int)));

    QObject::connect(colorSlider[1], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarColorG(int)));

    QObject::connect(colorSlider[2], SIGNAL(valueChanged(int)),
            previewdrawer, SLOT(cambiarColorB(int)));

    QVBoxLayout *texturaLayout[CANT];
    QGroupBox *texturaGroup[CANT];

    for(int j = 0; j < CANT; j++) {

        int i = 0;

        texturaLayout[j] = new QVBoxLayout;
	
        texturaLayout[j]->addWidget(usaButtons[j],i++);
        texturaLayout[j]->addWidget(baseButtons[j],i++);
        texturaLayout[j]->addWidget(turbButtons[j],i++);
        texturaLayout[j]->addWidget(new QLabel(tr("Amplitud")),i++);
        texturaLayout[j]->addWidget(amplitudSlider[j],i++);
        texturaLayout[j]->addWidget(new QLabel(tr("Intensidad")),i++);
        texturaLayout[j]->addWidget(intensidadSlider[j],i++);
        texturaLayout[j]->addWidget(new QLabel(tr("Turbulencia")),i++);
        texturaLayout[j]->addWidget(turbSlider[j],i++);
        texturaLayout[j]->addWidget(new QLabel(tr("Angulo")),i++);
        texturaLayout[j]->addWidget(anguloSlider[j],i++);
        texturaLayout[j]->addWidget(new QLabel(tr("Frecuencia u")),i++);
        texturaLayout[j]->addWidget(frecuenciaUSlider[j],i++);
        texturaLayout[j]->addWidget(new QLabel(tr("Frecuencia v")),i++);
        texturaLayout[j]->addWidget(frecuenciaVSlider[j],i++);
        texturaLayout[j]->addWidget(new QLabel(tr("Desplazamiento u")),i++);
        texturaLayout[j]->addWidget(desplUSlider[j],i++);
        texturaLayout[j]->addWidget(new QLabel(tr("Desplazamiento v")),i++);
        texturaLayout[j]->addWidget(desplVSlider[j],i++);
        texturaLayout[j]->addWidget(new QLabel(tr("Alfa")),i++);
        texturaLayout[j]->addWidget(alphaSlider[j],i++);
        texturaLayout[j]->addWidget(new QLabel(tr("Beta")),i++);
        texturaLayout[j]->addWidget(betaSlider[j],i++);
        texturaLayout[j]->addWidget(new QLabel(tr("Gama")),i++);
        texturaLayout[j]->addWidget(gamaSlider[j],i++);
            
        texturaLayout[j]->addStretch(200);

        texturaGroup[j] = new QGroupBox;
	    texturaGroup[j]->setLayout(texturaLayout[j]);

	    texturaGroup[j]->setTitle(tr("Textura"));
    }


    QGridLayout *vertGrid = new QGridLayout;
    
    QLabel* verticesLabel = new QLabel(tr("Vertices"));
    QLabel* x = new QLabel(tr("X"));
    QLabel* y = new QLabel(tr("Y"));
    QLabel* z = new QLabel(tr("Z"));

	vertGrid->addWidget(x,0,0);
    vertGrid->addWidget(y,0,1);
    vertGrid->addWidget(z,0,2);

    for(int i = 0 ; i < 4; i++) {
        vertGrid->addWidget(lineedit[i][0],i+1,0);
        vertGrid->addWidget(lineedit[i][1],i+1,1);
        vertGrid->addWidget(lineedit[i][2],i+1,2);
    }

    QLabel* colorLabel[3];
    colorLabel[0] = new QLabel(tr("R"));
    colorLabel[1] = new QLabel(tr("G"));
    colorLabel[2] = new QLabel(tr("B"));

    int i = 0;

    QVBoxLayout *colorLayout = new QVBoxLayout;

    colorLayout->addWidget(colorLabel[0], i++);
    colorLayout->addWidget(colorSlider[0], i++);
    colorLayout->addWidget(colorLabel[1], i++);
    colorLayout->addWidget(colorSlider[1], i++);
    colorLayout->addWidget(colorLabel[2], i++);
    colorLayout->addWidget(colorSlider[2], i++);
    
	i = 0;
    QVBoxLayout *generalesLayout = new QVBoxLayout;
    generalesLayout->addLayout(colorLayout, i++);
    generalesLayout->addWidget(verticesLabel, i++);
    generalesLayout->addLayout(vertGrid, i++);

    for (int j = 0; j < CANT-1; j++) {
        generalesLayout->addWidget(new QLabel(tr("Combinacion")),i++);
        generalesLayout->addWidget(combinaButtons[j],i++);
    }

    generalesLayout->addWidget(antialias,i++,0);
    generalesLayout->addWidget(guardarButton,i++,0);
    generalesLayout->addStretch(100);

    QGroupBox *generalesGroup = new QGroupBox;
    generalesGroup->setLayout(generalesLayout);
    generalesGroup->setTitle(tr("Generales"));

    QGridLayout *mainGrid = new QGridLayout;
    mainGrid->addWidget(texturaGroup[0],0,0);
    mainGrid->addWidget(texturaGroup[1],0,1);
    mainGrid->addWidget(texturaGroup[2],0,2);
    mainGrid->addWidget(generalesGroup,0,3);
    mainGrid->addWidget(previewdrawer,0,4);

    // no minimizar la ventana al empezar el programa
    mainGrid->setSizeConstraint(QLayout::SetMinimumSize);
    setLayout(mainGrid);

    setWindowTitle(tr("Parametros de la Textura"));
}

