#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "histogram.h"

std::vector<double> input_numbers(size_t count);
std::vector<size_t> make_histogram(const std::vector<double>& numbers, size_t& bin_count, std::vector<std::string>& bins_title);
std::vector<std::string> input_bins_title(size_t bin_count);
void scale_histogram(std::vector<size_t>& bins, size_t& max_count, size_t max_length);
void print_histogram(const std::vector<size_t>& bins, const std::vector <std::string>& bins_title);

void svg_begin(double width, double height);
void svg_end();
void svg_text(double left, double baseline, std::string text);
void svg_rect(double x, double y, double width, double height, std::string stroke, std::string fill);
void svg_line(double x1, double y1, double x2, double y2, std::string stroke, double stroke_width, std::string stroke_dasharray);
void show_histogram_svg(const std::vector<size_t>& bins, const std::vector<std::string>& bins_title, double dash_length, double gap_length);

int main() {

    size_t number_count, bin_count;
    std::cerr << "Enter number count: ";
    std::cin >> number_count;

    std::cerr << "Enter numbers:\n";
    const auto numbers = input_numbers(number_count);

    std::cerr << "Enter bin count: ";
    std::cin >> bin_count;

    double max_el, min_el;
    find_minmax(numbers, min_el, max_el);

    double bin_size = (max_el - min_el) / bin_count;

    std::cerr << "Enter title of bins: \n";
    auto bins_title = input_bins_title(bin_count);

    auto bins = make_histogram(numbers, bin_count, bins_title);

    double dash_length, gap_length;
    std::cerr << "Enter dash length: ";
    std::cin >> dash_length;
    std::cerr << "Enter gap length: ";
    std::cin >> gap_length;

    show_histogram_svg(bins, bins_title, dash_length, gap_length);

}

std::vector<double> input_numbers(size_t count) {
    std::vector<double> result(count);
    for (size_t i = 0; i < count; i++) {
        std::cin >> result[i];
    }
    return result;
}

std::vector<size_t> make_histogram(const std::vector<double>& numbers, size_t& bin_count, std::vector<std::string>& bins_title) {

    double max_el, min_el;
    find_minmax(numbers, min_el, max_el);

    double bin_size = (max_el - min_el) / bin_count;
    auto number_count = numbers.size();
    std::vector<size_t> bins(bin_count);

    for (size_t i = 0; i < number_count; i++) {
        bool found = false;
        for (size_t j = 0; (j < bin_count - 1) && !found; j++) {
            auto lo = min_el + j * bin_size;
            auto hi = min_el + (j + 1) * bin_size;
            if ((lo <= numbers[i]) && (numbers[i] < hi)) {
                bins[j]++;
                found = true;
            }
        }
        if (!found) {
            bins[bin_count - 1]++;
        }
    }

    size_t max_length = 0;
    for (int i = 0; i < bin_count; i++) {
        max_length = std::max(max_length, bins_title[i].length());
    }
    size_t max_count = *std::max_element(bins.begin(), bins.end());

    scale_histogram(bins, max_count, max_length);

    return bins;

}

std::vector<std::string> input_bins_title(size_t bin_count) {
    std::vector <std::string> bins_title(bin_count);
    std::cin.ignore();
    for (int i = 0; i < bin_count; i++) {
        getline(std::cin, bins_title[i]);
    }
    return bins_title;
}

void scale_histogram(std::vector<size_t>& bins, size_t& max_count, size_t max_length) {
    if (max_count >= 79 - max_length) {
        for (int i = 0; i < bins.size(); i++) {
            bins[i] = (79 - max_length) * (static_cast<double>(bins[i]) / max_count);
        }
    }
}

void print_histogram(std::vector<size_t>& bins, const std::vector<std::string>& bins_title) {
    size_t bin_count = bins.size();
    size_t max_length = 0;
    for (int i = 0; i < bin_count; i++) {
        max_length = std::max(max_length, bins_title[i].length());
    }
    size_t max_count = *std::max_element(bins.begin(), bins.end());

    scale_histogram(bins, max_count, max_length);

    for (size_t i = 0; i < bin_count; i++) {
        if (bins_title[i].length() < max_length) {
            for (size_t j = 0; j < max_length - bins_title[i].length(); j++) {
                std::cout << " ";
            }
        }
        std::cout << bins_title[i] << "|";
        for (size_t j = 0; j < bins[i]; j++) {
            std::cout << '*';
        }
        std::cout << "\n";
    }

}

void svg_begin(double width, double height) {
    std::cout << "<?xml version='1.0' encoding='UTF-8'?>\n";
    std::cout << "<svg ";
    std::cout << "width='" << width << "' ";
    std::cout << "height='" << height << "' ";
    std::cout << "viewBox='0 0 " << width << " " << height << "' ";
    std::cout << "xmlns='http://www.w3.org/2000/svg'>\n";
}

void svg_end() {
    std::cout << "</svg>\n";
}

void svg_text(double left, double baseline, std::string text) {
    std::cout << "<text x='" << left << "' y='" << baseline << "'>" << text << "</text>";
}

void svg_rect(double x, double y, double width, double height, std::string stroke, std::string fill) {
    std::cout << "<rect x='" << x << "' y='" << y << "' width='" << width << "' height='"
            << height << "' stroke='" << stroke << "' fill='" << fill << "' />";
}

void svg_line(double x1, double y1, double x2, double y2, std::string stroke, double stroke_width, std::string stroke_dasharray) {
    std::cout << "<line x1='" << x1 << "' y1='" << y1 << "' x2='" << x2 << "' y2='" << y2
                << "' stroke='" << stroke << "' stroke-width='" << stroke_width << "' stroke-dasharray='" << stroke_dasharray << "' />";
}

void show_histogram_svg(const std::vector<size_t>& bins, const std::vector<std::string>& bins_title, double dash_length, double gap_length) {
    const auto IMAGE_WIDTH = 900;
    const auto TEXT_LEFT = 20;
    const auto TEXT_BASELINE = 20;
    const auto TEXT_WIDTH = 50;
    const auto BIN_HEIGHT = 30;
    auto IMAGE_HEIGHT = BIN_HEIGHT * bins.size();
    const auto BLOCK_WIDTH = 10;
    double top = 0;

    svg_begin(IMAGE_WIDTH, IMAGE_HEIGHT);

    for (int i=0; i<bins.size(); i++) {
        const double bin_width = bins[i] * BLOCK_WIDTH;
        svg_text(TEXT_LEFT, top + TEXT_BASELINE, bins_title[i]);
        std::cout << "\n";
        svg_rect(TEXT_WIDTH, top, bin_width, BIN_HEIGHT, "black", "red");
        std::cout << "\n";
        top += BIN_HEIGHT;
        svg_line(0, top, IMAGE_WIDTH, top, "black", 1, std::to_string(dash_length) + " " + std::to_string(gap_length));
        std::cout << "\n";
    }

    svg_end();

}
