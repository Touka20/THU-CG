#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "scene_parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"

#include <string>


int main(int argc, char *argv[]) {
    for (int argNum = 1; argNum < argc; ++argNum) {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }

    if (argc != 3) {
        cout << "Usage: ./bin/PA1 <input scene file> <output bmp file>" << endl;
        return 1;
    }
    string inputFile = argv[1];
    string outputFile = argv[2];  // only bmp is allowed.

    // TODO: Main RayCasting Logic
    // First, parse the scene using SceneParser.
    SceneParser s(inputFile.c_str());
    Camera *cam=s.getCamera();
    Image I(cam->getWidth(),cam->getHeight());

    // Then loop over each pixel in the image, shooting a ray
    for ( int x = 0; x < cam->getWidth() ; ++x) {
        for ( int y = 0; y < cam->getHeight() ; ++y) {
            Ray camRay = s.getCamera()->generateRay(Vector2f(x, y)) ;
            
            Group* baseGroup = s.getGroup() ;
            Hit hit;
            bool isIntersect = baseGroup->intersect (camRay,hit,0);
            //std::cout<<x<<' '<<y<<' '<<camRay.getDirection()[0]<<' '<<camRay.getDirection()[1]<<' '<<camRay.getDirection()[2]<<' '<<camRay.getOrigin()[2]<<endl;
            if (isIntersect) {
                Vector3f finalColor = Vector3f::ZERO;
                for ( int li = 0; li < s.getNumLights() ; ++li ) {
                    Light* light = s.getLight(li) ;
                    Vector3f L, lightColor ;
                    light->getIllumination(camRay. pointAtParameter( hit.getT() ) , L, lightColor) ;
                    finalColor += hit.getMaterial ()->Shade(camRay, hit , L, lightColor) ;
                }           
                    I.SetPixel(x, y, finalColor) ;
            } else {
            I.SetPixel(x, y, s.getBackgroundColor() ) ;
            }
        }
    }

    // through that pixel and finding its intersection with
    // the scene.  Write the color at the intersection to that
    // pixel in your output image.
    I.SaveImage(argv[2]);
    cout << "Hello! Computer Graphics!" << endl;
    return 0;
}

