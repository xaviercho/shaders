#pragma once
#include "Angel.h"
const int NumTimesToSubdivide = 5;
const int NumTriangles = 4096;  // (4 faces)^(NumTimesToSubdivide + 1)
const int NumVertices = 3 * NumTriangles;
int tri_index = 0;
int cylNumVertices = 600;

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

Angel::vec4 eye = vec4(0.0, 1.5, 3.0, 1.0);
Angel::vec4 v = vec4(0.0, 1.0, 0.0, 00);
Angel::vec4 u = vec4(1.0, 0.0, 0.0, 0.0);
Angel::vec4 n = Angel::normalize(vec4(0.0, 1.0, 2.0, 0.0));
#define SCALE_VECTOR    .1
#define SCALE_ANGLE     .7
#define X               0
#define Y               1
#define Z               2
#ifndef PI
#define PI  3.14159265358979323846
#endif

point4 points[NumVertices];
vec3   normals[NumVertices];

point4 cylinderData[600];
color4 cylinderColor[600];

// Model-view and Projection matrices uniform location
GLuint  ModelView, Projection, vPosition, vNormal, vColor;
GLuint program[3];
GLuint buffers[6];
GLuint vao[3];

point4 position_of_light;
point4 scene_light_pos;

//Spinning light: orbit parameterization
float spin = 0.0, spin_step = 0.001;
float  material_shininess;
color4 ambient_product;
color4 diffuse_product;
color4 specular_product;

void make_sphere(void);
void paint_sphere(void);
void paint_ellipse(void);
void make_cylinder(void);
void paint_cylinder(void);


