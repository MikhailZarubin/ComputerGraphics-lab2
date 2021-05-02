#include "View.h"

View::View(QWidget *parent)
    :QOpenGLWidget(parent), data(), visualization_state(VISUALIZATION_QUADS), layer(0), VBOtexture(),  textureImage ()
{
   
}
void View::LoadData(std::string filename)
{
    data.ReadFile(filename);
}
void View::initializeGL()
{
    glClearColor(0, 0, 0, 0);
    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glGenTextures(1, &VBOtexture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}
void View::resizeGL(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.f, data.GetWidth() - 1, 0.f, data.GetHeight() - 1, -1.f, 1.f);
    glViewport(0, 0, width, height);
    update();
}
void View::paintGL()
{
    qDebug() << "repaint" << visualization_state;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch (visualization_state)
    {
    case VISUALIZATION_QUADS:
        VisualisationQuads();
        break;
    case VISUALIZATION_QUADSTRIP:
        VisualisationQuadStrip();
        break;
    case VISUALIZATION_TEXTURE:
        VisualisationTexture();
        break;
    }
}
float View::TransferFunction(short value)
{
    float c = (value - data.GetMin()) / float(data.GetMax() - data.GetMin());
    return c;
}
void View::VisualisationQuads()
{
    float c;
    int w = data.GetWidth();
    int h = data.GetHeight();

    for (int y = 0; y < h - 1; y++) {
        for (int x = 0; x < w - 1; x++) {
            glBegin(GL_QUADS);

            c = TransferFunction(data[layer* w * h + y * w + x]);
            glColor3f(c, c, c);
            glVertex2i(x, y);

            c = TransferFunction(data[layer * w * h + (y + 1) * w + x]);
            glColor3f(c, c, c);
            glVertex2i(x, y + 1);

            c = TransferFunction(data[layer * w * h + (y + 1) * w + x + 1]);
            glColor3f(c, c, c);
            glVertex2i(x + 1, y + 1);

            c = TransferFunction(data[layer * w * h + y * w + x + 1]);
            glColor3f(c, c, c);
            glVertex2i(x + 1, y);

            glEnd();
        }
    }
}
void View::VisualisationQuadStrip()
{
}
void View::VisualisationTexture()
{

    genTextureImage();
    Load2dTexture();

    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2i(0, 0);
    glTexCoord2f(0, 1);
    glVertex2i(0, data.GetHeight());
    glTexCoord2f(1, 1);
    glVertex2i(data.GetWidth(), data.GetHeight());
    glTexCoord2f(1, 0);
    glVertex2i(data.GetWidth(), 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void View::keyPressEvent(QKeyEvent* event)
{
    unsigned int key_pressed = event->nativeVirtualKey();
    if (key_pressed == Qt::Key_U || key_pressed == Qt::Key_Up) {
        layer = std::min(layer + 1, data.GetDepth() - 1);
    }
    else 
        if (key_pressed == Qt::Key_D || key_pressed == Qt::Key_Down) {
        layer = std::max(layer - 1, 0);
    }
    else if (key_pressed == Qt::Key_N) {
        switch (visualization_state) {
        case VISUALIZATION_QUADS:
            visualization_state = VISUALIZATION_QUADSTRIP; break;
        case VISUALIZATION_QUADSTRIP:
            visualization_state = VISUALIZATION_TEXTURE; break;
        case VISUALIZATION_TEXTURE:
            visualization_state = VISUALIZATION_QUADS; break;
        }
    }

    update();
}
void View::Load2dTexture()
{
    glBindTexture(GL_TEXTURE_2D, VBOtexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage.width(), textureImage.height(),
        0, GL_BGRA, GL_UNSIGNED_BYTE, textureImage.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
void View::genTextureImage()
{
    int w = data.GetWidth();
    int h = data.GetHeight();
    textureImage = QImage(w, h, QImage::Format_RGB32);
    qDebug() << "GEN_TEXTURE";
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
        {
            float c = TransferFunction(data[layer * w * h + w * y + x]) * 255;
            textureImage.setPixelColor(x, y, QColor(c, c, c));
        }
}
