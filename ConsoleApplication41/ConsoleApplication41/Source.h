#pragma once
#include "Header.h"


class MatrixStack {
	int    _index;
	int    _size;
	mat4*  _matrices;

public:
	MatrixStack(int numMatrices = 32) :_index(0), _size(numMatrices)
	{
		_matrices = new mat4[numMatrices];
	}

	~MatrixStack()
	{
		delete[]_matrices;
	}

	void push(const mat4& m) {
		assert(_index + 1 < _size);
		_matrices[_index++] = m;
	}

	mat4& pop(void) {
		assert(_index - 1 >= 0);
		_index--;
		return _matrices[_index];
	}
};


enum {
	sphere = 0,
	ellipse = 1,
	cylinder = 2,
	NumNodes,
	Quit
};

MatrixStack  mvstack;
mat4         model_view = mat4(1.0);
mat4         projmat = mat4(1.0);


/*                    SPHERE                  */
/*                    DRAW                    */

void paint_sphere(){

	glUseProgram(program[sphere]);
	position_of_light = vec4(-3 * sin(spin), 4, -3 * cos(spin), 1.0);

	// Initialize shader lighting parameters
	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
	color4 light_specular(1.0, 1.0, 1.0, 1.0);

	color4 material_ambient(1.000, 0.843, 0.000, 1.0);
	color4 material_diffuse(1.000, 0.843, 0.000, 1.0);
	color4 material_specular(1.000, 0.843, 0.000, 1.0);
	float  material_shininess = 100.0;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program[sphere], "AmbientProduct"),
		1, ambient_product);
	glUniform4fv(glGetUniformLocation(program[sphere], "DiffuseProduct"),
		1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program[sphere], "SpecularProduct"),
		1, specular_product);

	glUniform4fv(glGetUniformLocation(program[sphere], "LightPosition"),
		1, position_of_light);
	color4 light_ambient2(0.0, 0.0, 0.0, 0.5);
	color4 light_diffuse2(0.0, 0.0, 0.0, 0.5);
	color4 light_specular2(0.5, 0.5, 0.5, 0.5);

	ambient_product = light_ambient2 * material_ambient;
	diffuse_product = light_diffuse2 * material_diffuse;
	specular_product = light_specular2 * material_specular;

	glUniform4fv(glGetUniformLocation(program[sphere], "AmbientProduct2"),//-- Stationary Light color values
		1, ambient_product);
	glUniform4fv(glGetUniformLocation(program[sphere], "DiffuseProduct2"),
		1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program[sphere], "SpecularProduct2"),
		1, specular_product);

	light_ambient = (0.2, 0.2, 0.2, 0.3);// Camera light colors
	light_diffuse = (0.0, 0.0, 005, 0.3);
	light_specular = (0.5, 0.5, 0.5, 0.3);

	ambient_product = light_ambient * material_ambient;
	diffuse_product = light_diffuse * material_diffuse;
	specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program[sphere], "AmbientProduct3"),//Camera light color values
		1, ambient_product);
	glUniform4fv(glGetUniformLocation(program[sphere], "DiffuseProduct3"),
		1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program[sphere], "SpecularProduct3"),
		1, specular_product);



	glUniform4fv(glGetUniformLocation(program[sphere], "LightPosition"),//Spinning light position
		1, position_of_light);
	glUniform4fv(glGetUniformLocation(program[sphere], "LightPosition2"),//Stationary light position
		1, scene_light_pos);
	glUniform4fv(glGetUniformLocation(program[sphere], "LightPosition3"),//Camera Light Position
		1, eye - vec4(1.0, 1.0, 0, 0));

	glUniform1f(glGetUniformLocation(program[sphere], "Shininess"),
		material_shininess);

	Projection = glGetUniformLocation(program[sphere], "Projection");
	ModelView = glGetUniformLocation(program[sphere], "ModelView");
	glUniformMatrix4fv(Projection, 1, GL_TRUE, projmat);
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);

	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}


