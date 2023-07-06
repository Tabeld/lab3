#ifndef HISTOGRAM_H_INCLUDED
#define HISTOGRAM_H_INCLUDED

#endif // HISTOGRAM_H_INCLUDED

#include <vector>
void find_minmax(const std::vector<double>& numbers, double& min, double& max) {
    max = *std::max_element(numbers.begin(), numbers.end());
    min = *std::min_element(numbers.begin(), numbers.end());
}
