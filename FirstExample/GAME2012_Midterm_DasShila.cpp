﻿
///////////////////////////////////////////////////////////////////////
//
// GAME2012_A2_DasShila.cpp by Shila Das (C) 2020 All Rights Reserved.
//
// Assignment 2 submission.
//
// Description:
// Here I finished everything with bonus part
//Render the cube
//Render the second cube.
//Rotate each cube constantly in opposite directions
//Use “W” and “S” to move the camera in and out.
//Use “A”and “D” to move the camera leftand right.
//Use “R”and “F” to move the camera upand down.
//user to change the number of cubes to be rendered
//rendering a ground plane underneath the cubes
///////////////////////////////////////////////////////////////////////

using namespace std;

#include <cstdlib>
#include <ctime>
#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <iostream>
#include <array>
#define FPS 60
#define SPEED 0.25f
#define MOVESPEED 0.1f
#define TURNSPEED 1.0f
#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)
#define XY_AXIS glm::vec3(1,1,0)
#define YZ_AXIS glm::vec3(0,1,1)
#define XZ_AXIS glm::vec3(1,0,1)

enum keyMasks {
	KEY_FORWARD = 0b00000001, // 0x01 or   1	or   01
	KEY_BACKWARD = 0b00000010,  // 0x02 or   2	or   02
	KEY_LEFT = 0b00000100, // 0x01 or   4	
	KEY_RIGHT = 0b00001000,  // 0x02 or  8	
	KEY_UP = 0b00010000, // 0x01 or   16	
	KEY_DOWN = 0b00100000,  // 0x02 or   322	
	KEY_MOUSECLICKED = 0b00000100	// 0x04 or 4 or 04
	// Any other keys you want to add.
};
int deltaTime, currentTime, lastTime = 0;
GLuint vao, ibo, points_vbo, colours_vbo, mvp_ID;
glm::mat4 MVP, View, Projection;
const int MaxNumVertices = 500; // Number of vertices on circle.
static int numVertices = 5;
#define PI 3.14159265358979324
const int NumVertices =30; //(6 faces)(2 triangles/face)(3 vertices/triangle)
//top face/4 triangle, bottom 4 tri, side, 12 triangele
GLfloat cube_verticesP[NumVertices][3] = { 0 };
GLfloat cube_colorsP[NumVertices][3] = { 0 };

float theta = 0.0f;

// Globals.
static float R = 1.0; // Radius of circle.
static float X = 0.0; // X-coordinate of center of circle.
static float Y = 0.0; // Y-coordinate of center of circle.

std::array<glm::vec3, MaxNumVertices> vertices = {};
std::array<glm::vec3, MaxNumVertices> colors = {};
std::array<glm::vec3, 10> unique_vertices = {};

GLfloat shape_vertices[MaxNumVertices][3] = { 0 };
GLfloat shape_colors[MaxNumVertices][3] = { 0 };
int index = 0;
std::array<glm::vec3, 72> verticesN = {};



std::array<glm::vec3, NumVertices> verticesP = {};
std::array<glm::vec3, NumVertices> colorsP = {};

std::array<glm::vec3, 8> unique_verticesP = {
	glm::vec3(-0.5f, 0.9f, 0.0f),		// 4.
	

	glm::vec3(-0.5f, -0.9f, -0.5f),		// 6.
	glm::vec3(0.5f, -0.9f, -0.5f),		// 7.
	glm::vec3(0.9f, 0.0f, -0.5f),		// 8.
	glm::vec3(0.5f, 0.9f, -0.5f),		// 9.
	glm::vec3(-0.5f, 0.9f, -0.5f),		// 10.
	glm::vec3(-0.9f, 0.0f, -0.5f),		// 11.
	glm::vec3(-0.9f, 0.0f, -0.9f)		// 5.

};

