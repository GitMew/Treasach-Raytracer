#ifndef TREASACH_EXAMPLES_H
#define TREASACH_EXAMPLES_H

#include <iostream>
#include "../rendering/Camera.h"
#include "../rendering/Screen.h"
#include "../rendering/Tracer.h"
#include "../scene/Scene.h"
#include "../primitives/Sphere.h"
#include "../primitives/FlatDisk.h"
#include "../scene/Scene.h"
#include "../shading/Material.h"
#include "../shading/BRDF.h"
#include "../primitives/FlatInfinitePlane.h"


void execute(const Camera& camera, Screen& screen, Tracer& tracer, const Scene& scene, const std::string& fileStem) {
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

    // Render
    tracer.render(camera, screen, scene, "../data/out/" + fileStem + ".png");
}


void example1() {
    Camera camera = Camera({0,0,-50}, {0,0,1}, {1600, 1, 10, 0, 0});  // Camera looking in +z.
    Screen screen = Screen(500,400);
    Tracer tracer = Tracer(100, 10, 0.1, 0, 1);
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

    execute(camera, screen, tracer, scene, "example1");
}


void example2() {
    Camera camera = Camera({0,2,-50}, {0,-0.01,1}, {1500, 3, 10, 0, 0});  // Camera looking in +z.
    Screen screen = Screen(500,400);
    //Camera camera = Camera({0,2,-50}, {0,-0.01,1}, {500, 4, 10, 0, 0});  // Camera looking in +z.
    //Screen screen = Screen(200,160);
    Tracer tracer = Tracer(50, 50, 0.1, 0, 1);
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

    auto light1 = new FlatDisk({-5, 5, 0}, {1, -1, 0.1}, 1.5);  // Closer to the camera, pointing away from it.
    light1->setMaterial(lightMaterial1);
    scene.add(light1);

//    // Light 2
//    auto emitter2 = new UniformEmission(ContinuousColour({8, 15, 7}));
//    auto lightMaterial2 = new Material(diffuseBrdf, emitter2);
//
//    auto light2 = new FlatDisk({5, 1, -4}, {-1, 0, 1}, 2.0);  // Closer to the camera, pointing away from it.
//    light2->setMaterial(lightMaterial2);
//    scene.add(light2);

    execute(camera, screen, tracer, scene, "example2");
}

#endif