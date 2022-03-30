#ifndef __SITE_H__
#define __SITE_H__

#include <iostream>
#include <string>
#include <vector>
// #include <algorithm>
using namespace std;

class Site
{
private:


public:
    string _name;
    int _max_bandwidth;
    int _available_bandwidth;
    int _full_distribute;
    int _num_distribute;
    bool full_dised;
    vector<int> cost;

    Site(string name, int max_bandwidth);
    int available();
    bool get(int width);
    void final_get(int width);
    bool full_able();
    int used();
    ~Site();
};

#endif