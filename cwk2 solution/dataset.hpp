#pragma once

#include <vector>
#include "quake.hpp"

class QuakeDataset
{
  public:
    QuakeDataset() {}
    QuakeDataset(const std::string& filename) { loadData(filename); }
    void loadData(const std::string&);
    int size() const { return data.size(); }
    Quake operator[](int index) const { return data.at(index); }

    // These no longer apply, but we keep them to preserve structure
    Quake strongest() const; 
    Quake shallowest() const; 
    double meanDepth() const;
    double meanMagnitude() const;

  private:
    std::vector<Quake> data;
    void checkDataExists() const;
};
