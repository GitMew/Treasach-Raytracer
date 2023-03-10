#ifndef TREASACH_EXAMPLES_H
#define TREASACH_EXAMPLES_H

#include <iostream>
#include <chrono>  // For timing
//#include <format>  // For formatting timing

#include "../rendering/Camera.h"
#include "../rendering/Screen.h"
#include "../rendering/RayTracer.h"
#include "../scene/Scene.h"
#include "../primitives/Sphere.h"
#include "../primitives/FlatDisk.h"
#include "../scene/Scene.h"
#include "../shading/Material.h"
#include "../shading/BRDF.h"
#include "../primitives/FlatInfinitePlane.h"
#include "../primitives/AABB.h"
#include "../rendering/NormalTracer.h"

#define OVERWRITE_IMAGES true  // Handy if you're doing quick prototyping.


// Replacement for std::format("{:_%Y-%m-%d_%H%M%OS}", timestamp) because somehow <format> isn't recognised by CLion.
// https://stackoverflow.com/a/41826232/9352077
std::string currentTimestring() {
    using sysclock_t = std::chrono::system_clock;

    std::time_t now = sysclock_t::to_time_t(sysclock_t::now());
    char buf[32] = { 0 };  // Too big, but whatever.
    std::strftime(buf, sizeof(buf), "_%Y-%m-%d_%H%M%S", std::localtime(&now));
    return std::string(buf);
}


void execute(const Camera& camera, Screen& screen, RayTracer& tracer, const Scene& scene, const std::string& fileStem) {
    // Some diagnostics first
    std::cout << "Camera: " << camera.lens.antiAliasingDensity*camera.lens.antiAliasingDensity << " anti-aliasing rays pp; " << camera.lens.focalPointAmount << " focal points pp.\n";
    std::cout << "Image: " << screen.width << " x " << screen.height << "\n";
    std::cout << "Rays in deepest layer: " << tracer.shadowIntegralSamples << " x (" << tracer.hemisphericIntegralSamples << "^" << tracer.maxDepth << ")\n";
    std::cout << "Objects: " << scene.objects.size() << "\n";
    std::cout << "Lights: " << scene.lights.size() << "\n";

    std::cout << "Total amount of rays, roughly: " <<
        tracer.shadowIntegralSamples
        * pow(tracer.hemisphericIntegralSamples, tracer.maxDepth)
        * screen.width * screen.height
        * (1 + camera.lens.focalPointAmount)
        * pow(camera.lens.antiAliasingDensity, 2)
        << " rays.\n";

    // Render and time.
    auto startTime = std::chrono::high_resolution_clock::now();
    std::string fullFilePath = "../data/out/" + fileStem + (OVERWRITE_IMAGES ? "" : currentTimestring()) + ".png";
    tracer.render(camera, screen, scene, fullFilePath);
    auto endTime = std::chrono::high_resolution_clock::now();

    auto durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count();
    std::cout << "Render took " << durationMs/1000.0 << " seconds.";
}

void execute(const Camera& camera, Screen& screen, NormalTracer& tracer, const Scene& scene, const std::string& fileStem) {
    std::string fullFilePath = "../data/out/" + fileStem + (OVERWRITE_IMAGES ? "" : currentTimestring()) + ".png";
    tracer.render(camera, screen, scene, fullFilePath);
}


// Sphere with two lights and a mirror
void example1() {
    Camera camera = Camera({0,0,-50}, {0,0,1}, {1600, 1, 10, 0, 0});  // Camera looking in +z.
    Screen screen = Screen(500,400);
    RayTracer tracer = RayTracer(100, 10, 0.1, 0, 1);
    Scene scene   = Scene();

    // A diffuse sphere
    auto diffuseBrdf = std::vector<BRDF*>();
    diffuseBrdf.push_back(new DiffuseBRDF(1.0));
    auto diffuseMaterial = new Material(diffuseBrdf);

    auto sphere = new Sphere(Vector3(0, 0, -1), 2.5);
    sphere->setMaterial(diffuseMaterial);
    scene.add(sphere);

    // A mirror
    auto mirrorBrdf = std::vector<BRDF*>();
    mirrorBrdf.push_back(new MirrorBRDF());
    auto mirrorMaterial = new Material(mirrorBrdf);

    auto mirror = new FlatDisk({-7, 0, 5}, {0.5, 0, -1}, 4.0);
    mirror->setMaterial(mirrorMaterial);
    scene.add(mirror);

    // Light 1
    auto emitter1 = new UniformEmission(ContinuousColour({15, 5, 7}));
    auto lightMaterial1 = new Material(diffuseBrdf, emitter1);

    auto light1 = new FlatDisk({-5, 3, -4}, {1, -1, 1}, 2.0);  // Closer to the camera, pointing away from it.
    light1->setMaterial(lightMaterial1);
    scene.add(light1);

    // Light 2
    auto emitter2 = new UniformEmission(ContinuousColour({8, 15, 7}));
    auto lightMaterial2 = new Material(diffuseBrdf, emitter2);

    auto light2 = new FlatDisk({5, 1, -4}, {-1, 0, 1}, 2.0);  // Closer to the camera, pointing away from it.
    light2->setMaterial(lightMaterial2);
    scene.add(light2);

    // Mirror frame: a very dim emitter, so that it 1. doesn't affect the rest of the scene, and 2. absorbs 100% of the rays that hit it.
    auto emitter3 = new UniformEmission(ContinuousColour({0.1, 0.1, 0.1}));
    auto lightMaterial3 = new Material(diffuseBrdf, emitter3);
    auto mirrorFrame = new FlatDisk({-7.05, 0, 5.5}, {0.5, 0, -1}, 4.1);
    mirrorFrame->setMaterial(lightMaterial3);
    scene.add(mirrorFrame);

    execute(camera, screen, tracer, scene, "example1");
}


