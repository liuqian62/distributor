#include "Site.h"
// #include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdlib>
#include <ctime>
#include <math.h>
// #include <istream>
// #include <streambuf>
// #include <sstream>
class Distributor
{
private:
    string _Input_demand = "/data/demand.csv";
    string _Input_bandwidth = "/data/site_bandwidth.csv";
    string _Input_qos = "/data/qos.csv";
    string _Input_config = "/data/config.ini";
    string _Output = "/output/solution.txt";

    int qos_constraint;
    bool is_first_time;
    double average_demand;
    int max_demand=0;
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
    vector<int> eveytime_demand;
    vector<int> custormer_num_of_sites;
    vector<int> sort_site_id;
    

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

    void do_distribute_average(int dis);
    void do_distribute_weight(int dis);
    void do_distribute_softmax(int dis);
    void do_distribute_minsite(int dis);
    void do_distribute_lesscost(int dis);
    // void get_information_table();
    void init_distribute_table();
    void get_information_demand();
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
    void optimize(int dis);

    /**
     * @brief output the result as result.txt to the right directory
     *
     */

    void softmax(vector<double> x);

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