std::array<glm::vec3, 8> unique_colorsP = {
	glm::vec3(0.0f, 1.0f, 0.0f),//0
glm::vec3(0.0f, 1.0f, 0.0f),//0
glm::vec3(0.0f, 1.0f, 0.0f),//0
	glm::vec3(0.0f, 1.0f, 0.0f),//0

glm::vec3(0.0f, 1.0f, 0.0f),//0
glm::vec3(0.0f, 1.0f, 0.0f),//0
glm::vec3(0.0f, 1.0f, 0.0f),//0
	glm::vec3(1.0f, 0.5f, 0.0f)//7

	
};
// Our bitflags. 1 byte for up to 8 keys.
unsigned char keys = 0; // Initialized to 0 or 0b00000000.
//part 2 2.b) In order to render using only 8 vertices, you need to use a index list.
// Cube data.
GLshort cube_indices[] = {
	// Front.
	0, 1, 2, 3,
	// Left.
	4, 5, 6, 7,			// 7, 4, 0, 3,
	// Bottom.
	8, 9, 10, 11,		// 0, 4, 5, 1,
	// Right.
	12, 13, 14, 15,		// 2, 1, 5, 6,
	// Back.
	16, 17, 18, 19,		// 5, 4, 7, 6,
	// Top.
	20, 21, 22, 23		// 2, 6, 7, 3
};

GLfloat cube_vertices[] = {
	-0.9f, -0.9f, 0.9f,		// 0.
	0.9f, -0.9f, 0.9f,		// 1.
	0.9f, 0.9f, 0.9f,		// 2.
	-0.9f, 0.9f, 0.9f,		// 3.

	-0.9f, 0.9f, -0.9f,		// 4. (copy of old 7)
	-0.9f, -0.9f, -0.9f,    // 5. (copy of old 4)
	-0.9f, -0.9f, 0.9f,		// 6. (copy of old 0)
	-0.9f, 0.9f, 0.9f,		// 7. (copy of old 3)

	-0.9f, -0.9f, 0.9f,		// 8. (copy of old 0)
	-0.9f, -0.9f, -0.9f,    // 9. (copy of old 4)
	0.9f, -0.9f, -0.9f,		// 10. (copy of old 5)
	0.9f, -0.9f, 0.9f,		// 11. (copy of old 1)

	0.9f, 0.9f, 0.9f,		// 12. (copy of old 2)
	0.9f, -0.9f, 0.9f,		// 13. (copy of old 1)
	0.9f, -0.9f, -0.9f,		// 14. (copy of old 5)
	0.9f, 0.9f, -0.9f,		// 15. (copy of old 6)

	0.9f, -0.9f, -0.9f,		// 16. (copy of old 5)
	-0.9f, -0.9f, -0.9f,    // 17. (copy of old 4)
	-0.9f, 0.9f, -0.9f,		// 18. (copy of old 7)
	0.9f, 0.9f, -0.9f,		// 19. (copy of old 6)

	0.9f, 0.9f, 0.9f,		// 20. (copy of old 2)
	0.9f, 0.9f, -0.9f,		// 21. (copy of old 6)
	-0.9f, 0.9f, -0.9f,		// 22. (copy of old 7)
	-0.9f, 0.9f, 0.9f		// 23. (copy of old 3)
};
//5.c)Change the color of the points/lines.
GLfloat cube_colours[] = {
	1.0f, 0.4f, 0.0f,		// 0.
	1.0f, 0.4f, 0.0f,		// 1.
	1.0f, 0.4f, 0.0f,		// 2.
	1.0f, 0.4f, 0.0f,		// 3.

	0.0f, 0.5f, 1.0f,		// 4.
	0.0f, 0.5f, 1.0f,		// 5.
	0.0f, 0.5f, 1.0f,		// 6.
	0.0f, 0.5f, 1.0f,		// 7.

	0.0f, 0.0f, 1.0f,		// 8.
	0.0f, 0.0f, 1.0f,		// 9.
	0.0f, 0.0f, 1.0f,		// 10.
	0.0f, 0.0f, 1.0f,		// 11.

	1.0f, 1.0f, 0.0f,		// 12.
	1.0f, 1.0f, 0.0f,		// 13.
	1.0f, 1.0f, 0.0f,		// 14.
	1.0f, 1.0f, 0.0f,		// 15.

	1.0f, 0.0f, 1.0f,		// 16.
	1.0f, 0.0f, 1.0f,		// 17.
	1.0f, 0.0f, 1.0f,		// 18.
	1.0f, 0.0f, 1.0f,		// 19.

	0.5f, 1.0f, 1.0f,		// 20.
	0.5f, 1.0f, 1.0f,		// 21.
	0.5f, 1.0f, 1.0f,		// 22.
	0.5f, 1.0f, 1.0f		// 23.
};

// Wireframe data.

GLshort wire_indices[] = {
	// Front.
	0, 1, 2, 3,
	// Left.
	7, 4, 0, 3,
	// Bottom.
	0, 4, 5, 1,
	// Right.
	2, 1, 5, 6,
	// Back.
	5, 4, 7, 6,
	// Top.
	2, 6, 7, 3
};

