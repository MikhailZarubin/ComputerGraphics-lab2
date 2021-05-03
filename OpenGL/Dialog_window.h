#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include"View.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QRadioButton>
#include <QGroupBox>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QGroupBox;
class QLabel;
class QSpinBox;
QT_END_NAMESPACE
class QRadioButton;

class View;
class Window : public QWidget
{
    Q_OBJECT

public:
    Window(View* v, QWidget* parent = Q_NULLPTR);
    ~Window();
private:
    void createControls(const QString& title);

    View* glWidget;

    QGroupBox* controlsGroup;

    QLabel* minimumLabel;
    QLabel* maximumLabel;
    QSpinBox* minimumSpinBox;
    QSpinBox* maximumSpinBox;

    QRadioButton* radio1;
    QRadioButton* radio2;

    QLabel* modeCurrent;
    QLabel* modeLabel;

private slots:
    void handleButton();

protected slots:
    void keyPressEvent(QKeyEvent* event) override;

};

#endif
