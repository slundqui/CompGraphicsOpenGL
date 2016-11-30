/*
 * Building.h: Header file for a class that draws buildings.
 *
 */


#ifndef _BUILDING_H_
#define _BUILDING_H_

#include <Fl/gl.h>

class Building {
  private:
    GLubyte display_list;   // A list of display_lists for multiple buildings
    GLuint  texture_obj_wall;    // The object for the grass texture.
    GLuint  texture_obj_roof;    // The object for the grass texture.
    bool    initialized;    // Whether or not we have been initialised.

    void LoadTexure(const char* filename);
    void DrawBuilding(float xsize, float ysize, float zsize, float roof_height);
    void DrawWalls(float xsize, float ysize, float zsize);
    void DrawRoof(float xsize, float ysize, float zsize, int roof_height);
    void DrawTriangle(float, float, float, float, float, float, float, float, float);
    bool LoadTexture(const char* filename, GLuint* textureObj);

  public:
    // Constructor. Can't do initialization here because we are
    // created before the OpenGL context is set up.
    Building(void) { display_list = 0; initialized = false; };

    // Destructor. Frees the display lists and texture object.
    ~Building(void);

    // Initializer. Creates the display list.
    bool    Initialize(void);

    // Does the drawing.
    void    Draw(void);
};


#endif