GLfloat wire_vertices[] = {
	-0.9f, -0.9f, 0.9f,		// 0.
	0.9f, -0.9f, 0.9f,		// 1.
	0.9f, 0.9f, 0.9f,		// 2.
	-0.9f, 0.9f, 0.9f,		// 3.
	-0.9f, -0.9f, -0.9f,	// 4.
	0.9f, -0.9f, -0.9f,		// 5.
	0.9f, 0.9f, -0.9f,		// 6.
	-0.9f, 0.9f, -0.9f,		// 7.
};
//5.c)Change the color of the points/lines.
GLfloat wire_colours[] = {
	1.0f, 0.0f, 1.0f,		// 0.
	1.0f, 0.0f, 1.0f,		// 1.
	1.0f, 0.0f, 1.0f,		// 2.
	1.0f, 0.0f, 1.0f,		// 3.
	1.0f, 0.0f, 1.0f,		// 4.
	1.0f, 0.0f, 1.0f,		// 5.
	1.0f, 0.0f, 1.0f,		// 6.
	1.0f, 0.0f, 1.0f		// 7.
};
//scale = 0.4f,
glm::vec3 scale = glm::vec3(0.4f, 0.4f, 0.4f);
float  angle1 = 0.0f, angle2 = 0.0f, zoom = 0.0f, bound = 5.0f;
glm::vec3 position, frontVec, worldUp, upVec, rightVec; // Set by function
GLfloat pitch, yaw;
int lastX, lastY;
int n;
float* ar;// = new float[n];
void timer(int);
void resetView()
{
	position = glm::vec3(0.0f, 5.0f, 5.0f);
	frontVec = glm::vec3(0.0f, 0.0f, -1.0f);
	worldUp = glm::vec3(0, 1, 0);
	pitch = 0.0f;
	yaw = 0.0f;
	//yaw = -90.0f;
	// View will now get set only in transformObject
}
void init(void)
{
	srand((unsigned)time(NULL));
	//Specifying the name of vertex and fragment shaders.
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	//Loading and compiling shaders
	GLuint program = LoadShaders(shaders);
	glUseProgram(program);	//My Pipeline is set up

	mvp_ID = glGetUniformLocation(program, "MVP");
	//part 2-1 setup your camera into Perspective mode.
	//a.Set the starting location of the camera somewhere on the Y - Z 		plane.
	//	b.The camera should be looking at the origin point(0, 0, 0).
	// Projection matrix : 45∞ Field of View, aspect ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::perspective(glm::radians(45.0f), 1.0f / 1.0f, 0.1f, 100.0f);
	//Projection = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f);
	// Or, for an ortho camera :
	//Projection = glm::ortho(-zoom + bound, zoom + bound, -zoom + bound, zoom + bound, 0.0f, 100.0f); // In world coordinates
	//The defaults in OpenGL are: the eye at (0, 0, 1); the center at (0, 0, 0) and the up is	given by the positive direction of the Oy axis(0, 1, 0).
	// Camera matrix
	View = glm::lookAt(
		glm::vec3(0, 0, 3),  // Camera is at (0,5,5), in World Space
		glm::vec3(0, 0, 0),	   // and looks at the origin
		glm::vec3(0, 1, 0)    // Head is up (set to 0,-1,0 to look upside-down)
	);

	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	ibo = 0;
	glGenBuffers(1, &ibo);
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wire_indices), wire_indices, GL_STATIC_DRAW);*/

	points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(wire_vertices), wire_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	colours_vbo = 0;
	glGenBuffers(1, &colours_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(wire_colours), wire_colours, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Can optionally unbind the buffer to avoid modification.

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

	// Enable depth test and face culling.
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	//Bonus 1. user to change the number of cubes to be rendered
	//cout << "No of wanted cube: ";
	//cin >> n;
	n = 1;
	//ar = new float[n];
	//for (int i = 0; i < n; i++)
	//{
	//	ar[i] = 0.0f;
	//}
	//5.a) Change the background color from black to a different color.
	glClearColor(0.3f, 0.4f, 0.5f, 255);
	timer(0);
}
//---------------------------------------------------------------------
//
// calculateView
//
void calculateView()
{
	frontVec.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontVec.y = sin(glm::radians(pitch));
	frontVec.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontVec = glm::normalize(frontVec);
	rightVec = glm::normalize(glm::cross(frontVec, worldUp));
	upVec = glm::normalize(glm::cross(rightVec, frontVec));
	//reset position = glm::vec3(0.0f, 5.0f, 5.0f);	frontVec = glm::vec3(0.0f, 0.0f, -1.0f);
	View = glm::lookAt(
		glm::vec3(0, 0, 3),
		//position + frontVec,  // Camera is at (x,y,z), in World Space 
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)); // Head is up (set to 0,-1,0 to look upside-down)
	/*View = glm::lookAt(
		position,
		position + frontVec,
		upVec);*/
}
//---------------------------------------------------------------------
//
// transformModel
//
//void transformObject(float scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation) {
void transformObject(glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation) {
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, glm::vec3(scale));

	// Update the projection.
	////Projection = glm::ortho(-zoom + -bound, zoom + bound, -zoom + -bound, zoom + bound, 0.0f, 100.0f);
	//Projection = glm::perspective(glm::radians(45.0f), 1.0f / 1.0f, 0.1f, 100.0f);
	//// Or, for an ortho camera :
	////Projection = glm::ortho(-zoom + bound, zoom + bound, -zoom + bound, zoom + bound, 0.0f, 100.0f); // In world coordinates
	////The defaults in OpenGL are: the eye at (0, 0, 1); the center at (0, 0, 0) and the up is	given by the positive direction of the Oy axis(0, 1, 0).
	//// Camera matrix
	//resetView();
	//View = glm::lookAt(
	//	glm::vec3(0, 10, 10),  // Camera is at (0,5,5), in World Space
	//	glm::vec3(0, 0, 0),	   // and looks at the origin
	//	glm::vec3(0, 1, 0)    // Head is up (set to 0,-1,0 to look upside-down)
	//);
	calculateView();
	MVP = Projection * View * Model;
	glUniformMatrix4fv(mvp_ID, 1, GL_FALSE, &MVP[0][0]);
}
int Index = 0;

