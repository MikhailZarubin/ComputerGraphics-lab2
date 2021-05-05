#pragma once
#pragma comment (lib, "opengl32.lib")

#include"Data.h"
#include<iostream>
#include <QDebug>
#include <QKeyEvent>
#include <QtWidgets/QOpenGLWidget>
#include <QtOpenGL/QGLFunctions>

template<class T1, class T2>
T1 limit_value(T1 x, T2 a, T2 b);

class View : public QOpenGLWidget
{
    Q_OBJECT
protected:
    Data data;
    enum { VISUALIZATION_QUADS, VISUALIZATION_QUADSTRIP, VISUALIZATION_TEXTURE } visualization_state;
    enum { xy, yz, xz} cut;
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
    int GetState() { return visualization_state; }
    int GetLayer() { return layer; }
public slots:
    void SetMin(short value);
    void SetMax(short value);

    void PressW();
    void PressS();
    void PressN();

    void SetXY();
    void SetYZ();
    void SetXZ();

    short GetMin();
    short GetMax();

    int GetDataWidth();
    int GetDataHeight();
    int GetDataDepth();
};

