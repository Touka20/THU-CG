#pragma once

#include <image.hpp>
#include <algorithm>
#include <queue>
#include <cstdio>
#include <iostream>
#include <stack>
using namespace std;
class Element {
public:
    virtual void draw(Image &img) = 0;
    virtual ~Element() = default;
};

class Line : public Element {

public:
    int xA, yA;
    int xB, yB;
    Vector3f color;

    void draw(Image &img) override {
        if(xA==xB){
            int y=yA<yB?yA:yB;
            int y_=yA+yB-y;
            for(int i=y;i<y_;i++){
                img.SetPixel(xA,i,color);
            }
        }

        else if(yA==yB){
            int x=xA<xB?xA:xB;
            int x_=xA+xB-x;
            for(int i=x;i<x_;i++){
                img.SetPixel(i,yA,color);
            }
        }
        else
        {
            float k=float((yB-yA))/(xB-xA),e;
            if(k>0)e=-0.5;else e=0.5;
            if(k<1&&k>-1){//|k|<1,from left to right
                int x=xA<xB?xA:xB;int x_=xA+xB-x;
                int y=xA<xB?yA:yB;
                for(;x<=x_;x++){
                    img.SetPixel(x,y,color);
                    e=e+k;
                     if(k>0){if(e>=0){y++;e=e-1;}}
                     else {if(e<=0){y--;e=e+1;}}
                }
            }
            else{//|k|>=1,from up to down(the top area of a picture has lower y)
                int y=yA<yB?yA:yB;int y_=yA+yB-y;
                int x=yA<yB?xA:xB;
                for(;y<y_;y++){
                    img.SetPixel(x,y,color);
                    e=e+1/k;
                    if(k>0){if(e>=0){x++;e=e-1;}}
                    else {if(e<=0){x--;e++;}}
                }
            }
        }
        img.SetPixel(xA,yA,color);
        img.SetPixel(xB,yB,color);
        printf("Draw a line from (%d, %d) to (%d, %d) using color (%f, %f, %f)\n", xA, yA, xB, yB,
                color.x(), color.y(), color.z());
    }
};

class Circle : public Element {
public:
    int cx, cy;
    int radius;
    Vector3f color;
    void draw(Image &img) override {
        // TODO: Implement Algorithm to draw a Circle
        int x=0;int y=radius;
        float d=1.25-radius;
        circlepoints(x,y,img);
        circlepoints(x,-y,img);
        circlepoints(-x,-y,img);
        circlepoints(-x,y,img);
        circlepoints(y,x,img);
        circlepoints(-y,x,img);
        circlepoints(y,-x,img);
        circlepoints(-y,-x,img);
        while(x<=y){
            if(d<0)d+=2*x+3;
            else {
                d+=2*(x-y)+5;y--;
            }
            x++;
            circlepoints(x,y,img);
            circlepoints(x,-y,img);
            circlepoints(-x,-y,img);
            circlepoints(-x,y,img);
            circlepoints(y,x,img);
            circlepoints(-y,x,img);
            circlepoints(y,-x,img);
            circlepoints(-y,-x,img);
        }
        printf("Draw a circle with center (%d, %d) and radius %d using color (%f, %f, %f)\n", cx, cy, radius,
               color.x(), color.y(), color.z());
    }

    void circlepoints(int x,int y,Image &img){
        if(x+cx>=0&&x+cx<=img.Width()-1&&y+cy>=0&&y+cy<=img.Height()-1)
        img.SetPixel(x+cx,y+cy,color);
    }
};

typedef struct{
    int x;
    int y;
}Seed;

class Fill : public Element {

public:
    int cx, cy;
    Vector3f color;
    bool inside(Image &img,int x){
        if(x>=0&&x<img.Width())return true;
        return false;
    }
    void draw(Image &img) override {
        int x=cx;int y=cy;
        // TODO: Flood fill
        Vector3f oldcolor=img.GetPixel(x,y);
        if(oldcolor==color)return;//如果本来就是要涂的颜色，直接return
        int xl,xr,i;//刚画完的一条线段，其左右端点横坐标
        bool spanNeedFill;//当前区域是否需要涂色的flag
        Seed pt;pt.x=x;pt.y=y;
        stack<Seed>s;
        s.push(pt);
        while(!s.empty()){
            pt=s.top();//取出栈顶元素，往左往右分别画到颜色不是oldcolor为止
            s.pop();
            x=pt.x;
            y=pt.y;
            while(inside(img,x)){//往右画
                if(img.GetPixel(x,y)==oldcolor){
                    img.SetPixel(x,y,color);
                    x++;
                }
                else break;
            }
            xr=x-1;x=pt.x-1;
            while(inside(img,x)){//往右画
                if(img.GetPixel(x,y)==oldcolor){
                    img.SetPixel(x,y,color);
                    x--;
                }
                else break;
            }
            xl=x+1;
            //处理上面一条扫描线
            x=xl;
            y++;if(y<img.Height()){
            while(x<=xr){
                spanNeedFill=false;
                while(inside(img,x)){
                    if(img.GetPixel(x,y)==oldcolor){
                        spanNeedFill=true;
                        x++;
                    }
                    else break;
                }
                if(spanNeedFill){
                    pt.x=x-1;pt.y=y;
                    s.push(pt);
                    spanNeedFill=false;
                }
                while(inside(img,x)){
                    if(img.GetPixel(x,y)!=oldcolor&&x<=xr)x++;
                    else break;
                }
            }
            }
            //处理下面一条扫描线
            x=xl;
            y=y-2;if(y>=0){
            while(x<=xr){
                spanNeedFill=false;
                while(inside(img,x)){
                    if(img.GetPixel(x,y)==oldcolor){
                        spanNeedFill=true;
                        x++;
                    }
                    else break;
                }
                if(spanNeedFill){
                    pt.x=x-1;pt.y=y;
                    s.push(pt);
                    spanNeedFill=false;
                }
                while(inside(img,x)){
                    if(img.GetPixel(x,y)!=oldcolor&&x<=xr)x++;
                    else break;
                }
            }
            }

        }
        printf("Flood fill source point = (%d, %d) using color (%f, %f, %f)\n", cx, cy,
                color.x(), color.y(), color.z());
    }
};