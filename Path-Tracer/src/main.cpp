#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include <string>
#include "vector.h"
#include "material.h"
#include "objects.h"
#include "camera.h"
#include "scene.h"
#include "renderer.h"

std::string OBJ_FILE_PATH = "../obj/star-wars.obj";

int main(int argc, char *argv[]) {

    time_t start, stop;
    time(&start);
    int samples = 100;

    if (argc == 2) samples = atoi(argv[1]);

    Camera camera = Camera(Vec(0, -3, 2.5), Vec(0,0,1), 1280, 720);
    Scene scene = Scene();

    scene.add( dynamic_cast<Object*>(new Sphere(Vec(0,0,-1000), 1000, Material(DIFF, Vec(0.85,0.4,0.4)))) );
    scene.add( dynamic_cast<Object*>(new Sphere(Vec(-1004,0,0), 1000, Material(DIFF, Vec(0,0,0)))) );
    scene.add( dynamic_cast<Object*>(new Sphere(Vec(1004,0,0), 1000, Material(DIFF, Vec(0,0,0)))) );
    scene.add( dynamic_cast<Object*>(new Sphere(Vec(0,1006,0), 1000, Material(DIFF, Vec(0.85,0.4,0.4)))) );
    scene.add( dynamic_cast<Object*>(new Sphere(Vec(0,0,110), 100, Material(EMIT, Vec(0,0,0), Vec(2.2,2.2,2.2)))) );
    scene.add( dynamic_cast<Object*>(new Mesh(Vec(), OBJ_FILE_PATH, Material(EMIT, Vec(0.9, 0.9, 0.9)))) );


    /*Renderer renderer = Renderer(&scene, &camera);
    renderer.render(samples);
    renderer.save_image("render.png");*/

    time(&stop);
    double diff = difftime(stop, start);
    int hrs = (int)diff/3600;
    int mins = ((int)diff/60)-(hrs*60);
    int secs = (int)diff-(hrs*3600)-(mins*60);
    printf("\rRendering: Complete!\nTime Taken: %i hrs, %i mins, %i secs\n\n", hrs, mins, secs);
    return 0;
}