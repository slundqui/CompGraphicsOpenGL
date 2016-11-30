/*
 * Mountain.cpp: A class for drawing the ground.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#include "Mountain.h"
#include "libtarga.h"
#include <stdio.h>
#include <OpenGL/glu.h>

// Destructor
Mountain::~Mountain(void)
{
    if ( initialized )
    {
        ClearSubdivision();
    }
}

void Mountain::DrawTriangle(struct triangle* t){
    float p1x = t->p1->x;
    float p1y = t->p1->y;
    float p1z = t->p1->z;
    float p2x = t->p2->x;
    float p2y = t->p2->y;
    float p2z = t->p2->z;
    float p3x = t->p3->x;
    float p3y = t->p3->y;
    float p3z = t->p3->z;
    
    //Calculate surface normal
    float ux = p2x - p1x;
    float uy = p2y - p1y;
    float uz = p2z - p1z;

    float vx = p3x - p1x;
    float vy = p3y - p1y;
    float vz = p3z - p1z;

    float nx = uy*vz - uz*vy;
    float ny = uz*vx - ux*vz;
    float nz = ux*vy - uy*vx;

    glNormal3f(nx, ny, nz);

    //Color gray
    glColor3f(0.2+p2z/80, 0.2+p2z/80, 0.2+p2z/80);
    glVertex3f(p1x, p1y, p1z);
    glVertex3f(p2x, p2y, p2z);
    glVertex3f(p3x, p3y, p3z);
}

void Mountain::DrawTriangles(void){
    glBegin(GL_TRIANGLES);
    for(std::vector <struct triangle*>::iterator it = triList.begin(); it != triList.end(); ++it){
        DrawTriangle(*it);
    }
    glEnd();
}

void Mountain::ClearSubdivision(){
    edgePtLookup.clear();
    //Free all triangles
    for(std::vector <struct triangle*>::iterator it = triList.begin(); it != triList.end(); ++it){
        delete *it;
    }
    triList.clear();
    //Free all points
    for(std::vector <struct point*>::iterator it = pointList.begin(); it != pointList.end(); ++it){
        delete *it;
    }
    pointList.clear();

}


void Mountain::AddTriangle(struct point * p1, struct point * p2, struct point * p3, bool addToPointList){
    if(addToPointList){
        //We push points onto pointList for free later
        pointList.push_back(p1);
        pointList.push_back(p2);
        pointList.push_back(p3);
    }

    //Only make triangle if there exists a point with z > 0
    if(p1->z > 0 || p2->z > 0 || p3->z > 0){
        struct triangle* t = new triangle();
        t->p1 = p1;
        t->p2 = p2;
        t->p3 = p3;
        //Add triangle to vector
        triList.push_back(t);
    }
}
void Mountain::ResetSubdivision(){
    ClearSubdivision();

    //Reset update val
    randUpdateVal = 10;

    //Initial subdivision triangle
    struct point * p1 = new point();
    struct point * p2 = new point();
    struct point * p3 = new point();

    p1->x = -20;
    p1->y = 50;
    p1->z = 0;

    p2->x = 50;
    p2->y = -20;
    p2->z = 0;

    p3->x = 50;
    p3->y = 50;
    p3->z = 50;

    AddTriangle(p1, p2, p3, true);

    p1 = new point();
    p2 = new point();
    p3 = new point();

    p1->x = -10;
    p1->y = -50;
    p1->z = 0;

    p2->x = -50;
    p2->y = -10;
    p2->z = 0;

    p3->x = -50;
    p3->y = -50;
    p3->z = 20;

    AddTriangle(p1, p2, p3, true);

    p1 = new point();
    p2 = new point();
    p3 = new point();

    p1->x = -50;
    p1->y = 10;
    p1->z = 0;

    p2->x = 10;
    p2->y = 50;
    p2->z = 0;

    p3->x = -50;
    p3->y = 50;
    p3->z = 20;

    AddTriangle(p1, p2, p3, true);

    p1 = new point();
    p2 = new point();
    p3 = new point();

    p1->x = 50;
    p1->y = 0;
    p1->z = 0;

    p2->x = 0;
    p2->y = -50;
    p2->z = 0;

    p3->x = 50;
    p3->y = -50;
    p3->z = 40;

    AddTriangle(p1, p2, p3, true);

}

// Initializer. Returns false if something went wrong, like not being able to
// load the texture.
bool
Mountain::Initialize(void)
{
    randUpdateRatio = .6;

    ResetSubdivision();

    // We only do all this stuff once, when the GL context is first set up.
    initialized = true;

    return true;
}

//Generates a random float between +- randUpdateVal
float Mountain::RandomFloat() {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = 2*randUpdateVal;
    float r = random * diff;
    return -randUpdateVal + r;
}

//This function will look up if a vertex on edge p1-p2 already exists
//If not, it will create it
struct point* Mountain::GetPointFromEdge(struct point * p1, struct point * p2){
    //Attempt to find existing points if it already exists
    //Note that since edges are orderless, we test both permutations of endpoints
    struct point * newP;
    //Get all edge combinations
    std::pair<struct point*, struct point*> p1p2 = std::pair<struct point*, struct point*>(p1, p2);
    std::pair<struct point*, struct point*> p2p1 = std::pair<struct point*, struct point*>(p2, p1);
    //If exists, use the point
    if(edgePtLookup.count(p1p2) > 0){
        newP = edgePtLookup.at(p1p2);
    }
    else if(edgePtLookup.count(p2p1) > 0){
        newP = edgePtLookup.at(p2p1);
    }
    //Not found, create new point
    else{
        newP = new point();
        //New points based on halfway points of 3 edges
        newP->x = p1->x + (p2->x - p1->x)/2;
        newP->y = p1->y + (p2->y - p1->y)/2;
        newP->z = p1->z + (p2->z - p1->z)/2;
        //Additinally, if the new z <= 0 (on the floor), dont change
        //This will remove weird artifacts at the bottom of the mountain
        if(newP->z > 0){
            newP->z += RandomFloat();
            //Constrain
            if(newP->z < 0){
                newP->z = 0;
            }
        }
        //Add this edge to the lookup
        edgePtLookup[p1p2] = newP;
        pointList.push_back(newP);
    }
    return newP;
}

//Subdivides a given triangle and pushes it onto the triList
void Mountain::SubdivideTriangle(struct triangle * t){
    
    struct point * p1 = t->p1;
    struct point * p2 = t->p2;
    struct point * p3 = t->p3;

    struct point * newP1 = GetPointFromEdge(p1, p2);
    struct point * newP2 = GetPointFromEdge(p2, p3);
    struct point * newP3 = GetPointFromEdge(p3, p1);

    //GetPointFromEdge already adds the point to the pointList if necessary
    AddTriangle(newP1, newP2, newP3, false);
    AddTriangle(p1, newP1, newP3, false);
    AddTriangle(newP1, p2, newP2, false);
    AddTriangle(newP3, newP2, p3, false);
}

void Mountain::Subdivide(){
    int numList = triList.size();
    for(int i = 0; i < numList; i++){
        struct triangle* t = triList[i];
        //This function will add triangles to triList
        SubdivideTriangle(t);
    }

    //Clear old triangles
    for(int i = 0; i < numList; i++){
        delete triList[i];
    }
    
    //Erase from vector
    triList.erase(triList.begin(), triList.begin() + numList);

    randUpdateVal *= randUpdateRatio;
}


// Draw just calls the display list we set up earlier.
void
Mountain::Draw(void)
{
    glPushMatrix();
    DrawTriangles();
    glPopMatrix();
}


