/*
 * Building.cpp: A class for drawing buildings.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#include "Building.h"
#include "libtarga.h"
#include <stdio.h>
#include <OpenGL/glu.h>

// Destructor
Building::~Building(void)
{
    if ( initialized )
    {
	glDeleteLists(display_list, 1);
	glDeleteTextures(1, &texture_obj_wall);
	glDeleteTextures(1, &texture_obj_roof);
    }
}

bool Building::LoadTexture(const char* filename, GLuint* textureObj){
    ubyte   *image_data;
    int	    image_height, image_width;

    // Load the image for the texture. The texture file has to be in
    // a place where it will be found.
    if ( ! ( image_data = (ubyte*)tga_load(filename, &image_width,
					   &image_height, TGA_TRUECOLOR_24) ) )
    {
	fprintf(stderr, "Building::Initialize: Couldn't load %s\n", filename);
	return false;
    }

    // This creates a texture object and binds it, so the next few operations
    // apply to this texture.
    glGenTextures(1, textureObj);
    glBindTexture(GL_TEXTURE_2D, *textureObj);

    // This sets a parameter for how the texture is loaded and interpreted.
    // basically, it says that the data is packed tightly in the image array.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // This sets up the texture with high quality filtering. First it builds
    // mipmaps from the image data, then it sets the filtering parameters
    // and the wrapping parameters. We want the grass to be repeated over the
    // ground.
    gluBuild2DMipmaps(GL_TEXTURE_2D,3, image_width, image_height, 
		      GL_RGB, GL_UNSIGNED_BYTE, image_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		    GL_NEAREST_MIPMAP_LINEAR);

    // This says what to do with the texture. Modulate will multiply the
    // texture by the underlying color.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
    return true;
}

// Initializer. Returns false if something went wrong, like not being able to
// load the texture.
bool
Building::Initialize(void)
{
    if(!LoadTexture("brick.tga", &texture_obj_wall)){
        return false;
    }
    if(!LoadTexture("roof.tga", &texture_obj_roof)){
        return false;
    }

    // Now do the geometry. Create the display list.
    display_list = glGenLists(3);

    glNewList(display_list, GL_COMPILE);
    DrawBuilding(3, 5, 2, 4);
    glEndList();

    glNewList(display_list+1, GL_COMPILE);
    DrawBuilding(6, 3, 5, 2);
    glEndList();

    glNewList(display_list+2, GL_COMPILE);
    DrawBuilding(4, 10, 7, 3);
    glEndList();

    // We only do all this stuff once, when the GL context is first set up.
    initialized = true;

    return true;
}

void Building::DrawBuilding(float xsize, float ysize, float zsize, float roof_height){
	// Turn on texturing and bind the wall texture.
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj_wall);

	// Draw the walls of the building
    DrawWalls(xsize, ysize, zsize);

    //Bind the roof texture
	glBindTexture(GL_TEXTURE_2D, texture_obj_roof);

    //Draw the roof of the building
    DrawRoof(xsize, ysize, zsize, roof_height);

	// Turn texturing off again, because we don't want everything else to
	// be textured.
	glDisable(GL_TEXTURE_2D);
}

void Building::DrawTriangle(float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float p3x, float p3y, float p3z){

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

    //glColor3f(0, 0, 1);
    glTexCoord2f(0, 0); glVertex3f(p1x, p1y, p1z);
    glTexCoord2f(1, 0); glVertex3f(p2x, p2y, p2z);
    glTexCoord2f(.5, 1); glVertex3f(p3x, p3y, p3z);
}

void Building::DrawRoof(float xsize, float ysize, float zsize, int roof_height){
    //// top
    //glNormal3f(0, 0, 1);
    ////glColor3f(1, 0, 0);
    //
    glBegin(GL_TRIANGLES);

    DrawTriangle(1*xsize, -1*ysize, 2*zsize, 1*xsize, 1*ysize, 2*zsize, 0, 0, 2*zsize+roof_height);

    DrawTriangle(1*xsize, 1*ysize, 2*zsize, -1*xsize, 1*ysize, 2*zsize, 0, 0, 2*zsize+roof_height);

    DrawTriangle(-1*xsize, 1*ysize, 2*zsize, -1*xsize, -1*ysize, 2*zsize, 0, 0, 2*zsize+roof_height);

    DrawTriangle(-1*xsize, -1*ysize, 2*zsize, 1*xsize, -1*ysize, 2*zsize, 0, 0, 2*zsize+roof_height);

    glEnd();
}

void Building::DrawWalls(float xsize, float ysize, float zsize)
{

    glBegin(GL_QUADS);
    // Sides

    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0); glVertex3f(-1*xsize, 1*ysize,  0);
    glTexCoord2f(0, 1); glVertex3f(-1*xsize, 1*ysize,  2*zsize);
    glTexCoord2f(1, 1); glVertex3f(1 *xsize, 1*ysize,  2*zsize);
    glTexCoord2f(1, 0); glVertex3f(1 *xsize, 1*ysize,  0*zsize);

    glNormal3f(0, -1, 0);
    glTexCoord2f(0, 0); glVertex3f(-1*xsize, -1*ysize,  0);
    glTexCoord2f(1, 0); glVertex3f( 1*xsize, -1*ysize,  0);
    glTexCoord2f(1, 1); glVertex3f( 1*xsize, -1*ysize,  2*zsize);
    glTexCoord2f(0, 1); glVertex3f(-1*xsize, -1*ysize,  2*zsize);

    glNormal3f(-1, 0, 0);
    glTexCoord2f(1, 0); glVertex3f(-1*xsize,  1*ysize,  0);
    glTexCoord2f(0, 0); glVertex3f(-1*xsize, -1*ysize,  0);
    glTexCoord2f(0, 1); glVertex3f(-1*xsize, -1*ysize,  2*zsize);
    glTexCoord2f(1, 1); glVertex3f(-1*xsize,  1*ysize,  2*zsize);

    glNormal3f(1, 0, 0);
    glTexCoord2f(1, 1); glVertex3f(1*xsize,  1*ysize,  2*zsize);
    glTexCoord2f(0, 1); glVertex3f(1*xsize, -1*ysize,  2*zsize);
    glTexCoord2f(0, 0); glVertex3f(1*xsize, -1*ysize,  0);
    glTexCoord2f(1, 0); glVertex3f(1*xsize,  1*ysize,  0);
    
    glEnd();
}


// Draw just calls the display list we set up earlier.
void
Building::Draw(void)
{
    glPushMatrix();
    //Rotation and translation for different buildings
    glRotatef(75, 0, 0, 1);
    glTranslatef(20, 20, 0);
    glCallList(display_list);
    glPopMatrix();

    glPushMatrix();
    glRotatef(20, 0, 0, 1);
    glTranslatef(-20, 15, 0);
    glCallList(display_list+1);
    glPopMatrix();

    glPushMatrix();
    //Rotation and translation for different buildings
    glRotatef(30, 0, 0, 1);
    glTranslatef(-20, -30, 0);
    glCallList(display_list+2);
    glPopMatrix();

}


