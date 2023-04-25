#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// TODO: Implement Plane representing an infinite plane
// function: ax+by+cz=d
// choose your representation , add more fields and fill in the functions

class Plane : public Object3D {
public:
    Plane() {
        n=Vector3f(0);
        dd=0;
    }

    Plane(const Vector3f &normal, float d, Material *m) : Object3D(m) {
        n=normal;
        dd=d;
    }

    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        float t;
        if(Vector3f::dot(n,r.getDirection()))
        t=-(-dd+Vector3f::dot(r.getOrigin(),n))/Vector3f::dot(n,r.getDirection());
        else return false;
        if(t>tmin&&t<h.getT()){
            h.set(t,material,n);
            return true;
        }else
        return false;
    }

protected:

Vector3f n;
float dd;

};

#endif //PLANE_H
		

