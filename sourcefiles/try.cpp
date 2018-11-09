 // A majority of the code was obtained from shaded sphere under week 8 
 /*****

 Fragment shading of a sphere
 student : Xavier Cho
 professor: George Kamberov
 Assignment 4

 Code base: Angel 5th Edition Chapters 5.6-5.10
 OGL Compatability: OGL3.1 GLSL 1.5

 Shaders: vshader56.glsl, fshader56.glsl

 Lighting Scenario: distant lights moving independently of the camera

 Interaction:

 Up/down arrow key     = Camera Slide forward/backward
 Left/right arrow key  = Camera Slide left/right

 ***/

#include "Angel.h"
#include <assert.h>
#include "Source.h"


/*                    SPHERE                  */
/*                    FUNCS                   */
void idle(void) {

	//Spinning light orbit:
	spin += spin_step;

	glutPostRedisplay();
}

void
triangle(const point4& a, const point4& b, const point4& c){
	
	vec3  normal = normalize(cross(b - a, c - b));

	normals[tri_index] = normalize(vec3(a[0], a[1], a[2]));  points[tri_index] = a;  tri_index++;
	normals[tri_index] = normalize(vec3(b[0], b[1], b[2]));  points[tri_index] = b;  tri_index++;
	normals[tri_index] = normalize(vec3(c[0], c[1], c[2]));  points[tri_index] = c;  tri_index++;
}

point4 unit(const point4& p){
	float len = p.x*p.x + p.y*p.y + p.z*p.z;

	point4 t;
	if (len > DivideByZeroTolerance) {
		t = p / sqrt(len);
		t.w = 1.0;
	}

	return t;
}

void
divide_triangle(const point4& a, const point4& b,
	const point4& c, int count)
{
	if (count > 0) {
		point4 v1 = unit(a + b);
		point4 v2 = unit(a + c);
		point4 v3 = unit(b + c);
		divide_triangle(a, v1, v2, count - 1);
		divide_triangle(c, v2, v3, count - 1);
		divide_triangle(b, v3, v1, count - 1);
		divide_triangle(v1, v3, v2, count - 1);
	}
	else {
		triangle(a, b, c);
	}
}


void
tetrahedron(int count)
{
	point4 v[4] = {
		vec4(0.0, 0.0, 1.0, 1.0),
		vec4(0.0, 0.942809, -0.333333, 1.0),
		vec4(-0.816497, -0.471405, -0.333333, 1.0),
		vec4(0.816497, -0.471405, -0.333333, 1.0)
	};

	divide_triangle(v[0], v[1], v[2], count);
	divide_triangle(v[3], v[2], v[1], count);
	divide_triangle(v[0], v[3], v[1], count);
	divide_triangle(v[0], v[2], v[3], count);
}


/*                    NODE                    */
/*                    FUNCS                   */
struct Node {
	mat4  transform;
	void(*render)(void);
	Node* sibling;
	Node* child;

	Node() :
		render(nullptr), sibling(nullptr), child(nullptr) {}

	Node(mat4& m, void(*render)(void), Node* sibling, Node* child) :
		transform(m), render(render), sibling(sibling), child(child) {}
};

Node  nodes[NumNodes];


void initNodes(void)
{
	mat4  m;

	m = Translate(-1.5, 0.7, -1.0)*Scale(0.75,0.75,0.75);
	nodes[sphere] = Node(m, paint_sphere, &nodes[ellipse], nullptr);

	m = Translate(-1, -1.2, -.5)*Scale(0.35, 1.0, 0.75);
	nodes[ellipse] = Node(m,paint_ellipse, &nodes[cylinder], nullptr);

	m = Translate(1, 0.7, .5)*Scale(0.75, 0.75, 0.75);
	nodes[cylinder] = Node(m, paint_cylinder, nullptr, nullptr);


}


void traverse(Node* node)
{
	if (node == nullptr) { return; }

	mvstack.push(model_view);

	model_view *= node->transform;
	node->render();

	if (node->child) { traverse(node->child); }

	model_view = mvstack.pop();

	if (node->sibling) { traverse(node->sibling); }
}

/*                    SPHERE                  */
/*                    BUILD                   */

void make_sphere()
{
	tetrahedron(NumTimesToSubdivide);
}

