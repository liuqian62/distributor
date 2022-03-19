#include "Site.h"
#include <string>

using namespace std;

Site::Site(string name, int max_bandwidth)
{
    this->_name = name;
    this->_max_bandwidth = max_bandwidth;
    this->_available_bandwidth = max_bandwidth;
    this->_full_distribute = 0;
    this->_num_distribute = 0;
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
void Site::add_num()
{
    this->_num_distribute += 1;
    if (_available_bandwidth == 0)
    {
        _full_distribute++;
    }
    else{
                    int used =this->_max_bandwidth-this->_available_bandwidth;
        if(cost.size()==0){

            cost.push_back(used);
        }else{
            if((cost[0]/2)<used){
                cost.push_back(used);
                // sort(cost.begin(),cost.end());
            }
        }
    }
}
Site::~Site()
{
}