#ifndef TREASACH_TEST_H
#define TREASACH_TEST_H

#include <iostream>
#include "../geometry/SphereMath.h"


void test_hemisphereSampling() {
    auto rng = RNG();
    auto normal = Vector3(0,1,0);
    for (int i = 0; i < 1000; i++) {
        Vector3 result;
        double pdf = sampleHemisphereBiased(rng, normal, result);
        std::cout << pdf << " [" << result.x << "," << result.y << "," << result.z << "]\n";
    }
}

#endif //TREASACH_TEST_H
