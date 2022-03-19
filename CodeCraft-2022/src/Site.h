#ifndef __SITE_H__
#define __SITE_H__

#include <iostream>
#include <string>
#include <vector>

class Site
{
private:
    string _name;
    int _max_bandwidth;
    int _available_bandwidth;
    int _full_distribute;
    int _num_distribute;
    vector<int> cost;

public:
    Site(string name, int max_bandwidth);
    int available();
    bool get(int width);
    void add_num();
    ~Site();
};

