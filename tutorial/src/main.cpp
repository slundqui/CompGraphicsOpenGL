#include <Fl/Fl.h>
#include "MyWindow.h"

int main(int argc, char** args)
{
    MyWindow myWindow(400, 400, "CS447 Tutorial");
    myWindow.show();

    Fl::run();

    return 0;
}