// Half dome and a ground plane
void example2() {
    Camera camera = Camera({0,2,-50}, {0,-0.01,1}, {1500, 3, 10, 0, 0});  // Camera looking in +z.
    Screen screen = Screen(500,400);
    //Camera camera = Camera({0,2,-50}, {0,-0.01,1}, {500, 4, 10, 0, 0});  // Camera looking in +z.
    //Screen screen = Screen(200,160);
    RayTracer tracer = RayTracer(50, 50, 0.1, 0, 1);
    Scene scene   = Scene();

    //
    auto diffuseBrdf = std::vector<BRDF*>();
    diffuseBrdf.push_back(new DiffuseBRDF(1.0));
    auto diffuseMaterial = new Material(diffuseBrdf);

    auto plane = new FlatInfinitePlane({0,0,0}, {0,1,0});
    plane->setMaterial(diffuseMaterial);
    scene.add(plane);

    auto sphere = new Sphere(Vector3(0, 1.1, -1), 3);
    sphere->setMaterial(diffuseMaterial);
    scene.add(sphere);

//    // A mirror
//    auto mirrorBrdf = std::vector<BRDF*>();
//    mirrorBrdf.push_back(new MirrorBRDF());
//    auto mirrorMaterial = new Material(mirrorBrdf);
//
//    auto mirror = new FlatDisk({-7, 0, 5}, {0.5, 0, -1}, 4.0);
//    mirror->setMaterial(mirrorMaterial);
//    scene.add(mirror);

    // Light 1
    auto emitter1 = new UniformEmission(ContinuousColour({150, 100, 100}));
    auto lightMaterial1 = new Material(diffuseBrdf, emitter1);

    auto light1 = new FlatDisk({-5, 5, 0}, {1, -1, 0.1}, 1.5);
    light1->setMaterial(lightMaterial1);
    scene.add(light1);

//    // Light 2
//    auto emitter2 = new UniformEmission(ContinuousColour({8, 15, 7}));
//    auto lightMaterial2 = new Material(diffuseBrdf, emitter2);
//
//    auto light2 = new FlatDisk({5, 1, -4}, {-1, 0, 1}, 2.0);
//    light2->setMaterial(lightMaterial2);
//    scene.add(light2);

    execute(camera, screen, tracer, scene, "example2");
}


// AABB and rotated camera.
void example3() {
    Camera camera = Camera({-50,15,-50}, {1,-0.3,1}, {1500, 3, 10, 0, 0});  // Camera looking toward +x and +z and slightly downward.
//    Screen screen = Screen(400,400);
//    RayTracer tracer = RayTracer(10, 20, 0.1, 0, 0);
    Screen screen = Screen(500,400);
    RayTracer tracer = RayTracer(500, 50, 0.1, 0, 1);
    Scene scene   = Scene();

    // Box
    auto diffuseBrdf = std::vector<BRDF*>();
    diffuseBrdf.push_back(new DiffuseBRDF(1.0));
    auto diffuseMaterial = new Material(diffuseBrdf);

    auto box = new AABB({-2,-2,-2}, {2,2,2});
    box->setMaterial(diffuseMaterial);
    scene.add(box);

    // Plane
    auto plane = new FlatInfinitePlane({0,-3.5,0}, {0,1,0});
    plane->setMaterial(diffuseMaterial);
    scene.add(plane);

    // Light
    auto emitter1 = new UniformEmission(ContinuousColour({30, 22, 22}));
    auto lightMaterial1 = new Material(diffuseBrdf, emitter1);

    auto light1 = new FlatDisk({-5, 3.5, 5}, {1, -1.25, 0.1}, 1.25);
    light1->setMaterial(lightMaterial1);
    scene.add(light1);

    execute(camera, screen, tracer, scene, "example3");
}

