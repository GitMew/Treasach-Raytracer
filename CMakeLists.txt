cmake_minimum_required(VERSION 3.23)
project(treasach)

set(CMAKE_CXX_STANDARD 20)

add_executable(treasach src/main.cpp
        src/geometry/Vector.cpp
        src/geometry/Vector.h
        src/rendering/Ray.cpp
        src/rendering/Ray.h
        src/rendering/Camera.cpp
        src/rendering/Camera.h
        src/rendering/Screen.cpp
        src/rendering/Screen.h
        src/rendering/Tracer.cpp
        src/rendering/Tracer.h
        src/scene/Scene.cpp
        src/scene/Scene.h
        src/primitives/SceneObject.h
        src/primitives/Sphere.cpp
        src/primitives/Sphere.h)
