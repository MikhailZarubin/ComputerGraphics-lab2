#include "Dialog_window.h"



Window::Window(View* v, QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(1200, 600);

    glWidget = v;

    createControls(tr("Controls"));

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(controlsGroup);
    layout->addWidget(glWidget);
    setLayout(layout);

    setWindowTitle(tr("Sliders"));
}
Window::~Window()
{
    delete glWidget;
}
void Window::createControls(const QString& title)
{
    controlsGroup = new QGroupBox(title);

    minimumLabel = new QLabel(tr("Minimum value:"));
    maximumLabel = new QLabel(tr("Maximum value:"));

    modeLabel = new QLabel(tr("Visualisation mode:"));
    modeCurrent = new QLabel(tr("QUADS"));

    minimumSpinBox = new QSpinBox;
    minimumSpinBox->setRange(glWidget->getMin() , glWidget->getMax());
    minimumSpinBox->setSingleStep(1);

    maximumSpinBox = new QSpinBox;
    maximumSpinBox->setRange(glWidget->getMin(), glWidget->getMax());
    maximumSpinBox->setSingleStep(1);

    radio1 = new QRadioButton(tr("X-Cut"));
    radio2 = new QRadioButton(tr("Y-Cut"));

    connect(radio1, &QRadioButton::clicked, this, &Window::handleButton);
    connect(radio2, &QRadioButton::clicked, this, &Window::handleButton);

    radio1->setChecked(true);

    connect(minimumSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
        glWidget, &View::SetMin);
    connect(minimumSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
        glWidget, &View::SetMin);
    connect(maximumSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
        glWidget, &View::SetMax);
    connect(maximumSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
        glWidget, &View::SetMax);

    QGridLayout* controlsLayout = new QGridLayout;
    controlsLayout->addWidget(minimumLabel, 0, 0);
    controlsLayout->addWidget(maximumLabel, 1, 0);

    controlsLayout->addWidget(minimumSpinBox, 0, 1);
    controlsLayout->addWidget(maximumSpinBox, 1, 1);
    controlsLayout->addWidget(radio1, 2, 0);
    controlsLayout->addWidget(radio2, 2, 1);
    controlsLayout->addWidget(modeLabel, 3, 0);
    controlsLayout->addWidget(modeCurrent, 3, 1);

    controlsGroup->setLayout(controlsLayout);
}

void Window::handleButton()
{
    if (radio1->isChecked())
    {
        qDebug() << "SET X";
        glWidget->SetX();
    }
    if (radio2->isChecked())
    {
        qDebug() << "SET Y";
        glWidget->SetY();
    }
}

void Window::keyPressEvent(QKeyEvent* event)
{
    unsigned int key_pressed = event->nativeVirtualKey();
    qDebug() << "PRESSED";

    if (key_pressed == Qt::Key_W)
    {
        glWidget->PressW();
    }
    else if (key_pressed == Qt::Key_S)
    {
        glWidget->PressS();
    }
    else if (key_pressed == Qt::Key_N)
    {
        glWidget->PressN();

        qDebug() << "STATE: " << glWidget->getState();

        switch (glWidget->getState())
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