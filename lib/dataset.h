#pragma once

#include <string>
#include <unordered_map>
#include <vector>

struct Dataset {
    std::vector<std::string> dates;
    std::unordered_map<std::string, std::vector<double>> columns;

    static Dataset loadCSV(const std::string& path);

    std::size_t size() const;
    bool hasColumn(const std::string& name) const;
    const std::vector<double>& getColumn(const std::string& name) const;
};
