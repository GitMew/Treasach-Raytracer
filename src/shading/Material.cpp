#include "Material.h"
#include "BRDF.h"  // Wasn't added in Material.h due to cyclic dependency, but needed so that BRDF is not an "incomplete type".


Material::Material() = default;

Material::Material(const std::vector<BRDF*>& brdfs) {
    // Convert the given pointers to shared pointers. Inverse of https://stackoverflow.com/a/63918146/9352077.
    this->brdfs.reserve(brdfs.size());
    std::transform(brdfs.cbegin(), brdfs.cend(), std::back_inserter(this->brdfs),
                   [] (BRDF* ptr) { return std::shared_ptr<BRDF>(ptr); });
}

Material::Material(const std::vector<BRDF*>& brdfs, EmissionFunction *emitter) : Material(brdfs) {  // Delegated constructor
    this->emitter = std::shared_ptr<EmissionFunction>(emitter);
}

