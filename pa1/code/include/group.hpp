#ifndef GROUP_H
#define GROUP_H


#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>
using namespace std;

// TODO: Implement Group - add data structure to store a list of Object*
class Group : public Object3D {

public:

    Group() {
    }

    explicit Group (int num_objects) {
       list.resize(num_objects);
    }

    ~Group() override {

    }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        bool az=false;
        for(int i=0;i<list.size();i++){
            if(list[i]->intersect(r,h,tmin))//短路性质
            az=true;
        }
        return az;
    }

    void addObject(int index, Object3D *obj) {
        list[index]=obj;
    }

    int getGroupSize() {
        return list.size();
    }

private:
vector < Object3D* > list;

};

#endif
	
