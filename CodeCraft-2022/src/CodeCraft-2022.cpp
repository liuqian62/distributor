#include "Site.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>

string Input_demand = "data/demand.csv";
string Input_bandwidth = "data/site_bandwidth.csv";
string Input_qos = "data/qos.csv";
string Input_config = "data/config.ini";
string Output = "output/solution.txt";

using namespace std;

class Distributor
{
private:
    string _Input_demand = "data/demand.csv";
    string _Input_bandwidth = "data/site_bandwidth.csv";
    string _Input_qos = "data/qos.csv";
    string _Input_config = "data/config.ini";
    string _Output = "output/solution.txt";

    int qos_constraint;
    vector<Site> sites;
    vector<vector<int>> table;
    vector<vector<bool>> status;
    vector<string> customer_ids;
    vector<string> site_names;

public:
    /**
     * @brief Construct a new Distributor object
     *
     * @param Input_demand path to demand.csv
     * @param Input_bandwidth path to bandwidth.csv
     * @param Input_qos path to qos.csv
     * @param Input_config path to config.ini
     * @param Output path to save solution.txt
     */
    Distributor(string Input_demand,
                string Input_bandwidth,
                string Input_qos,
                string Input_config,
                string Output);
    /**
     * @brief read data
     *
     */
    void read_data();

    /**
     * @brief read config file
     *
     */
    void read_config();

    /**
     * @brief read site bandwidth
     *
     */
    void read_bandwidth();

    /**
     * @brief read qos
     *
     */
    void read_qos();

    /**
     * @brief read demand file
     *
     */
    void read_demand();

    /**
     * @brief include distribute, check and output
     *
     */
    void distribute();

    /**
     * @brief to distribute with less cost
     *
     */
    void do_distribute();

    /**
     * @brief check is there any customer site do not have enough bandwidth
     * if there is some customer sites do not have enough bandwidth, try to repair
     *
     */
    void final_check();

    /**
     * @brief output the result as result.txt to the right directory
     *
     */
    void output();

    /**
     * @brief Destroy the Distributor object
     *
     */
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

void Distributor::read_config()
{
    ifstream ifs;
    ifs.open(_Input_config, ios::in);

    if (!ifs.is_open())
    {
        cout << "failed to open " << _Input_config << endl;
        return;
    }

    string buf;
    getline(ifs, buf);
    while (getline(ifs, buf))
    {

        char *r = nullptr;

        r = strtok((char *)buf.data(), "=");
        r = strtok(NULL, "=");
        this->qos_constraint = atoi(r);

        cout << qos_constraint << endl;
    }
    ifs.close();
}
void Distributor::read_bandwidth()
{
    ifstream ifs;
    ifs.open(_Input_bandwidth, ios::in);

    if (!ifs.is_open())
    {
        cout << "failed to open " << _Input_bandwidth << endl;
        return;
    }

    string buf;
    getline(ifs, buf);
    while (getline(ifs, buf))
    {

        char *r = nullptr;

        r = strtok((char *)buf.data(), ",");
        string site_name = r;
        r = strtok(NULL, ",");
        int band = atoi(r);
        Site site(site_name, band);
        sites.push_back(site);
    }
    for (int i = 0; i < sites.size(); i++)
    {
        cout << sites[i]._name << endl;
        cout << sites[i].available() << endl;
    }
    ifs.close();
}
void Distributor::read_qos()
{
    ifstream ifs;
    ifs.open(_Input_qos, ios::in);

    if (!ifs.is_open())
    {
        cout << "failed to open " << _Input_qos << endl;
        return;
    }

    string buf;
    char *r = nullptr;
    getline(ifs, buf);
    r = strtok((char *)buf.data(), ",");

    r = strtok(NULL, ",");
    while (r != NULL)
    {
        string customer_id = r;
        customer_ids.push_back(customer_id);
        r = strtok(NULL, ",");
    }

    while (getline(ifs, buf))
    {

        r = strtok((char *)buf.data(), ",");
        string site_name = r;
        site_names.push_back(site_name);
        r = strtok(NULL, ",");
        vector<int> rows;
        vector<bool> statu;
        while (r != NULL)
        {
            int qos = atoi(r);
            if (qos < qos_constraint)
            {
                rows.push_back(sites[site_names.size()]._max_bandwidth);
                statu.push_back(true);
            }
            else
            {
                rows.push_back(0);
                statu.push_back(false);
            }
            r = strtok(NULL, ",");
        }
        table.push_back(rows);
        status.push_back(statu);
    }
    for (int i = 0; i < customer_ids.size(); i++)
    {
        cout << customer_ids[i] << endl;
    }

    for (int i = 0; i < site_names.size(); i++)
    {
        cout << site_names[i] << endl;
    }
    for (int i = 0; i < table.size(); i++)
    {
        for (int j = 0; j < table[0].size(); j++)
        {
            cout << table[i][j] << ",";
        }
        cout << endl;
    }
    for (int i = 0; i < status.size(); i++)
    {
        for (int j = 0; j < status[0].size(); j++)
        {
            cout << status[i][j] << ",";
        }
        cout << endl;
    }
    ifs.close();
}
void Distributor::read_demand()
{
}
// void distribute();
void Distributor::do_distribute()
{
}
void Distributor::final_check()
{
}
void Distributor::output()
{
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