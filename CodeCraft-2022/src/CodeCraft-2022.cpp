#include "Site.h"
// #include <unordered_map>
#include <string>
// #include <vector>
// #include <algorithm>
#include <iostream>
// #include <fstream>
// #include <stdio.h>
// #include <cstring>
#include "Distributor.h"

using namespace std;

// string Input_demand = "/data/demand.csv";
// string Input_bandwidth = "/data/site_bandwidth.csv";
// string Input_qos = "/data/qos.csv";
// string Input_config = "/data/config.ini";
// string Output = "/output/solution.txt";


// string Input_demand = "pressure_data/demand.csv";
// string Input_bandwidth = "pressure_data/site_bandwidth.csv";
// string Input_qos = "pressure_data/qos.csv";
// string Input_config = "pressure_data/config.ini";
// string Output = "output/solution.txt";



string Input_demand = "data/demand.csv";
string Input_bandwidth = "data/site_bandwidth.csv";
string Input_qos = "data/qos.csv";
string Input_config = "data/config.ini";
string Output = "output/solution.txt";



int main()
{
    Distributor distributor(Input_demand,
                            Input_bandwidth,
                            Input_qos,
                            Input_config,
                            Output);
    distributor.read_data();
    distributor.distribute();
    return 0;
}