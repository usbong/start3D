/*
 * Copyright 2020~2022 SYSON, MICHAEL B.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 *     
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @company: USBONG
 * @author: SYSON, MICHAEL B.
 * @date created: 20201013
 * @date updated: 20220525; from 20201017
 * @website address: http://www.usbong.ph
 *   
 * Acknowledgments:
 * 1) "Bulalakaw Wars" Team (2007): 
 * Syson, M., Camacho, R., Gonzales, D., Del Rosario, R., Vidal, E., et al.
 *
 */

#include <windows.h> //Windows Machine

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
 
#include <stdio.h>
#include <math.h>

#include "Beam.h"
//removed by Mike, 20201013
//#include "PolygonUtils.h"

//edited by Mike, 20201013
//#include <string.h>
#include <string>

//#include "MyDynamicObject.h"
//#include "ModelPool.h"

typedef struct
{
    GLubyte id_field_length;
    GLubyte color_map_type;
    GLubyte image_type_code;
    GLubyte ignore[9];
    GLushort width;
    GLushort height;
    GLubyte image_pixel_size;
    GLubyte image_descriptor;
} TARGA_HEADER;

GLboolean Beam::test_pow2(GLushort i)
{
    while (i % 2 == 0)
        i /= 2;
    if (i == 1)
        return GL_TRUE;
    else
        return GL_FALSE;
}

void Beam::load_tga(char *filename)
{
    TARGA_HEADER targa;
    FILE *file;
    GLubyte *data;
    GLuint i;

    file = fopen(filename, "rb");
    if (file == NULL)
        return;

    /* test validity of targa file */
    if (fread(&targa, 1, sizeof(targa), file) != sizeof(targa) ||
        targa.id_field_length != 0 || targa.color_map_type != 0 ||
        targa.image_type_code != 2 || ! test_pow2(targa.width) ||
        ! test_pow2(targa.height) || targa.image_pixel_size != 32 ||
        targa.image_descriptor != 8)
    {
        fclose(file);
        free(data);
        return;
    }

    /* read targa file into memory */
    data = (GLubyte *) malloc(targa.width * targa.height * 4);
    if (fread(data, targa.width * targa.height * 4, 1, file) != 1)
    {
        fclose(file);
        return;
    }

    /* swap texture bytes so that BGRA becomes RGBA */
    for (i = 0; i < targa.width * targa.height * 4; i += 4)
    {
        GLbyte swap = data[i];
        data[i] = data[i + 2];
        data[i + 2] = swap;
    }

    /* build OpenGL texture */
    fclose(file);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, targa.width, targa.height,
                      GL_RGBA, GL_UNSIGNED_BYTE, data);
    free(data);
}

Beam::Beam(): MyDynamicObject(0,0,300)
{ 
    currentState=HIDDEN_STATE;
//    myXPos=0.0;
//    myYPos=0.0;
    //myYPos=300.0;
//    myZPos=300.0;

	//edited by Mike, 20201013
/*    
    myWidth=2.0;
    myHeight=2.0;
*/
    myWidth=0.2;
    myHeight=0.2;

    stepX=0.01;
    stepY=0.01;
    stepZ=0.01;

	//edited by Mike, 20201013    
/*    myWidthX=0.5;
*/
    myWidthX=0.025;

    rotationAngle=0.0f;//360.0f;//90.0;
    rotationStep=10.0;//1.0f;    
/*	//edited by Mike, 20201013
    thrust=2.0f;
    thrustMax=0.5f;
*/
    thrust=0.02f;
    thrustMax=0.05f;

    xVel;
    yVel;
    
/*	//edited by Mike, 20201013
    maxXVel=1.0f;
    maxYVel=1.0f;
*/
    maxXVel=0.01f;
    maxYVel=0.01f;

    //boundary = 8.0f;

    tricount = 120;
    isMovingForward = 0;

    initializeCollisionSpheres(1);
    addSphere(0.0f, 0.0f, 0.0f, 0.5f);
    setCollidable(true);

    //init default values
    //previousFacingState=FACING_UP;
//    currentFacingState=FACING_UP;
	
	//loadTexture(myBodyTexture, "bodyTexture.tga", &myBodyTextureObject);
	//loadTexture(myHeadTexture, "headTexture.tga", &myHeadTextureObject);
	//setup();
}

Beam::~Beam()
{
}

void Beam::move(float rot, float* xyz)
{	
     //fprintf(stdout, "%f", rot);
     //fprintf(stderr, "testing");
     rotationAngle = rot;
     myXPos = xyz[0];
     myYPos = xyz[1];
     myZPos = xyz[2];
     
     changeState(ACTIVE_STATE);
}

