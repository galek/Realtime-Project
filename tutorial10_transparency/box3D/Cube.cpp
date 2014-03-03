﻿// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

//for realtime project
#include "Cube.h"
#include <iostream>
// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GL/glfw.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

// Include AntTweakBar
#include <AntTweakBar.h>


#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/quaternion_utils.hpp> // See quaternion_utils.cpp for RotationBetweenVectors, LookAt and RotateTowards


using namespace glm;
using namespace std;
#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include "box3DglobalRule.cpp"



class Cube{
private:
	vec3 position;
	vec3 rotation;
	vec3 velocity;
	float inertia;
	float angularVelocity;
	vec4 quaternionRotation;
	float size;
	float mass;

	vec3 color;
	bool isRender;
	int begin_indices;
	int begin_indexed_vertices;
	int begin_indexed_uvs;
	int begin_indexed_normals;
	float p0[3];
	float p1[3];
	float p2[3];
	float p3[3];
	float p4[3];
	float p5[3];
	float p6[3];
	float p7[3];
public:
	Cube(vec3 cubePosition,vec3 cubeRotation,vec3 cubeVelocity,float cubeSize,float m){
		size=cubeSize;
		velocity=cubeVelocity;
		position=cubePosition;
		rotation=cubeRotation;
		mass=m;
		angularVelocity=1;
		//อนุรักษ์พลังงานกล ศักย์
		//อนุรักษ์โมเมนตัมเชิงมุม เส้น
		isRender=false;
		p0[0]=cubeVertex0.x*size; p0[1]=cubeVertex0.y*size; p0[2]=cubeVertex0.z*size;
		p1[0]=cubeVertex1.x*size; p1[1]=cubeVertex1.y*size; p1[2]=cubeVertex1.z*size;
		p2[0]=cubeVertex2.x*size; p2[1]=cubeVertex2.y*size; p2[2]=cubeVertex2.z*size;
		p3[0]=cubeVertex3.x*size; p3[1]=cubeVertex3.y*size; p3[2]=cubeVertex3.z*size;
		p4[0]=cubeVertex4.x*size; p4[1]=cubeVertex4.y*size; p4[2]=cubeVertex4.z*size;
		p5[0]=cubeVertex5.x*size; p5[1]=cubeVertex5.y*size; p5[2]=cubeVertex5.z*size;
		p6[0]=cubeVertex6.x*size; p6[1]=cubeVertex6.y*size; p6[2]=cubeVertex6.z*size;
		p7[0]=cubeVertex7.x*size; p7[1]=cubeVertex7.y*size; p7[2]=cubeVertex7.z*size;
	}
	void addForce(vec3 force,float size){
	}
	vec4 rotate(vec4 vertex){
		return vec4(0,0,0,0);
	}
	mat4 getRotationMatrix(){
		return eulerAngleYXZ(rotation.x,rotation.y,rotation.z);
	}
	mat4 getTranslationMatrix(){
		return mat4();
	}
	mat4 getScaleMatrix(){
		return mat4();
	}
	void renderCube(vec3 color){
		rotation.x+=0.01;
		rotation.y+=0.01;
		rotation.z+=0.01;
		glBegin(GL_QUADS);{
			//front
			glColor3f(0,0,1);
			glVertex3fv(p4);
			glVertex3fv(p5);
			glVertex3fv(p7);
			glVertex3fv(p6);
			//back
			glColor3f(1,1,0);
			glVertex3fv(p2);
			glVertex3fv(p3);
			glVertex3fv(p1);
			glVertex3fv(p0);
			//leaf
			glColor3f(0,1,0);
			glVertex3fv(p2);
			glVertex3fv(p6);
			glVertex3fv(p4);
			glVertex3fv(p0);
			//right
			glColor3f(1,0,1);
			glVertex3fv(p7);
			glVertex3fv(p3);
			glVertex3fv(p1);
			glVertex3fv(p5);
			//up
			glColor3f(1,0,0);
			glVertex3fv(p2);
			glVertex3fv(p3);
			glVertex3fv(p7);
			glVertex3fv(p6);
			//down
			glColor3f(0,1,1);
			glVertex3fv(p0);
			glVertex3fv(p1);
			glVertex3fv(p5);
			glVertex3fv(p4);
		}glEnd();
	}
	void derenderCube(std::vector<unsigned short> indices,
		std::vector<glm::vec3> indexed_vertices,
		std::vector<glm::vec2> indexed_uvs,
		std::vector<glm::vec3> indexed_normals){

	}
};