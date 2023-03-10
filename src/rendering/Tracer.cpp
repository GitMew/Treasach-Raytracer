#include <iostream>

#include "Tracer.h"
#include "stb_image_write.h"


void Tracer::render(const Camera &camera, Screen &screen, const Scene &scene, const std::string& fileOut) {
    std::cout << "=== Output will be written to " << fileOut << ". ===\n";

    // Raytracing
    std::cout << "Raytracing ...\n";
    //#pragma omp parallel
    for (auto& pixel : screen.pixels) {
        auto rays = std::vector<Ray>();
        camera.generateRays(screen, pixel, rays);
        //#pragma omp parallel for reduction (+:pixel.radiance)
        for (auto& ray : rays)
            pixel.radiance += this->trace(scene, ray, 0);  // Accumulate

        pixel.radiance = 1.0/rays.size() * pixel.radiance;  // Average
    }

    // Tone mapping
    std::cout << "Tonemapping ...\n";
    uint8_t pixelArray[3*screen.width*screen.height];
    for (auto& pixel : screen.pixels) {
        RgbColour rgbValue = this->tonemap(pixel.radiance);
        pixelArray[3*(pixel.y*screen.width + pixel.x) + 0] = rgbValue.red;
        pixelArray[3*(pixel.y*screen.width + pixel.x) + 1] = rgbValue.green;
        pixelArray[3*(pixel.y*screen.width + pixel.x) + 2] = rgbValue.blue;
        //std::cout << pixel << " (" << +rgbValue.red << ", " << +rgbValue.green << ", " << +rgbValue.blue << ")" << "\n";
    }

    // File writing
    std::cout << "Writing file ...\n";
    stbi_write_png(fileOut.c_str(), screen.width, screen.height, 3, pixelArray, 3*screen.width);  // Documentation: https://github.com/nothings/stb/blob/master/stb_image_write.h#L79
}
