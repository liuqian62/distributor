#include "Site.h"
// #include <string>

using namespace std;

Site::Site(string name, int max_bandwidth)
{
    this->_name = name;
    this->_max_bandwidth = max_bandwidth;
    this->_available_bandwidth = max_bandwidth;
    this->_full_distribute = 0;
    this->_num_distribute = 0;
    this->cost.push_back(0);
    this->full_dised=false;
}

int Site::available()
{
    return this->_available_bandwidth;
}
bool Site::get(int width)
{
    if (_available_bandwidth < width)
    {
        return false;
    }
    else
    {
        _available_bandwidth -= width;

        return true;
    }
}
void Site::final_get(int width)
{
    this->full_dised=false;
    this->_num_distribute += 1;
    if (width >= 0.9*_max_bandwidth)
    {
        // _full_distribute++;
    }
    else
    {
        // int used =this->_max_bandwidth-this->_available_bandwidth;

        if (cost[0] < width)
        {
            // cost.push_back(used);
            // cost[0] = width*0.6+cost[0]*0.4;
            // cost[0] = width;
            cost[0] = width*0.3+cost[0]*0.7;
            // sort(cost.begin(),cost.end());
        }
        else{
// cost[0] = width*0.5+cost[0]*0.5;
// cost[0] = width*0.2+cost[0]*0.8;
        }
        
    }
}
bool Site::full_able()
{
    if (_full_distribute == 0)
    {
        return true;
    }
    if ((_full_distribute + 1) * 20 <= (_num_distribute + 1))
    {
        return true;
    }
    return false;
}

int Site::used(){
    return _max_bandwidth-_available_bandwidth;
}
Site::~Site()
{
}