void quad(int a, int b, int c, int d, int e, int f)
{
	colorsP[Index] = unique_colorsP[a];
	verticesP[Index] = unique_verticesP[a]; Index++;
	colorsP[Index] = unique_colorsP[a];
	verticesP[Index] = unique_verticesP[b]; Index++;
	colorsP[Index] = unique_colorsP[a];
	verticesP[Index] = unique_verticesP[c]; Index++;
	colorsP[Index] = unique_colorsP[a];
	verticesP[Index] = unique_verticesP[d]; Index++;
	colorsP[Index] = unique_colorsP[a];
	verticesP[Index] = unique_verticesP[e]; Index++;
	colorsP[Index] = unique_colorsP[a];
	verticesP[Index] = unique_verticesP[f]; Index++;

}
void colorcube()
{
	
	quad(1, 0, 2, 2, 0, 3);
	quad(3, 0, 4, 4, 0, 5);
	
	quad(5, 0, 1, 1, 6, 2);

	quad(2, 6, 3, 3, 6, 4);
	
	quad(4, 6, 5, 5, 6, 0);
	

	
	for (int i = 0; i < NumVertices; ++i) {

		cube_verticesP[i][0] = verticesP[i][0];
		cube_verticesP[i][1] = verticesP[i][1];
		cube_verticesP[i][2] = verticesP[i][2];

		cube_colorsP[i][0] = colorsP[i][0];
		cube_colorsP[i][1] = colorsP[i][1];
		cube_colorsP[i][2] = colorsP[i][2];
	}
	
	//Transform the crystal and make the entire scale 50% of the original size
	transformObject(glm::vec3(0.5f, 0.5f, 0.5f), Y_AXIS, angle2 += ((float)45 / (float)1000 * deltaTime), glm::vec3(0.0f, -0.45f, 0.0f));
}
//int Index = 0;
//void quad(int a, int b, int c, int d, int e, int f)
//{
//	//colors[index] = unique_colors[a];
//	verticesN[Index] = unique_vertices[a]; Index++;
//	//colors[Index] = unique_colors[a];
//	verticesN[Index] = unique_vertices[b]; Index++;
//	//colors[Index] = unique_colors[a];
//	verticesN[Index] = unique_vertices[c]; Index++;
//	//colors[Index] = unique_colors[a];
//	verticesN[Index] = unique_vertices[d]; Index++;
//	//colors[Index] = unique_colors[a];
//	verticesN[Index] = unique_vertices[e]; Index++;
//	//colors[Index] = unique_colors[a];
//	verticesN[Index] = unique_vertices[f]; Index++;
//
//}
void createModel(int n, float z)
{
	for (int i = 0; i < n; ++i)
	{

		vertices[i] = glm::vec3(X + R * cos(theta), Y + R * sin(theta), z);
		colors[i] = glm::vec3(1.0f, 1.0f, 0.0f);//glm::vec3((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
		unique_vertices[index][0] = vertices[i][0];
		unique_vertices[index][1] = vertices[i][1];
		unique_vertices[index][2] = vertices[i][2];
		theta += 2 * PI / n;
	}

	for (int i = 0; i < n; ++i) {

		shape_vertices[i][0] = vertices[i][0];
		shape_vertices[i][1] = vertices[i][1];
		shape_vertices[i][2] = vertices[i][2];
		
		index++;
		shape_colors[i][0] = colors[i][0];
		shape_colors[i][1] = colors[i][1];
		shape_colors[i][2] = colors[i][2];
	}
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape_vertices), shape_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape_colors), shape_colors, GL_STATIC_DRAW);

	//transformObject(0.4f, YZ_AXIS, rotAngle+=((float)45 / (float)1000 * deltaTime), glm::vec3(0.0f, 0.0f, 0.0f));
	transformObject(glm::vec3(0.4f,0.4f,0.4f), X_AXIS, angle1 += 1.0f, glm::vec3(0.0f, 0.0f, 0.4f));
	//Ordering the GPU to start the pipeline
	glDrawArrays(GL_TRIANGLE_FAN, 0, numVertices);
}
void drawCube(void)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colours), cube_colours, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Close the currently open buffer.

	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_SHORT, 0);
}

