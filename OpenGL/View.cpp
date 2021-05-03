#include "View.h"

View::View(QWidget* parent)
    :QOpenGLWidget(parent), data(), visualization_state(VISUALIZATION_QUADS), layer(0), VBOtexture(), textureImage(), cut(x)
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
    qDebug() << "min=" << data.GetMin();
    qDebug() << "max= " << data.GetMax();
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
    int d = data.GetDepth();

    if (cut == x)
    {
        for (int y = 0; y < h - 1; y++)
        {
            for (int x = 0; x < w - 1; x++)
            {
                glBegin(GL_QUADS);
                c = TransferFunction(data[layer * w * h + y * w + x]);
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
    if (cut == y)
    {
        for (int y = 0; y < h - 1; y++)
        {
            for (int x = 0; x < w - 1; x++)
            {
                glBegin(GL_QUADS);
                c = TransferFunction(data[layer + y * d + x * w * h]);
                glColor3f(c, c, c);
                glVertex2i(x, y);

                c = TransferFunction(data[layer + (y + 1) * d + x * w * h]);
                glColor3f(c, c, c);
                glVertex2i(x, y + 1);

                c = TransferFunction(data[layer + (y + 1) * d + (x + 1) * w * h]);
                glColor3f(c, c, c);
                glVertex2i(x + 1, y + 1);

                c = TransferFunction(data[layer + y * d + (x + 1) * w * h]);
                glColor3f(c, c, c);
                glVertex2i(x + 1, y);
                glEnd();
            }
        }
    }
}

void View::VisualisationQuadStrip()
{
    float c;
    int w = data.GetWidth();
    int h = data.GetHeight();
    int d = data.GetDepth();
    if (cut == x)
    {
        for (int y = 0; y < h - 1; y++)
        {
            glBegin(GL_QUAD_STRIP);
            for (int x = 0; x < w; x++)
            {
                c = TransferFunction(data[layer * w * h + y * w + x]);
                glColor3f(c, c, c);
                glVertex2i(x, y);

                c = TransferFunction(data[layer * w * h + (y + 1) * w + x]);
                glColor3f(c, c, c);
                glVertex2i(x, y + 1);
            }
            glEnd();
        }
    }
    if (cut == y)
    {
        for (int y = 0; y < h - 1; y++)
        {
            glBegin(GL_QUAD_STRIP);
            for (int x = 0; x < w; x++)
            {
                c = TransferFunction(data[layer + y * d + x * w * h]);
                glColor3f(c, c, c);
                glVertex2i(x, y);

                c = TransferFunction(data[layer + (y + 1) * d + x * w * h]);
                glColor3f(c, c, c);
                glVertex2i(x, y + 1);
            }
            glEnd();
        }
    }
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
    int d = data.GetDepth();

    textureImage = QImage(w, h, QImage::Format_RGB32);
    qDebug() << "GEN_TEXTURE";
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
        {
            float c;
            if (cut == x)
                c = TransferFunction(data[layer * w * h + w * y + x]);
            if (cut == y)
                c = TransferFunction(data[layer + y * d + x * w * h]);
            if (c > 255)
                c = 255;
            if (c < 0)
                c = 0;
            textureImage.setPixelColor(x, y, QColor(c, c, c));
        }
}

void View::SetX()
{
    cut = x; 
    update();
}
void View::SetY()
{
    cut = y;
    update();
}

void View::SetMin(short value)
{
    data.SetMin(value);
    update();
}
void View::SetMax(short value)
{
    data.SetMax(value);
    update();
}
void View::PressW()
{
    layer = std::min(layer + 1, data.GetHeight() - 1);
    update();
}

void View::PressS()
{
    layer = std::max(layer - 1, 0);
    update();
}

void View::PressN()
{
    switch (visualization_state)
    {
    case VISUALIZATION_QUADS:
        visualization_state = VISUALIZATION_QUADSTRIP; break;
    case VISUALIZATION_QUADSTRIP:
        visualization_state = VISUALIZATION_TEXTURE; break;
    case VISUALIZATION_TEXTURE:
        visualization_state = VISUALIZATION_QUADS; break;
    }
    update();
}
short View::getMin()
{
    return data.GetMin();
}
short View::getMax()
{
    return data.GetMax();
}