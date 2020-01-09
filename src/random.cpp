#include "random.h"

namespace rfunc {

RandFunction uniform_distribution_rand(int min_value, int max_value) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(min_value, max_value);

    return std::bind(distribution, generator);
}

RandFunction fixed_distribution(int value) {
    return [value]() {
        return value;
    };
}

}
