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

    bool hasISO = false;
    size_t isoIndex = 0;
    size_t dateIndex = 0;

    for (size_t i = 0; i < headers.size(); ++i) {
        if (headers[i] == "iso") {
            hasISO = true;
            isoIndex = i;
        }
        if (headers[i] == "date") {
            dateIndex = i;
        }
    }

    for (size_t i = 0; i < headers.size(); ++i) {
        if (headers[i] != "iso" && headers[i] != "date") {
            dataset.columns[headers[i]] = {};
        }
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
        if (hasISO) {
            if (values[isoIndex] != iso) {
                continue;
            }
        }

        dataset.dates.push_back(values[dateIndex]);

        for (size_t i = 1; i < values.size(); ++i) {
            if (headers[i] == "iso" || headers[i] == "date") continue;
            double value = 0.0;

            try {
                value = std::stod(values[i]);
            } catch (...) {
                value = 0.0;
            }

            dataset.columns[headers[i]].push_back(value);
        }
    }

    if (!dataset.hasColumn("inflation_yoy") && !dataset.hasColumn("inflation")) {
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
