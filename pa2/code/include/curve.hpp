#ifndef CURVE_HPP
#define CURVE_HPP

#include "object3d.hpp"
#include <vecmath.h>
#include <vector>
#include <utility>
#include <iostream>

#include <algorithm>
using namespace std;
// TODO (PA2): Implement Bernstein class to compute spline basis function.
//       You may refer to the python-script for implementation.

// The CurvePoint object stores information about a point on a curve
// after it has been tesselated: the vertex (V) and the tangent (T)
// It is the responsiblility of functions that create these objects to fill in all the data.
struct CurvePoint {
    Vector3f V; // Vertex
    Vector3f T; // Tangent  (unit)
};

class Curve : public Object3D {
protected:
    std::vector<Vector3f> controls;
public:
    explicit Curve(std::vector<Vector3f> points) : controls(std::move(points)) {}

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        return false;
    }

    std::vector<Vector3f> &getControls() {
        return controls;
    }

    virtual void discretize(int resolution, std::vector<CurvePoint>& data) = 0;

    void drawGL() override {
        Object3D::drawGL();
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glDisable(GL_LIGHTING);
        glColor3f(1, 1, 0);
        glBegin(GL_LINE_STRIP);
        for (auto & control : controls) { glVertex3fv(control); }
        glEnd();
        glPointSize(4);
        glBegin(GL_POINTS);
        for (auto & control : controls) { glVertex3fv(control); }
        glEnd();
        std::vector<CurvePoint> sampledPoints;
        discretize(30, sampledPoints);
        glColor3f(1, 1, 1);
        glBegin(GL_LINE_STRIP);
        for (auto & cp : sampledPoints) { glVertex3fv(cp.V); }
        glEnd();
        glPopAttrib();
    }
};

class BezierCurve : public Curve {
public:
    explicit BezierCurve(const std::vector<Vector3f> &points) : Curve(points) {
        if (points.size() < 4 || points.size() % 3 != 1) {
            printf("Number of control points of BezierCurve must be 3n+1!\n");
            exit(0);
        }
    }

    void discretize(int resolution, std::vector<CurvePoint>& data) override {
        data.clear();
        // TODO (PA2): fill in data vector
        std::vector <Vector3f> c=getControls();
        std::vector <Vector3f> dif;
        int n=c.size()-1;
        for(int i=0;i<n;i++){
            dif.push_back(c[i+1]-c[i]);
        }
        for(int i=0;i<=resolution;i++){
            double t=i/double(resolution);
            std::vector<Vector3f>tmp1(c);
            std::vector<Vector3f>tmp2(dif);
            for(int j=0;j<n;j++){
                for(int k=0;k<n-j;k++){
                    tmp1[k]=(1-t)*tmp1[k]+t*tmp1[k+1];
                }
            }
            for(int j=0;j<n-1;j++){
                for(int k=0;k<=n-j-1;k++){
                    tmp2[k]=(1-t)*tmp2[k]+t*tmp2[k+1];
                }
            }                    
            data.push_back(CurvePoint({tmp1[0],n*tmp2[0]}));
        }
    }

protected:

};

class BsplineCurve : public Curve {
public:
    BsplineCurve(const std::vector<Vector3f> &points) : Curve(points) {
        if (points.size() < 4) {
            printf("Number of control points of BspineCurve must be more than 4!\n");
            exit(0);
        }
    }

    void discretize(int resolution, std::vector<CurvePoint>& data) override {
        data.clear();
        // TODO (PA2): fill in data vector
        int k=4;
        vector <Vector3f> c=getControls();
        int n=c.size()-1;
        vector <Vector3f> dif;
        for(int i=1;i<=n;i++){
            dif.push_back((c[i]-c[i-1])/(k-1)*(n+k));
        }

        vector <double> K;
        vector <double> K_;
        for(int i=0;i<=n+k;i++){
            K.push_back(double(i)/(n+k));
        }
        for(int i=0;i<=n+k-2;i++){
            K_.push_back(double(i)/(n+k));
        }

        for(int x=0;x<=resolution;x++){
            double t=K[k-1]+double(x)*(n-k+2)/resolution/(n+k);
            int j=x*(n-k+2)/resolution+k-1;

            vector<Vector3f>tmp1(c);
            vector<Vector3f>tmp2(dif);
            for(int r=1;r<=k-1;r++){
                for(int i=j;i>=j-k+r+1;i--){
                    double cof1=double(t-K[i])/(K[i+k-r]-K[i]);
                    double cof2=1-cof1;
                    tmp1[i]=cof1*tmp1[i]+cof2*tmp1[i-1];
                }
            }
            
            int j_=j-1;
            for(int r=1;r<=k-2;r++){
                for(int i=j_;i>=j_-k+r+2;i--){
                    double cof1=double(t-K_[i])/(K_[i+k-r]-K_[i]);
                    double cof2=1-cof1;
                    tmp2[i]=cof1*tmp2[i]+cof2*tmp2[i-1];
                }
            }
            data.push_back(CurvePoint({tmp1[j],(k-1)*tmp2[j_]}));
        }
    }

protected:

};

#endif // CURVE_HPP
