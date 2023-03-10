cmake_minimum_required(VERSION 3.23)
project(treasach)

set(CMAKE_CXX_STANDARD 20)

add_executable(treasach src/main.cpp
        src/geometry/Vector3.cpp
        src/geometry/Vector3.h
        src/rendering/Ray.cpp
        src/rendering/Ray.h
        src/rendering/Camera.cpp
        src/rendering/Camera.h
        src/rendering/Screen.cpp
        src/rendering/Screen.h
        src/rendering/RayTracer.cpp
        src/rendering/RayTracer.h
        src/scene/Scene.cpp
        src/scene/Scene.h
        src/primitives/SceneObject.h
        src/primitives/Sphere.cpp
        src/primitives/Sphere.h
        src/shading/BRDF.cpp
        src/shading/BRDF.h
        src/shading/Texture.cpp
        src/shading/Texture.h
        src/shading/Material.cpp
        src/shading/Material.h
        src/noise/RNG.cpp
        src/noise/RNG.h
        lib/stb/stb_image_write.cpp
        src/shading/Colour.h
        src/shading/Colour.cpp
        src/shading/EmissionFunction.cpp
        src/shading/EmissionFunction.h
        src/geometry/SphereMath.h
        src/geometry/SphereMath.cpp
        src/primitives/FlatDisk.cpp src/primitives/FlatDisk.h src/primitives/FlatInfinitePlane.cpp src/primitives/FlatInfinitePlane.h src/geometry/Constants.cpp src/geometry/Constants.h src/examples/Examples.h src/primitives/AABB.cpp src/primitives/AABB.h src/rendering/NormalTracer.cpp src/rendering/NormalTracer.h src/rendering/Tracer.h src/rendering/Tracer.cpp src/examples/Test.h)


# The following magic provided to you by Alexander on Discord, to manage libraries.
add_subdirectory(lib)
target_link_libraries(treasach PUBLIC
        stb
#        glm
)
target_include_directories(treasach PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/lib/stb"
#        "${PROJECT_SOURCE_DIR}/lib/glm"
)

