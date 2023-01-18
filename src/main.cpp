#include <iostream>
#include "rendering/Tracer.h"
#include "scene/Scene.h"
#include "rendering/Camera.h"
#include "rendering/Screen.h"
#include "primitives/Sphere.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    Camera camera = Camera();       // TODO: needs an aperture and a focal distance.
    Screen screen = Screen(20,10);  // TODO: Since the camera has a focal plane with distance AND we need a distance to the screen, perhaps the screen should be part of the camera.
    Scene scene   = Scene();
    Tracer tracer = Tracer();

    scene.add(Sphere(Vector(0,0,10), 1));
    tracer.render(camera, screen, scene, "test.png");
    return 0;
}