void drawWire(void)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wire_indices), wire_indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wire_vertices), wire_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wire_colours), wire_colours, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Close the currently open buffer.

	glLineWidth(4.0f); // Thicken up the rendered lines.
	glDrawElements(GL_LINE_LOOP, 24, GL_UNSIGNED_SHORT, 0);
}

//---------------------------------------------------------------------
//
// display
//
int dir = 1;


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Delta time stuff.
	currentTime = glutGet(GLUT_ELAPSED_TIME); // Gets elapsed time in milliseconds.
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	glBindVertexArray(vao);
	
	createModel(5,0.9f);
	//draw quad
	//quad(0,5,1,1,6,5);
	createModel(5,0.4f);
	//Transform the prism and make the XZ scale 150% of the original size and the Y scale 25% of the original size so it looks more like a platform.
	transformObject(glm::vec3(1.5f,0.25f,1.5f), XY_AXIS, angle2 -= 1.0f, glm::vec3(0.0f, -0.45f, 0.0f));
	index = 0;
	
	//reset direction
	dir = 1;

	
	//colorcube();
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_verticesP), cube_verticesP, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colorsP), cube_colorsP, GL_STATIC_DRAW);
									  
    //transformObject(0.2f, YZ_AXIS, rotAngle += ((float)45 / (float)1000 * deltaTime), glm::vec3(0.0f, 0.0f, 0.0f));

	glDrawArrays(GL_TRIANGLES, 0, NumVertices); // Try GL_LINE_STRIP too!
	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

	glutSwapBuffers(); // Now for a potentially smoother render.
}

//void idle()
//{
//	glutPostRedisplay(); // Tells glut to redisplay the current window.
//}

void parseKeys()
{
	if (keys & KEY_FORWARD)
		position.x += SPEED;
	if (keys & KEY_BACKWARD)
		position.x -= SPEED;
	if (keys & KEY_UP)
		position.y -= SPEED;
	if (keys & KEY_DOWN)
		position.y += SPEED;
	if (keys & KEY_LEFT)
		position.z += SPEED;
	if (keys & KEY_RIGHT)
		position.z -= SPEED;
}

void timer(int) { // essentially our update()
	parseKeys();
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, timer, 0); // 60 FPS or 16.67ms.
}

//---------------------------------------------------------------------
//
// keyDown
//
void keyDown(unsigned char key, int x, int y) // x and y is mouse location upon key press.
{
	if (key == 'w')
	{
		if (!(keys & KEY_FORWARD))
			keys |= KEY_FORWARD; // keys = keys | KEY_FORWARD
	}
	else if (key == 's')
	{
		if (!(keys & KEY_BACKWARD))
			keys |= KEY_BACKWARD;
	}
	if (key == 'r')
	{
		if (!(keys & KEY_UP))
			keys |= KEY_UP; // keys = keys | KEY_FORWARD
	}
	else if (key == 'f')
	{
		if (!(keys & KEY_DOWN))
			keys |= KEY_DOWN;
	}
	if (key == 'a')
	{
		if (!(keys & KEY_LEFT))
			keys |= KEY_LEFT; // keys = keys | KEY_FORWARD
	}
	else if (key == 'd')
	{
		if (!(keys & KEY_RIGHT))
			keys |= KEY_RIGHT;
	}
}

