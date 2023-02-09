#ifndef TREASACH_SCENEOBJECT_H
#define TREASACH_SCENEOBJECT_H

#include "../rendering/Ray.h"
#include "../shading/Material.h"
#include "../noise/RNG.h"
#include "../geometry/Constants.h"

class SceneObject {
    public:
        std::shared_ptr<Material> material = std::make_shared<Material>();

        // Intersection law.
        virtual double intersect(const Ray& ray) const = 0;
        /**
         * Normal law.
         * - Precondition: p should be on the object.
         * - Postcondition: the result must be normalised.
         */
        virtual Vector3 outwardNormal(const Vector3& p) const = 0;
        /**
         * IncomingDirection should be viewed as in a Phong diagram,
         * with its TAIL at p, not its head (so "incoming" might be confusing).
         */
        Vector3 normal(const Vector3& p, const Vector3& incomingDirection) const {
            Vector3 n = this->outwardNormal(p);
            bool isOutward = n * incomingDirection > 0;  // Not implemented with a branch (if) because that's probably slower.
            return (2*isOutward-1) * n;
        }
        // For lights, we need a way to get a random surface point. Returns the probability too.
        virtual double randomPoint(RNG& rng, Vector3& result) const = 0;

        void setMaterial(Material* materialPointer) { this->material = std::shared_ptr<Material>(materialPointer); };
};


#endif //TREASACH_SCENEOBJECT_H
