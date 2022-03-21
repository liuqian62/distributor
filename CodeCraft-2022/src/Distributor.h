#include "Site.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
// #include <istream>
// #include <streambuf>
// #include <sstream>
class Distributor
{
private:
    string _Input_demand = "data/demand.csv";
    string _Input_bandwidth = "data/site_bandwidth.csv";
    string _Input_qos = "data/qos.csv";
    string _Input_config = "data/config.ini";
    string _Output = "output/solution.txt";

    int qos_constraint;
    bool is_first_time;
    ofstream ofs;
    vector<Site> sites;
    vector<vector<int>> table;
    vector<vector<int>> var_qoss;
    vector<vector<bool>> status;
    vector<string> customer_ids;
    vector<string> site_names;
    vector<string> times;
    vector<vector<int>> demands;
    vector<vector<int>> distirbute_table;

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
    void do_distribute(int dis);

    /**
     * @brief check is there any customer site do not have enough bandwidth
     * if there is some customer sites do not have enough bandwidth, try to repair
     *
     */
    void final_check(int dis);

/**
 * @brief do optimize to get less cost
 * 
 */
    void optimize();

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