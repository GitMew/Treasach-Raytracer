#ifndef TREASACH_MATERIAL_H
#define TREASACH_MATERIAL_H

#include <vector>
#include <optional>
#include <memory>
#include "Colour.h"
#include "EmissionFunction.h"

class BRDF;

/**
 * Stores the light interaction properties of an object.
 * Consists of its BSDFs, a normal map, and if it is a light, its light properties.
 */
class Material {
    public:
        // This is the way C++ handles abstract fields. It has to be a pointer, because otherwise the compiler doesn't
        // know how big the field should be. unique_ptr makes it so that the pointer is freed when the surrounding
        // object dies; a shared_ptr is a generalisation that counts how many references still exist and frees at 0.
        std::vector<std::shared_ptr<BRDF>> brdfs  = std::vector<std::shared_ptr<BRDF>>();
        //std::shared_ptr<EmissionFunction> emitter = std::make_shared<NoEmission>();
        std::shared_ptr<EmissionFunction> emitter = nullptr;

        Material();
        Material(const std::vector<BRDF*>& brdfs);
        Material(const std::vector<BRDF*>& brdfs, EmissionFunction* emitter);
};


#endif //TREASACH_MATERIAL_H