/*                    CYLINDER                  */
/*                    DRAW                      */
void paint_cylinder() {

	glUseProgram(program[cylinder]);
	// Initialize shader lighting parameters

	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
	color4 light_specular(1.0, 1.0, 1.0, 1.0);

	color4 material_ambient(0.412, 0.412, 0.412);
	color4 material_diffuse(0.412, 0.412, 0.412);
	color4 material_specular(0.412, 0.412, 0.412);
	float  material_shininess = 1000;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program[sphere], "AmbientProduct"),
		1, ambient_product);
	glUniform4fv(glGetUniformLocation(program[sphere], "DiffuseProduct"),
		1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program[sphere], "SpecularProduct"),
		1, specular_product);

	light_ambient = (0.2, 0.2, 0.2, 0.5);// Camera light colors
	light_diffuse = (0.5, 0.5, 0.5, 0.5);
	light_specular = (0.5, 0.5, 0.5, 0.5);

	ambient_product = light_ambient * material_ambient;
	diffuse_product = light_diffuse * material_diffuse;
	specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program[sphere], "AmbientProduct2"),//Camera light color values
		1, ambient_product);
	glUniform4fv(glGetUniformLocation(program[sphere], "DiffuseProduct2"),
		1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program[sphere], "SpecularProduct2"),
		1, specular_product);

	glUniform4fv(glGetUniformLocation(program[sphere], "LightPosition"),
		1, scene_light_pos);
	glUniform4fv(glGetUniformLocation(program[sphere], "LightPosition2"),//Camera position
		1, model_view*LookAt(eye, eye - n, v));

	glUniform1f(glGetUniformLocation(program[sphere], "Shininess"),
		material_shininess);

	Projection = glGetUniformLocation(program[cylinder], "Projection");
	ModelView = glGetUniformLocation(program[cylinder], "ModelView");
	glUniformMatrix4fv(Projection, 1, GL_TRUE, projmat);
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);

	glBindVertexArray(vao[2]);
	glDrawArrays(GL_TRIANGLES, 0, cylNumVertices);
}


/*                    ELLIPSE                  */
/*                    DRAW                    */
void paint_ellipse() {

	glUseProgram(program[ellipse]);

	// Initialize shader lighting parameters
	point4 position_of_light(0.0, 0.0, 2.0, 0.0);
	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
	color4 light_specular(1.0, 1.0, 1.0, 1.0);

	color4 material_ambient(0.502, 0.000, 0.000, 1.0);
	color4 material_diffuse(0.502, 0.000, 0.000, 1.0);
	color4 material_specular(0.502, 0.000, 0.000, 1.0);
	float  material_shininess = 500.0;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program[ellipse], "AmbientProduct"),
		1, ambient_product);
	glUniform4fv(glGetUniformLocation(program[ellipse], "DiffuseProduct"),
		1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program[ellipse], "SpecularProduct"),
		1, specular_product);

	glUniform4fv(glGetUniformLocation(program[ellipse], "AmbientProduct2"),//Camera Light color Values
		1, ambient_product);
	glUniform4fv(glGetUniformLocation(program[ellipse], "DiffuseProduct2"),
		1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program[ellipse], "SpecularProduct2"),
		1, specular_product);

	glUniform4fv(glGetUniformLocation(program[ellipse], "LightPosition"),// Stationary light Light
		1, position_of_light);

	glUniform4fv(glGetUniformLocation(program[ellipse], "LightPosition2"), 1, eye - vec4(1.0, 1.0, 0, 0));//camera position

	glUniform1f(glGetUniformLocation(program[ellipse], "Shininess"),
		material_shininess);

	Projection = glGetUniformLocation(program[ellipse], "Projection");
	ModelView = glGetUniformLocation(program[ellipse], "ModelView");
	glUniformMatrix4fv(Projection, 1, GL_TRUE, projmat);
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);

	glBindVertexArray(vao[1]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}
