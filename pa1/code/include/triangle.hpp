#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <iostream>
using namespace std;

// TODO: implement this class and add more fields as necessary,
class Triangle: public Object3D {

public:
	Triangle() = delete;

    // a b c are three vertex positions of the triangle
	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m) : Object3D(m) {
		vertices[0]=a;
		vertices[1]=b;
		vertices[2]=c;
		Vector3f d=vertices[0]-vertices[1];
		Vector3f e=vertices[1]-vertices[2];
		normal=Vector3f(d[1]*e[2]-d[2]*e[1],d[2]*e[0]-d[0]*e[2],d[0]*e[1]-d[1]*e[0]);
	
	}

	bool intersect( const Ray& ray,  Hit& hit , float tmin) override {
		float t,beta,gamma;
		Vector3f E1=vertices[0]-vertices[1];
		Vector3f E2=vertices[0]-vertices[2];
		Vector3f S=vertices[0]-ray.getOrigin();
		float deno=Matrix3f(ray.getDirection(),E1,E2).determinant();
		t=Matrix3f(S,E1,E2).determinant()/deno;
		if(!deno)return false;
		beta=Matrix3f(ray.getDirection(),S,E2).determinant()/deno;
		gamma=Matrix3f(ray.getDirection(),E1,S).determinant()/deno;
		if(t>tmin&&t<hit.getT()&&beta>=0&&gamma>=0&&beta+gamma<=1){
			hit.set(t,material,normal);
			return true;
		}
        return false;
	}
	Vector3f normal;
	Vector3f vertices[3];
protected:

};

#endif //TRIANGLE_H