void make_cylinder()
{
	{
		GLfloat x, z, theta;
		int iMax = 50, vertexCounter = 0;
		// cylinder top
		for (int i = 0; i < iMax; i++) {
			theta = (2 * PI / (float)iMax)*i; x = cos(theta); z = sin(theta);
			cylinderData[vertexCounter] = vec4(0, 1, 0, 1.0); cylinderColor[vertexCounter] = vec4(0.5, 0.5, 0.5, 1.0);
			vertexCounter++;
			cylinderData[vertexCounter] = vec4(x, 1, z, 1.0); cylinderColor[vertexCounter] = vec4(0.5, 0.5, 0.5, 1.0);
			vertexCounter++;

			theta = (2 * PI / (float)iMax)*(i + 1.0); x = cos(theta); z = sin(theta);
			cylinderData[vertexCounter] = vec4(x, 1, z, 1.0); cylinderColor[vertexCounter] = vec4(0.5, 0.5, 0.5, 1.0);
			vertexCounter++;
		}
		

		//cylinder sides

		for (int i = 0; i < iMax; i++) {
			theta = (2 * PI / (float)iMax)*i; x = cos(theta); z = sin(theta);
			cylinderData[vertexCounter] = vec4(x, 1, z, 1.0); cylinderColor[vertexCounter] = vec4(0.663, 0.663, 0.663, 0.5);
			vertexCounter++;
			cylinderData[vertexCounter] = vec4(x, 0, z, 1.0); cylinderColor[vertexCounter] = vec4(0.663, 0.663, 0.663, 0.5);
			vertexCounter++;

			theta = (2 * PI / (float)iMax)*(i + 1.0); x = cos(theta); z = sin(theta);
			cylinderData[vertexCounter] = vec4(x, 1, z, 1.0); cylinderColor[vertexCounter] = vec4(0.663, 0.663, 0.663, 0.5);
			vertexCounter++;
			cylinderData[vertexCounter] = vec4(x, 1, z, 1.0); cylinderColor[vertexCounter] = vec4(0.663, 0.663, 0.663, 0.5);
			vertexCounter++;

			theta = (2 * PI / (float)iMax)*i; x = cos(theta); z = sin(theta);
			cylinderData[vertexCounter] = vec4(x, 0, z, 1.0); cylinderColor[vertexCounter] = vec4(0.663, 0.663, 0.663, 0.5);
			vertexCounter++;

			theta = (2 * PI / (float)iMax)*(i + 1.0); x = cos(theta); z = sin(theta);
			cylinderData[vertexCounter] = vec4(x, 0, z, 1.0); cylinderColor[vertexCounter] = vec4(0.663, 0.663, 0.663, 0.5);
			vertexCounter++;

		}


		//cylinder bottom
		for (int i = 0; i < iMax; i++) {
			theta = (2 * PI / (float)iMax)*i; x = cos(theta); z = sin(theta);
			cylinderData[vertexCounter] = vec4(0, 0, 0, 1.0); cylinderColor[vertexCounter] = vec4(0.5, 0.5, 0.5, 1.0);
			vertexCounter++;
			cylinderData[vertexCounter] = vec4(x, 0, z, 1.0); cylinderColor[vertexCounter] = vec4(0.5, 0.5, 0.5, 1.0);
			vertexCounter++;
			theta = (2 * PI / (float)iMax)*(i + 1.0); x = cos(theta); z = sin(theta);
			cylinderData[vertexCounter] = vec4(x, 0, z, 1.0); cylinderColor[vertexCounter] = vec4(0.5, 0.5, 0.5, 1.0);
			vertexCounter++;
		}
	}
}


/*                    KEYBOARD                  */
/*                    STUFF                    */

