#ifndef GROUP_H
#define GROUP_H


#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>


// TODO (PA2): Implement Group - copy from PA1
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

    void drawGL() override {
        for (auto *obj : list) {
            obj->drawGL();
        }
    }

    void addObject(int index, Object3D *obj) {
        list[index]=obj;
    }

    int getGroupSize() {
        return list.size();
    }

private:
std::vector <Object3D*> list;
};

#endif
	
