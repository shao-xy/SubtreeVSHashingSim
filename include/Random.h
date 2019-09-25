#ifndef __INCLUDE_RANDOM_H__
#define __INCLUDE_RANDOM_H__

#include <random>

class RandomGenerator;

class RandomRange {
	friend class RandomGenerator;
    std::uniform_int_distribution<size_t> dist_;
public:
	RandomRange(size_t min = 0, size_t max = 0) : dist_(min, max) {}
};

class RandomGenerator {
    // random seed by default
    std::mt19937 gen_;

public:
    RandomGenerator(unsigned int seed = std::random_device{}())
        : gen_{seed} {}

    // if you want predictable numbers
    void SetSeed(unsigned int seed) {
        gen_.seed(seed);
    }

    size_t operator()(RandomRange range) {
        return range.dist_(gen_);
    }
};

#endif /* include/Random.h */
