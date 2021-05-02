#pragma once
#pragma comment (lib, "opengl32.lib")

#include"Data.h"
#include<iostream>
#include <QDebug>
#include <QKeyEvent>
#include <QtWidgets/QOpenGLWidget>

class View : public QOpenGLWidget
{
    Q_OBJECT
        Data data;
    enum { VISUALIZATION_QUADS, VISUALIZATION_QUADSTRIP, VISUALIZATION_TEXTURE } visualization_state;
    int layer;
    GLuint VBOtexture;
    QImage textureImage;
public:
    View(QWidget* parent = Q_NULLPTR);
    void LoadData(std::string filename);
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void VisualisationQuads();
    void VisualisationQuadStrip();
    void VisualisationTexture();
    float TransferFunction(short value);
    void Load2dTexture();
    void genTextureImage();
    void keyPressEvent(QKeyEvent* event) override;
};
