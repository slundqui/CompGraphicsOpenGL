#ifndef MY_WINDOW_H
#define MY_WINDOW_H

#include <Fl/Fl_Gl_Window.h>
#include "TargaImage.h"

class MyWindow : public Fl_Gl_Window
{
public:
    MyWindow(int width, int height, char* title);
    void InitializeGL();
    virtual void draw();
    void DrawCube();
    virtual int handle(int event);
    bool ResizeImage(TargaImage* image);
    void LoadTexture(char* filename);

    float rotation, rotationIncrement;
    bool animating;
    unsigned int textureId;

    virtual ~MyWindow();
};

#endif
