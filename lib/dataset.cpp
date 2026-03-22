#include "dataset.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

namespace {

std::vector<std::string> splitCSVLine(const std::string& line) {
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string item;

    while (std::getline(ss, item, ',')) {
        item.erase(item.begin(),
                   std::find_if(item.begin(), item.end(),
                   [](unsigned char ch){ return !std::isspace(ch); }));

        item.erase(std::find_if(item.rbegin(), item.rend(),
                   [](unsigned char ch){ return !std::isspace(ch); }).base(),
                   item.end());

        result.push_back(item);
    }

    return result;
}

}

Dataset Dataset::loadCSV(const std::string& path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        throw std::runtime_error("Dataset::loadCSV: cannot open file");
    }

    Dataset dataset;

    std::string headerLine;
    if (!std::getline(file, headerLine)) {
        throw std::runtime_error("Dataset::loadCSV: empty CSV file");
    }

    auto headers = splitCSVLine(headerLine);

    if (headers.size() < 2) {
        throw std::runtime_error("Dataset::loadCSV: CSV must contain date + at least one column");
    }

    for (size_t i = 1; i < headers.size(); ++i) {
        dataset.columns[headers[i]] = {};
    }

    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        auto values = splitCSVLine(line);

        if (values.size() != headers.size()) {
            continue;
        }

        dataset.dates.push_back(values[0]);

        for (size_t i = 1; i < values.size(); ++i) {
            double value = 0.0;

            try {
                value = std::stod(values[i]);
            } catch (...) {
                value = 0.0;
            }

            dataset.columns[headers[i]].push_back(value);
        }
    }

    if (!dataset.hasColumn("inflation_yoy")) {
        throw std::runtime_error("Dataset::loadCSV: required column inflation_yoy not found");
    }

    if (dataset.size() < 10) {
        throw std::runtime_error("Dataset::loadCSV: dataset too small");
    }

    return dataset;
}

std::size_t Dataset::size() const {
    return dates.size();
}

bool Dataset::hasColumn(const std::string& name) const {
    return columns.find(name) != columns.end();
}

const std::vector<double>& Dataset::getColumn(const std::string& name) const {
    auto it = columns.find(name);

    if (it == columns.end()) {
        throw std::runtime_error("Dataset::getColumn: column not found: " + name);
    }

    return it->second;
}
