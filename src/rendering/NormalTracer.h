#ifndef TREASACH_NORMALTRACER_H
#define TREASACH_NORMALTRACER_H

#include "Tracer.h"


class NormalTracer : public Tracer {

    public:
        ContinuousColour trace(const Scene& scene, const Ray& ray, unsigned recursionsSoFar);
        RgbColour tonemap(ContinuousColour radiance);

};


#endif //TREASACH_NORMALTRACER_H
