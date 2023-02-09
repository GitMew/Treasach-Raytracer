#ifndef TREASACH_CAMERA_H
#define TREASACH_CAMERA_H

#include <vector>
#include "../geometry/Vector3.h"
#include "Ray.h"
#include "Screen.h"


struct LensProperties {
    double viewDistance;   // Distance to the view plane.

    unsigned antiAliasingDensity = 1;  // The total amount of points will be this squared.

    double focalDistance;  // Distance to the depth-of-field focal plane.
    double aperture = 0;       // Radius of the lens opening wherein "primary rays" can start.
    unsigned focalPointAmount = 0;
};


class Camera {
    public:
        Vector3 eye;
        Vector3 dir;
        Vector3 up = Vector3(0, 1, 0);

        Vector3 viewX;
        Vector3 viewY;
        Vector3 viewZ;

        std::vector<std::pair<double,double>> intraPixelGrid;
        std::vector<std::pair<double,double>> apertureDisk;

        LensProperties lens;

        Camera(const Vector3& eye, const Vector3& dir, LensProperties lens);
        void generateRays(const Screen& screen, const Pixel& pixel, std::vector<Ray>& rays) const;

    private:

};


#endif //TREASACH_CAMERA_H