void Beam::draw()
{
    glPushMatrix();  
    glTranslatef(myXPos, myYPos, myZPos);    

    switch (currentState)
    {
            case ACTIVE_STATE:
               glColor3f(1.0f, 1.0f, 0.0f); //yellow
           	   glPushMatrix();	
/*
                   if (rotationAngle>360)
                     rotationAngle-=360;
                   else if (rotationAngle<-360)
                     rotationAngle+=360;
*/
                   //since the model ship faces the top, do this...
                   //glRotatef(-90, 1.0f, 0.0f, 0.0f);
                   //since the model ship is facing the opposite direction (in terms of Z)...
                   //glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
                   //since the model ship is too big, do this...
					//edited by Mike, 20201013
//                   glScalef(0.1f, 0.1f, 0.1f);

                   /*	//edited by Mike, 20201013
                   		//note: uses PolygonUtils.cpp
				   		drawSphere();
                   */
                	drawBeam();
                	
           	   glPopMatrix();	// pop back to original coordinate system
               break;
    }    
    glPopMatrix();	// pop back to original coordinate system
}

//added by Mike, 20201013
void Beam::drawBeam()
{
	//added by Mike, 20201002; edited by Mike, 202010015
	//to make anchor/origin/reference point start at top-left
//    glTranslatef(0.0f, 0.1f, 0.0f);   

	//reference point at center
    glTranslatef(0.1f/2, 0.1f/2, 0.0f);   
	
	//Reference: https://www3.ntu.edu.sg/home/ehchua/programming/opengl/HowTo_OpenGL_C.html;
	//last accessed: 20200928 
   //note: 0.5f = half of the window width or height

   //TO-DO: -reverify: vertices due to 0.01 not centered at origin
   // Draw a Red 1x1 Square centered at origin
   //TO-DO: -update: this due to output is 1 x 2 box, width x height
   glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
/*    //edited by Mike, 20201001  
	  glVertex2f(-0.1f, -0.1f);    // x, y
      glVertex2f( 0.1f, -0.1f);
      glVertex2f( 0.1f,  0.1f);
      glVertex2f(-0.1f,  0.1f);
*/      
/*
	  //1x1 box
	  glVertex2f(0.0f, 0.0f);    // x, y
      glVertex2f( 0.0f, -0.1f);
      glVertex2f( 0.1f,  -0.1f);
      glVertex2f(0.1f,  0.0f);
*/
	  glVertex2f(0.0f, 0.0f);    // x, y
      glVertex2f( 0.0f, -0.01f);
      glVertex2f( 0.01f,  -0.01f);
      glVertex2f(0.01f,  0.0f);
      
   glEnd();    
}


void Beam::update(float dt)
{
    switch (currentState)
    {
           case ACTIVE_STATE:      
                    //thrust+=0.01f;


                    //rotationAngle in degrees, convert to radians 
                    //im not yet sure why, but i have to deduct 90 to rotationAngle
                    rotationAngleRad = (rotationAngle) * 3.141593f / 180.0f;
                    
                    yAccel = (cos(rotationAngleRad)*thrust);
                    xAccel = -(sin(rotationAngleRad)*thrust);

					//edited by Mike, 20201014
/*                    xVel=xAccel;
                    yVel=yAccel;
*/
/*					//edited by Mike, 20201015
					//10x10 grid
                    xVel=xAccel*2;
                    yVel=yAccel*2;
*/
					//20x20 grid
                    xVel=xAccel*4;
                    yVel=yAccel*4;

                    myXPos+=xVel;
                    myYPos+=yVel;

/*                    
                    if (thrust>0)
                      thrust-=0.01f;
                    else thrust=0;
*/
               		//wrap the world 
                    //note +/- 10.5f is the boundary 
                    //edited by Mike, 20201013
/*
               		if ((myXPos <= -25.0f) || (myXPos > 25.0f)) changeState(HIDDEN_STATE); 
               		if ((myYPos <= -21.0f) || (myYPos > 21.0f)) changeState(HIDDEN_STATE);
*/

/*					//edited by Mike, 20201015
	           		if (myXPos <= 0.0f) changeState(HIDDEN_STATE); //if left side
	           		else if (myXPos >= 1.0f) changeState(HIDDEN_STATE); //if right side
	
	           		if (myYPos >= 1.0f) changeState(HIDDEN_STATE); //if bottom side
	           		else if (myYPos <= 0.0f) changeState(HIDDEN_STATE); //if top side
*/               		
					//TO-DO: -add: 0.1f*iColumnCountMax
	           		if (myXPos <= 0.0f) changeState(HIDDEN_STATE); //if left side
	           		else if (myXPos >= 0.1f*20) changeState(HIDDEN_STATE); //if right side
	
					//TO-DO: -add: 0.1f*iRowCountMax
	           		if (myYPos >= 0.1f*20) changeState(HIDDEN_STATE); //if bottom side
	           		else if (myYPos <= 0.0f) changeState(HIDDEN_STATE); //if top side

                break;
            default: //STANDING STATE
              break;//do nothing    
    }
} 

void Beam::changeState(int s)
{
  currentState=s;                  
}

void Beam::hitBy(MyDynamicObject* mdo)
{
     changeState(HIDDEN_STATE);
}

//added by Mike, 20201016
void Beam::destroy()
{
/*	
	for(int i = 0; i < MAX_EXPLOSION_PARTICLES; ++i) {
	    delete [] explosionParticle[i];
	}
	delete [] explosionParticle;
*/
}

