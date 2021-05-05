#include"Dialog_window.h"


Window::Window(QWidget* parent, const char* input)
    : QWidget(parent)
{
    glWidget = new View;
    layout = new QHBoxLayout;
    controlsGroup = new QGroupBox;
    documentLabel = new QLabel;
    documentName = new QLabel;
    minimumLabel = new QLabel;
    maximumLabel = new QLabel;
    modeLabel = new QLabel;
    modeCurrent = new QLabel;
    minimumSpinBox = new QSpinBox;
    maximumSpinBox = new QSpinBox;
    radio1 = new QRadioButton;
    radio2 = new QRadioButton;
    radio3 = new QRadioButton;
    dimLabel1 = new QLabel;
    dimLabel2 = new QLabel;
    dimLabel3 = new QLabel;
    dimWidth = new QLabel;
    dimHeight = new QLabel;
    dimDepth = new QLabel;
    layerLabel = new QLabel;
    layerCurrent = new QLabel;
    controlsLayout = new QGridLayout;

    setMinimumSize(WIDTH, HEIGHT);

    QSurfaceFormat format;
    QSurfaceFormat::setDefaultFormat(format);
    glWidget->setFormat(format);

    glWidget->LoadData(input);
    document = input;

    createControls(tr("Controls"));

    layout->addWidget(controlsGroup);
    layout->addWidget(glWidget);
    setLayout(layout);

    setWindowTitle(tr("TomogramVisualization"));
}

void Window::createControls(const QString& title)
{
    controlsGroup->setTitle(title);

    documentLabel->setText(tr("Document name:"));
    documentName->setText(tr(document));

    minimumLabel->setText(tr("Minimum value:"));
    maximumLabel->setText(tr("Maximum value:"));

    modeLabel->setText(tr("Visualisation mode:"));
    modeCurrent->setText(tr("QUADS"));

    minimumSpinBox->setRange(glWidget->GetMin(), glWidget->GetMax());
    minimumSpinBox->setSingleStep(1);
    minimumSpinBox->setValue(glWidget->GetMin());

    maximumSpinBox->setRange(glWidget->GetMin(), glWidget->GetMax());
    maximumSpinBox->setSingleStep(1);
    maximumSpinBox->setValue(glWidget->GetMax());

    radio1->setText(tr("surface XY"));
    radio2->setText(tr("surface YZ"));
    radio3->setText(tr("surface XZ"));

    connect(radio1, &QRadioButton::clicked, this, &Window::handleButton);
    connect(radio2, &QRadioButton::clicked, this, &Window::handleButton);
    connect(radio3, &QRadioButton::clicked, this, &Window::handleButton);

    radio1->setChecked(true);

    dimLabel1->setText(tr("Width:"));
    dimLabel2->setText(tr("Height:"));
    dimLabel3->setText(tr("Depth:"));
    dimWidth->setText(tr(std::to_string(glWidget->GetDataWidth()).c_str()));
    dimHeight->setText(tr(std::to_string(glWidget->GetDataHeight()).c_str()));
    dimDepth->setText(tr(std::to_string(glWidget->GetDataDepth()).c_str()));

    layerLabel->setText(tr("Current layer:"));
    layerCurrent->setText(tr(std::to_string(glWidget->GetLayer()).c_str()));


    connect(minimumSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
        glWidget, &View::SetMin);
    connect(maximumSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
        glWidget, &View::SetMax);


    controlsLayout->addWidget(documentLabel, 0, 0);
    controlsLayout->addWidget(documentName, 0, 1);

    controlsLayout->addWidget(minimumLabel, 1, 0);
    controlsLayout->addWidget(minimumSpinBox, 1, 1);

    controlsLayout->addWidget(maximumLabel, 2, 0);
    controlsLayout->addWidget(maximumSpinBox, 2, 1);

    controlsLayout->addWidget(radio1, 3, 0);
    controlsLayout->addWidget(radio2, 3, 1);
    controlsLayout->addWidget(radio3, 3, 2);

    controlsLayout->addWidget(modeLabel, 4, 0);
    controlsLayout->addWidget(modeCurrent, 4, 1);

    controlsLayout->addWidget(dimLabel1, 5, 0);
    controlsLayout->addWidget(dimWidth, 5, 1);

    controlsLayout->addWidget(dimLabel2, 6, 0);
    controlsLayout->addWidget(dimHeight, 6, 1);

    controlsLayout->addWidget(dimLabel3, 7, 0);
    controlsLayout->addWidget(dimDepth, 7, 1);

    controlsLayout->addWidget(layerLabel, 8, 0);
    controlsLayout->addWidget(layerCurrent, 8, 1);

    controlsGroup->setLayout(controlsLayout);
}

void Window::handleButton()
{
    if (radio1->isChecked())
    {
        qDebug() << "SET XY";
        glWidget->SetXY();
        layerCurrent->setText(std::to_string(glWidget->GetLayer()).c_str());
    }
    if (radio2->isChecked())
    {
        qDebug() << "SET YZ";
        glWidget->SetYZ();
        layerCurrent->setText(std::to_string(glWidget->GetLayer()).c_str());
    }
    if(radio3->isChecked())
    {
        qDebug() << "SET XZ";
        glWidget->SetXZ();
        layerCurrent->setText(std::to_string(glWidget->GetLayer()).c_str());
    }
}

void Window::keyPressEvent(QKeyEvent* event)
{
    unsigned int key_pressed = event->nativeVirtualKey();
    qDebug() << "PRESSED";

    if (key_pressed == Qt::Key_W)
    {
        glWidget->PressW();
        layerCurrent->setText(std::to_string(glWidget->GetLayer()).c_str());
    }
    else if (key_pressed == Qt::Key_S)
    {
        glWidget->PressS();
        layerCurrent->setText(std::to_string(glWidget->GetLayer()).c_str());
    }
    else if (key_pressed == Qt::Key_N)
    {
        glWidget->PressN();

        qDebug() << "STATE: " << glWidget->GetState();

        switch (glWidget->GetState())
        {
        case 0:
            modeCurrent->setText("QUADS");
            break;
        case 1:
            modeCurrent->setText("QUAD_STRIP");
            break;
        case 2:
            modeCurrent->setText("TEXTURE");
            break;
        }
    }
}