void keyDownSpec(int key, int x, int y) // x and y is mouse location upon key press.
{
	if (key == GLUT_KEY_UP)
	{
		if (!(keys & KEY_FORWARD))
			keys |= KEY_FORWARD;
	}
	else if (key == GLUT_KEY_DOWN)
	{
		if (!(keys & KEY_BACKWARD))
			keys |= KEY_BACKWARD;
	}
	if (key == GLUT_KEY_UP)
	{
		if (!(keys & KEY_UP))
			keys |= KEY_UP;
	}
	else if (key == GLUT_KEY_DOWN)
	{
		if (!(keys & KEY_DOWN))
			keys |= KEY_DOWN;
	}
	if (key == GLUT_KEY_UP)
	{
		if (!(keys & KEY_LEFT))
			keys |= KEY_LEFT;
	}
	else if (key == GLUT_KEY_DOWN)
	{
		if (!(keys & KEY_RIGHT))
			keys |= KEY_RIGHT;
	}

}

void keyUp(unsigned char key, int x, int y) // x and y is mouse location upon key press.
{
	if (key == 'w')
	{
		keys &= ~KEY_FORWARD; // keys = keys & ~KEY_FORWARD
	}
	else if (key == 's')
	{
		keys &= ~KEY_BACKWARD;
	}
	else if (key == 'a')
	{
		keys &= ~KEY_LEFT; // keys = keys & ~KEY_FORWARD
	}
	else if (key == 'd')
	{
		keys &= ~KEY_RIGHT;
	}
	else if (key == 'r')
	{
		keys &= ~KEY_UP; // keys = keys & ~KEY_FORWARD
	}
	else if (key == 'f')
	{
		keys &= ~KEY_DOWN;
	}
	else if (key == ' ')
	{
		resetView();
	}
}

void keyUpSpec(int key, int x, int y) // x and y is mouse location upon key press.
{
	if (key == GLUT_KEY_UP)
	{
		keys &= ~KEY_FORWARD;
	}
	else if (key == GLUT_KEY_DOWN)
	{
		keys &= ~KEY_BACKWARD;
	}
	if (key == GLUT_KEY_UP)
	{
		keys &= ~KEY_LEFT;
	}
	else if (key == GLUT_KEY_DOWN)
	{
		keys &= ~KEY_DOWN;
	}
	if (key == GLUT_KEY_UP)
	{
		keys &= ~KEY_UP;
	}
	else if (key == GLUT_KEY_DOWN)
	{
		keys &= ~KEY_DOWN;
	}

}

void mouseMove(int x, int y)
{
	if (keys & KEY_MOUSECLICKED)
	{
		pitch += (GLfloat)((y - lastY) * 0.1);
		yaw -= (GLfloat)((x - lastX) * 0.1);
		lastY = y;
		lastX = x;
	}
}

void mouseClick(int btn, int state, int x, int y)
{
	if (state == 0)
	{
		lastX = x;
		lastY = y;
		keys |= KEY_MOUSECLICKED;
		glutSetCursor(GLUT_CURSOR_NONE);
		//cout << "Mouse clicked." << endl;
	}
	else
	{
		keys &= ~KEY_MOUSECLICKED;
		glutSetCursor(GLUT_CURSOR_INHERIT);
		//cout << "Mouse released." << endl;
	}
}

//---------------------------------------------------------------------
//
// main
//

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutSetOption(GLUT_MULTISAMPLE, 8);
	//5.b)Change the window size to a different size.
	glutInitWindowSize(900, 900);
	glutCreateWindow("GAME2012 - Midterm_ Das_shila");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyDown);
	glutSpecialFunc(keyDownSpec);
	glutKeyboardUpFunc(keyUp); // New function for third example.
	glutSpecialUpFunc(keyUpSpec);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove); // Requires click to register.
	//glutIdleFunc(idle); // We cannot use this to set the framerate, but we can set a callback to run when the window receives no events.

	glutMainLoop();

	return 0;
}
