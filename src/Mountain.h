/*
 * Mountain.h: Header file for a class that draws the ground.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#ifndef _MOUNTAIN_H_
#define _MOUNTAIN_H_

#include <Fl/gl.h>
#include <vector>
#include <map>
#include <stdlib.h>

struct point{
    float x;
    float y;
    float z;
};

struct triangle{
    struct point * p1;
    struct point * p2;
    struct point * p3;
};

class Mountain {
  private:
    //GLubyte display_list;   // The display list that does all the work.
    //GLuint  texture_obj;    // The object for the grass texture.
    bool    initialized;    // Whether or not we have been initialised.
    bool    updated;
    float   randUpdateVal;
    float   randUpdateRatio;

    //This stores a list of triangles
    std::vector <struct triangle*> triList;
    //This stores all points in a list
    //This is only used for deleting the memory during a reset
    std::vector <struct point*> pointList;

    //A map that takes an edge in the form of a pair and returns an existing point on that edge
    std::map <std::pair<struct point*, struct point*>, struct point*> edgePtLookup;

    void DrawTriangle(struct triangle* t);
    void DrawTriangles();
    float RandomFloat();
    void SubdivideTriangle(struct triangle* t);
    struct point* GetPointFromEdge(struct point * p1, struct point * p2);
    void ClearSubdivision();
    void AddTriangle(struct point * p1, struct point * p2, struct point * p3, bool addToPointList);

  public:
    // Constructor. Can't do initialization here because we are
    // created before the OpenGL context is set up.
    Mountain(void) { initialized = false; };
    void Subdivide(void);
    void ResetSubdivision();

    // Destructor. Frees the display lists and texture object.
    ~Mountain(void);

    // Initializer. Creates the display list.
    bool    Initialize(void);

    // Does the drawing.
    void    Draw(void);
};


#endif