void keyboard(unsigned char key, int x, int y) {
	GLfloat xt, yt, zt;
	GLfloat cosine, sine;

	//prees  spacebar to  return to default 
	if (key == 32)
	{
		v = vec4(0.0, 1.0, 0.0, 00);
		u = vec4(1.0, 0.0, 0.0, 0.0);
		eye = vec4(0.0, 1.5, 3.0, 1.0);
		n = Angel::normalize(vec4(0.0, 1.0, 2.0, 0.0));
	}

	// positive or negative rotation
	if (key > 96)
	{
		cosine = cos(SCALE_ANGLE * PI / -180.0);
		sine = sin(SCALE_ANGLE * PI / -180.0);
	}
	else
	{
		cosine = cos(SCALE_ANGLE * PI / 180.0);
		sine = sin(SCALE_ANGLE * PI / 180.0);
	}

	switch (key)
	{
	case 'Z': // roll counterclockwise in the xy plane
	case 'z': // roll clockwise in the xy plane
		xt = u[X];
		yt = u[Y];
		zt = u[Z];
		u[X] = xt*cosine - v[X] * sine;
		u[Y] = yt*cosine - v[Y] * sine;
		u[Z] = zt*cosine - v[Z] * sine;
		v[X] = xt*sine + v[X] * cosine;
		v[Y] = yt*sine + v[Y] * cosine;
		v[Z] = zt*sine + v[Z] * cosine;
		break;////////


	case 'y': // pitch up
	case 'Y': // pitch down
		xt = v[X];
		yt = v[Y];
		zt = v[Z];
		v[X] = cosine*xt - sine*n[X];
		v[Y] = cosine*yt - sine*n[Y];
		v[Z] = cosine*zt - sine*n[Z];
		n[X] = sine*xt + cosine*n[X];
		n[Y] = sine*yt + cosine*n[Y];
		n[Z] = sine*zt + cosine*n[Z];
		break;

	case 'x': // pitch up
	case 'X': // pitch down
		xt = u[X];
		yt = u[Y];
		zt = u[Z];

		u[X] = cosine*xt - sine*n[X];
		u[Y] = cosine*yt - sine*n[Y];
		u[Z] = cosine*zt - sine*n[Z];
		n[X] = sine*xt + cosine*n[X];
		n[Y] = sine*yt + cosine*n[Y];
		n[Z] = sine*zt + cosine*n[Z];
		break;
		break;
	case 033:  // Escape key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
	}

	glutPostRedisplay();
}

void specKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP: // MOVE FORWARD
		eye -= 0.1*n;
		break;
	case GLUT_KEY_DOWN:  // MOVE WARD
		eye += 0.1*n;
		break;
	case GLUT_KEY_LEFT: // MOVE LEFT
		eye -= 0.1*u;
		break;
	case GLUT_KEY_RIGHT: // MOVE RIGHT
		eye += 0.1*u;
		break;
	}
}

void
reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	GLfloat left = -2.0, right = 2.0;
	GLfloat top = 2.0, bottom = -2.0;
	GLfloat zNear = -20.0, zFar = 20.0;

	GLfloat aspect = GLfloat(width) / height;

	if (aspect > 1.0) {
		left *= aspect;
		right *= aspect;
	}
	else {
		top /= aspect;
		bottom /= aspect;
	}

	mat4 projection = Ortho(left, right, bottom, top, zNear, zFar);
	glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
}


void init() {

	glGenVertexArrays(3, vao);
	glGenBuffers(6, buffers);

	/*                    init                      */
	/*                    sphere                    */

	make_sphere();
	
	program[sphere] = InitShader("vshader56.glsl", "fshader56.glsl");///"vshader56.glsl", "fshader56.glsl"

	glBindVertexArray(vao[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	vPosition = glGetAttribLocation(program[sphere], "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(nullptr));

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	vNormal = glGetAttribLocation(program[sphere], "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(nullptr));


	/*                    init                      */
	/*                    cylinder                   */
	make_cylinder();

	program[cylinder] = InitShader("vshader56.glsl", "fshader56.glsl");

	glBindVertexArray(vao[2]);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cylinderData), cylinderData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cylinderData), cylinderData, GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, buffers[4]);
	vPosition = glGetAttribLocation(program[cylinder], "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(nullptr));

	glBindBuffer(GL_ARRAY_BUFFER, buffers[5]);
	vNormal = glGetAttribLocation(program[cylinder], "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(nullptr));


	/*                    init                      */
	/*                    ellipse                    */

	program[ellipse] = InitShader("vshader56.glsl", "fshader56.glsl");
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	vPosition = glGetAttribLocation(program[ellipse], "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(nullptr));

	glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
	vNormal = glGetAttribLocation(program[sphere], "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(nullptr));

	glBindVertexArray(0);
	glUseProgram(0);
	
	initNodes();

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0);
}


void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	projmat = Perspective(60, 1.0, 1.0, 40.0); //change back to 90

	model_view = LookAt(eye, eye - n, v);

	scene_light_pos = vec4(10, 10, 15, 1.0);

	traverse(&nodes[0]);

	glutSwapBuffers();
}


int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutCreateWindow("ALL OF THE LIGHTS");

	glewInit();
	init();
	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specKey);

	glutMainLoop();
	return 0;
}