#ifndef RFUNC_RANDOM_H
#define RFUNC_RANDOM_H

#include <functional>
#include <random>
#include <unordered_map>

namespace rfunc {

enum Distribution {FIXED, UNIFORM};
const std::unordered_map<std::string, Distribution> string_to_distribution({
    {"FIXED", Distribution::FIXED},
    {"UNIFORM", Distribution::UNIFORM}
});

std::function<int()> get_random_function(
    Distribution distribution, int max_value
);

}

#endif
