#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// TODO: Implement functions and add more fields as necessary

class Sphere : public Object3D {
public:
    Sphere() {
        // unit ball at the center
        c=Vector3f(0);
        R=1;//useful?
    }

    Sphere(const Vector3f &center, float radius, Material *material) : Object3D(material) {
        // 
        c=center;
        R=radius;
    }

    ~Sphere() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        //
        Vector3f dir= r.getDirection().normalized();
        
        Vector3f l=c-r.getOrigin();
        float tp=Vector3f::dot(l,dir);
        float d2=l.squaredLength()-tp*tp;
        if(l.squaredLength()<R*R){
            float t=tp+sqrt(R*R-d2);
            if(t>tmin&&t<h.getT()){
                Vector3f n=l-dir*t;
                n=n.normalized();
                h.set(t,material,n);
                return true;
            }
            return false;
        }
        else if(l.squaredLength()>R*R){
            if(tp<0)return false;
            if(d2>R*R)return false;
            float t=tp-sqrt(R*R-d2);
            if(t>tmin&&t<h.getT()){
                Vector3f n=dir*t-l;
                n=n.normalized();
                h.set(t,material,n);
                return true;
            }
        }
        return false;
    }


Vector3f c;
float R;
};


#endif
