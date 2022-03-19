#include "Site.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

string Input_demand = "/data/demand.csv";
string Input_bandwidth = "/data/bandwidth.csv";
string Input_qos = "/data/qos.csv";
string Input_config = "/data/config.ini";
string Output = "/output/solution.txt";

using namespace std;

class Distributor
{
private:
    string _Input_demand = "/data/demand.csv";
    string _Input_bandwidth = "/data/bandwidth.csv";
    string _Input_qos = "/data/qos.csv";
    string _Input_config = "/data/config.ini";
    string _Output = "/output/solution.txt";

    int qos_constraint;

public:
    Distributor(string Input_demand,
                string Input_bandwidth,
                string Input_qos,
                string Input_config,
                string Output);
    void read_data();
    void read_config();
    void read_bandwidth();
    void read_qos();
    void read_demand();
    void distribute();
    void do_distribute();
    void final_check();
    void output();

    ~Distributor();
};

Distributor::Distributor(string Input_demand,
                         string Input_bandwidth,
                         string Input_qos,
                         string Input_config,
                         string Output)
{
    this->_Input_demand = Input_demand;
    this->_Input_bandwidth = Input_bandwidth;
    this->_Input_qos = Input_qos;
    this->_Input_config = Input_config;
    this->_Output = Output;
}
void Distributor::read_data()
{
    read_config();
    read_bandwidth();
    read_qos();
    read_demand();
}

void Distributor::distribute()
{
    do_distribute();
    final_check();
    output();
}

Distributor::~Distributor()
{
}

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