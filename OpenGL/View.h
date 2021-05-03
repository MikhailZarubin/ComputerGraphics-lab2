#pragma once
#pragma comment (lib, "opengl32.lib")

#include"Data.h"
#include<iostream>
#include <QDebug>
#include <QKeyEvent>
#include <QtWidgets/QOpenGLWidget>
#include <QtOpenGL/QGLFunctions>

class View : public QOpenGLWidget
{
    Q_OBJECT
private:
    Data data;
    enum { VISUALIZATION_QUADS, VISUALIZATION_QUADSTRIP, VISUALIZATION_TEXTURE } visualization_state;
    enum { x, y } cut;
    int layer;
    GLuint VBOtexture;
    QImage textureImage;
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void VisualisationQuads();
    void VisualisationQuadStrip();
    void VisualisationTexture();
    float TransferFunction(short value);
    void Load2dTexture();
    void genTextureImage();
public:
    View(QWidget* parent = Q_NULLPTR);
    void LoadData(std::string filename);
    int getState() { return visualization_state; }
public slots:
    void SetMin(short value);
    void SetMax(short value);

    void PressW();
    void PressS();
    void PressN();

    void SetX();
    void SetY();

    short getMin();
    short getMax();
};

