#include "View.h"

template <class T1, class T2>
T1 limit_value(T1 x, T2 a, T2 b)
{
    if (x <= a)
        return (T1)a;
    if (x >= b)
        return (T1)b;
    return x;
}
View::View(QWidget* parent)
    :QOpenGLWidget(parent), data(), VBOtexture(), textureImage()
{
    visualization_state = VISUALIZATION_QUADS;
    layer = 0;
    cut = xy;
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
    int w = 0, h = 0;
    w = data.GetWidth() - 1;
    h = data.GetHeight() - 1;
    glOrtho(0.f, w, 0.f, h, -1.f, 1.f);
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
    //c = limit_value(c, 0, 1);
    return c;
}
void View::VisualisationQuads()
{
    float c;
    int w = data.GetWidth();
    int h = data.GetHeight();
    int d = data.GetDepth();

    if (cut == xy)
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
    else if (cut == yz)
    {
        for (int z = 0; z < d - 1; z++)
        {
            for (int y = 0; y < h - 1; y++)
            {
                glBegin(GL_QUADS);
                c = TransferFunction(data[layer + y * w + z * w * h]);
                glColor3f(c, c, c);
                glVertex2i(y, z);

                c = TransferFunction(data[layer + y * w + (z + 1) * w * h]);
                glColor3f(c, c, c);
                glVertex2i(y, z + 1);

                c = TransferFunction(data[layer + (y + 1) * w + (z + 1) * w * h]);
                glColor3f(c, c, c);
                glVertex2i(y + 1, z + 1);

                c = TransferFunction(data[layer + (y + 1) * w + z * w * h]);
                glColor3f(c, c, c);
                glVertex2i(y + 1, z);
                glEnd();
            }
        }
    }
    else
    {
        for (int z = 0; z < d - 1; z++)
        {
            for (int x = 0; x < w - 1; x++)
            {
                glBegin(GL_QUADS);
                c = TransferFunction(data[layer * w + x + z * w * h]);
                glColor3f(c, c, c);
                glVertex2i(x, z);

                c = TransferFunction(data[layer * w + x + z * w * h]);
                glColor3f(c, c, c);
                glVertex2i(x, z + 1);

                c = TransferFunction(data[layer * w + (x + 1) + (z + 1) * w * h]);
                glColor3f(c, c, c);
                glVertex2i(x + 1, z + 1);

                c = TransferFunction(data[layer * w + (x + 1) + z * w * h]);
                glColor3f(c, c, c);
                glVertex2i(x + 1, z);
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
    if (cut == xy)
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
    else if (cut == yz)
    {
        for (int z = 0; z < d - 1; z++)
        {
            glBegin(GL_QUAD_STRIP);
            for (int y = 0; y < h; y++)
            {
                c = TransferFunction(data[layer + y * w + z * h * w]);
                glColor3f(c, c, c);
                glVertex2i(y,z);

                c = TransferFunction(data[layer + y * w + (z + 1) * h * w]);
                glColor3f(c, c, c);
                glVertex2i(y, z + 1);
            }
            glEnd();
        }
    }
    else
    {
        for (int z = 0; z < d - 1; z++)
        {
            glBegin(GL_QUAD_STRIP);
            for (int x = 0; x < w; x++)
            {
                c = TransferFunction(data[layer * w + x + z * w * h]);
                glColor3f(c, c, c);
                glVertex2i(x, z);

                c = TransferFunction(data[layer * w + x + (z + 1) * w * h]);
                glColor3f(c, c, c);
                glVertex2i(x, z + 1);
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
    if (cut == xy) {
        glTexCoord2f(0, 0);
        glVertex2i(0, 0);
        glTexCoord2f(0, 1);
        glVertex2i(0, data.GetHeight());
        glTexCoord2f(1, 1);
        glVertex2i(data.GetWidth(), data.GetHeight());
        glTexCoord2f(1, 0);
        glVertex2i(data.GetWidth(), 0);
    }
    else if (cut == yz)
    {
        glTexCoord2f(0, 0);
        glVertex2i(0, 0);
        glTexCoord2f(0, 1);
        glVertex2i(0, data.GetDepth());
        glTexCoord2f(1, 1);
        glVertex2i(data.GetHeight(), data.GetDepth());
        glTexCoord2f(1, 0);
        glVertex2i(data.GetHeight(), 0);
    }
    else
    {
        glTexCoord2f(0, 0);
        glVertex2i(0, 0);
        glTexCoord2f(0, 1);
        glVertex2i(0, data.GetDepth());
        glTexCoord2f(1, 1);
        glVertex2i(data.GetWidth(), data.GetDepth());
        glTexCoord2f(1, 0);
        glVertex2i(data.GetWidth(), 0);
    }
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

    qDebug() << "GEN_TEXTURE";
    if (cut == xy)
    {
        textureImage = QImage(w, h, QImage::Format_RGB32);
        for (int y = 0; y < h; y++)
            for (int x = 0; x < w; x++)
            {
                float c = TransferFunction(data[layer * w * h + x + y * w]) * 255.f;
                c = limit_value(c, 0, 255);
                textureImage.setPixelColor(x, y, QColor(c, c, c));
            }
    }
    else if (cut == yz)
    {
        textureImage = QImage(h, d, QImage::Format_RGB32);
        for (int z = 0; z < d; z++)
            for (int y = 0; y < h; y++)
            {
                float c = TransferFunction(data[layer + y * w + z * w * h]) * 255.f;
                c = limit_value(c, 0, 255);
                textureImage.setPixelColor(y, z, QColor(c, c, c));
            }
    }
    else
    {
        textureImage = QImage(w, d, QImage::Format_RGB32);
        for (int z = 0; z < d; z++)
            for (int x = 0; x < w; x++)
            {
                float c = TransferFunction(data[layer * w + x + z * w * h]) * 255.f;
                c = limit_value(c, 0, 255);
                textureImage.setPixelColor(x, z, QColor(c, c, c));
            }
   }
}

void View::SetXY()
{
    cut = xy; 
    update();
}
void View::SetYZ()
{
    cut = yz;
    update();
}
void View::SetXZ()
{
    cut = xz;
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
    if (cut == xy)
        layer = std::min(layer + 1, data.GetDepth() - 1);
    else if (cut == yz)
        layer = std::min(layer + 1, data.GetWidth() - 1);
    else
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
short View::GetMin()
{
    return data.GetMin();
}
short View::GetMax()
{
    return data.GetMax();
}
int View::GetDataWidth()
{
    return data.GetWidth();
}
int View::GetDataHeight()
{
    return data.GetHeight();
}
int View::GetDataDepth()
{
    return data.GetDepth();
}