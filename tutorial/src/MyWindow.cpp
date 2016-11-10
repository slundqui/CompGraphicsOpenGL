#include "MyWindow.h"
#include <Fl/Gl.h>
#include <Fl/Fl.h>
#include <OpenGl/Glu.h>
#include <iostream>
#include <math.h>

void IdleCallback(void* pData)
{
    if (pData != NULL)
    {
        MyWindow* pWindow = reinterpret_cast<MyWindow*>(pData);
        if (pWindow->animating)
        {
            pWindow->rotation += pWindow->rotationIncrement / 100;
            pWindow->redraw();
        }
    }
}

MyWindow::MyWindow(int width, int height, char* title) : Fl_Gl_Window(width, height, title)
{
    mode(FL_RGB | FL_ALPHA | FL_DEPTH | FL_DOUBLE);
    rotation = 0.f;
    rotationIncrement=10.f;
    animating = false;

    Fl::add_idle(IdleCallback, this);
}

void MyWindow::InitializeGL()
{
    glClearColor(.1f, .1f, .1f, 1);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    float lightColor[4] = {1, 1, 1, 1};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    LoadTexture("graphics.tga");
}

void MyWindow::draw()
{
    static bool firstTime = true;
    if (firstTime)
    {
        InitializeGL();
        firstTime = false;
    }// if

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      // clear the color and depth buffer

    // view transformations
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 1, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);
    float lightPosition[4] = {5, 5, 5, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glRotatef(rotation, 0, 1, 0);


    // draw something
    DrawCube();
}

void MyWindow::DrawCube()
{
    glBegin(GL_QUADS);
    // front
    glNormal3f(0, 0, 1);
    glColor3f(1, 0, 0);
    glTexCoord2f(0, 1); glVertex3f(-1, 1, 1);
    glTexCoord2f(0, 0); glVertex3f(-1, -1, 1);
    glTexCoord2f(1, 0); glVertex3f(1, -1, 1);
    glTexCoord2f(1, 1); glVertex3f(1, 1, 1);

    // back
    glNormal3f(0, 0, -1);
    glColor3f(0, 1, 0);
    glTexCoord2f(1, 1); glVertex3f(-1, 1, -1);
    glTexCoord2f(0, 1); glVertex3f(1, 1, -1);
    glTexCoord2f(0, 0); glVertex3f(1, -1, -1);
    glTexCoord2f(1, 0); glVertex3f(-1, -1, -1);

    // top
    glNormal3f(0, 1, 0);
    glColor3f(0, 0, 1);
    glTexCoord2f(0, 1); glVertex3f(-1, 1, -1);
    glTexCoord2f(0, 0); glVertex3f(-1, 1, 1);
    glTexCoord2f(1, 0); glVertex3f(1, 1, 1);
    glTexCoord2f(1, 1); glVertex3f(1, 1, -1);

    // bottom
    glNormal3f(0, -1, 0);
    glColor3f(1, 1, 0);
    glTexCoord2f(0, 0); glVertex3f(-1, -1, -1);
    glTexCoord2f(1, 0); glVertex3f(1, -1, -1);
    glTexCoord2f(1, 1); glVertex3f(1, -1, 1);
    glTexCoord2f(0, 1); glVertex3f(-1, -1, 1);

    // left
    glNormal3f(-1, 0, 0);
    glColor3f(0, 1, 1);
    glTexCoord2f(0, 1); glVertex3f(-1, 1, -1);
    glTexCoord2f(0, 0); glVertex3f(-1, -1, -1);
    glTexCoord2f(1, 0); glVertex3f(-1, -1, 1);
    glTexCoord2f(1, 1); glVertex3f(-1, 1, 1);

    // right
    glNormal3f(1, 0, 0);
    glColor3f(1, 0, 1);
    glTexCoord2f(0, 1); glVertex3f(1, 1, 1);
    glTexCoord2f(0, 0); glVertex3f(1, -1, 1);
    glTexCoord2f(1, 0); glVertex3f(1, -1, -1);
    glTexCoord2f(1, 1); glVertex3f(1, 1, -1);
    glEnd();
}

int MyWindow::handle(int event)
{
    switch (event)
    {
        case FL_FOCUS:
        case FL_UNFOCUS:
            return 1;

        case FL_KEYBOARD:
            int key = Fl::event_key();
            switch (key)
            {
                case FL_Left:
                    rotation -= rotationIncrement;
                    redraw();
                    return 1;

                case FL_Right:
                    rotation += rotationIncrement;
                    redraw();
                    return 1;

                case ' ':
                    animating = !animating;
                    return 1;
            }
    }

    return Fl_Gl_Window::handle(event);
}

void MyWindow::LoadTexture(char* filename)
{
    TargaImage* image = TargaImage::Load_Image(filename);
    if (!image)
    {
        std::cerr << "Failed to load texture:  " << filename << std::endl;
        return;
    }

    // reverse the row order
    TargaImage* reversedImage = image->Reverse_Rows();
    delete image;
    image = reversedImage;

    if (!ResizeImage(image))
    {
        std::cerr << "Failed to resize texture." << std::endl;
        return;
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
}

bool MyWindow::ResizeImage(TargaImage* image)
{
    int newWidth = pow(2.0, (int)ceil(log((float)image->width) / log(2.f)));
    int newHeight = pow(2.0, (int)ceil(log((float)image->width) / log(2.f)));

    newWidth = fmax(64, newWidth);
    newHeight = fmax(64, newHeight);

    if (newWidth != image->width && newHeight != image->height)
    {
        unsigned char* scaledData = new unsigned char[newWidth * newHeight * 4];
        if (gluScaleImage(GL_RGBA, image->width, image->height, GL_UNSIGNED_BYTE, image->data, newWidth, newHeight, GL_UNSIGNED_BYTE, scaledData) != 0)
        {
            delete[] scaledData;
            return false;
        }// if

        delete image->data;
        image->data = scaledData;
        image->width = newWidth;
        image->height = newHeight;
    }// if

    return true;
}

MyWindow::~MyWindow()
{}
