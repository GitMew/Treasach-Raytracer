#ifndef TREASACH_RNG_H
#define TREASACH_RNG_H

#include <random>


class RNG {  // Wrapper for the RNG setup found at https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution.
    public:
        RNG();
        double rand01();
        uint64_t randInt(uint64_t max);

    private:
        std::mt19937 mersenneTwister;
        std::uniform_real_distribution<> realDistribution;
};


#endif //TREASACH_RNG_H