// Cornell-ish box
void example4() {
//    Camera camera = Camera({-30,5,0}, {1,0,0}, {800, 3, 10, 0, 0});  // Far camera.
//    Camera camera = Camera({-10,5,0}, {1,0,0}, {150, 3, 10, 0, 0});  // Close camera.
//    Camera camera = Camera({4.5,9.9,4.5}, {-1,-1.75,-1}, {150, 3, 10, 0, 0});  // Reverse camera.
//    Camera camera = Camera({0,9.9,0}, {0.01,-1,0}, {250, 3, 10, 0, 0});  // Camera hovering over the room (note: can't point straight up/down due to up-vector).
    Camera camera = Camera({-2,0,0}, {-0.01,1,0}, {250, 3, 10, 0, 0});  // Camera looking up.
    Screen screen = Screen(200,400);
    RayTracer tracer = RayTracer(1, 1, 0.0, 0, 1);
//    NormalTracer tracer = NormalTracer();
    Scene scene   = Scene();

    // Materials
    auto diffuseBrdfWhite = std::vector<BRDF*>();
    auto diffuseBrdfRed   = std::vector<BRDF*>();
    auto diffuseBrdfGreen = std::vector<BRDF*>();

    diffuseBrdfWhite.push_back(new DiffuseBRDF(1.0));
    diffuseBrdfRed  .push_back(new DiffuseBRDF(1.0, {250,0,25}));
    diffuseBrdfGreen.push_back(new DiffuseBRDF(1.0, {80,180,50}));

    auto diffuseMaterialWhite = new Material(diffuseBrdfWhite);
    auto diffuseMaterialRed   = new Material(diffuseBrdfRed);
    auto diffuseMaterialGreen = new Material(diffuseBrdfGreen);

//    auto emitter1 = new UniformEmission(0.25*ContinuousColour({238,177,122}));
    auto emitter1 = new UniformEmission(ContinuousColour({238,177,122}));
    auto lightMaterial1 = new Material(diffuseBrdfWhite, emitter1);

    auto mirrorBrdf = std::vector<BRDF*>();
    mirrorBrdf.push_back(new MirrorBRDF());
    auto mirrorMaterial = new Material(mirrorBrdf);

    // Box
    auto leftWall  = new FlatInfinitePlane({0,0,-5}, {0,0,1});
    auto rightWall = new FlatInfinitePlane({0,0,+5}, {0,0,-1});
    auto floor     = new FlatInfinitePlane({0,0,0}, {0,1,0});
    auto ceiling   = new FlatInfinitePlane({0,+10,0}, {0,-1,0});
    auto back      = new FlatInfinitePlane({+5,0,0}, {-1,0,0});
    auto front     = new FlatInfinitePlane({-11,0,0}, {1,0,0});

    leftWall ->setMaterial(diffuseMaterialRed);
    rightWall->setMaterial(diffuseMaterialGreen);
    floor    ->setMaterial(diffuseMaterialWhite);
    ceiling  ->setMaterial(diffuseMaterialWhite);
    back     ->setMaterial(diffuseMaterialWhite);
    front    ->setMaterial(diffuseMaterialWhite);

    scene.add(leftWall);
    scene.add(rightWall);
    scene.add(floor);
    scene.add(ceiling);
    scene.add(back);
//    scene.add(front);

    // Content
    auto box1 = new AABB({-0.5,0,-3.5}, {2.0,5,-0.5});
    box1->setMaterial(diffuseMaterialWhite);
//    scene.add(box1);

    auto box2 = new AABB({-5.5,0,1.5}, {-3.5,2,3.5});
    box2->setMaterial(diffuseMaterialWhite);
//    scene.add(box2);

    auto sphere = new Sphere({0,5,2}, 1.5);
    sphere->setMaterial(mirrorMaterial);
//    scene.add(sphere);

    // Light
    auto light1 = new FlatDisk({-2, 9.99, 0}, {0, -1, 0}, 2.0);
//    auto light1 = new FlatDisk({-2, 0.01, 0}, {0, -1, 0}, 2.0);
    light1->setMaterial(lightMaterial1);
    scene.add(light1);

    execute(camera, screen, tracer, scene, "example4");
}

#endif