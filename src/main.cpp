#include <iostream>
#include "rendering/Tracer.h"
#include "scene/Scene.h"
#include "rendering/Camera.h"
#include "rendering/Screen.h"
#include "primitives/Sphere.h"


int main() {
    std::cout << "Hello, World!" << std::endl;

    Camera camera = Camera({0,0,0}, {0,0,1}, {10, 10, 0});
    Screen screen = Screen(20,10);
    Scene scene   = Scene();
    Tracer tracer = Tracer();

    scene.add(new Sphere(Vector3(0, 0, 10), 1));
    tracer.render(camera, screen, scene, "test.png");
    return 0;
}
