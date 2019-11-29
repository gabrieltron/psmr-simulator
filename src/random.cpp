#include "random.h"

namespace rfunc {

std::function<int()> get_random_function(
    Distribution distribution, int max_value
) {
    std::random_device rd;
    std::mt19937 generator(rd());


    if (distribution == Distribution::UNIFORM) {
        std::uniform_int_distribution<int> distribution(0,max_value);
        return std::bind(distribution, generator);
    }
}

}