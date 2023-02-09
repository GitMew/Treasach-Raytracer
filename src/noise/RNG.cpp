#include "RNG.h"


RNG::RNG() {
    std::random_device rd;                       // Will be used to obtain a seed for the random number engine
    this->mersenneTwister = std::mt19937(rd());  // Standard mersenne_twister_engine seeded with rd()
    this->realDistribution = std::uniform_real_distribution<>(0.0, 1.0);
}


double RNG::rand01() {
    return this->realDistribution(this->mersenneTwister);
}

uint64_t RNG::randInt(uint64_t max) {
    // Cast based on https://stackoverflow.com/q/605533/9352077
    return static_cast<uint64_t>(std::floor(this->rand01()*max));
}
