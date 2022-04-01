#include "Distributor.h"

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
    this->is_first_time = true;
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
    // vector<vector<int>> test_size;
    // for(int i=0;i<10;i++){
    //     vector<int>tep;
    //     test_size.push_back(tep);

    // }
    //         cout<<"size:"<<endl;
    //     cout<<test_size.size()<<endl;
    //     cout<<test_size[0].size()<<endl;

    ifstream ifs;
    ifs.open(_Input_config, ios::in);

    if (!ifs.is_open())
    {
        cout << "failed to open " << _Input_config << endl;
        return;
    }

    string buf;
    getline(ifs, buf);
    getline(ifs, buf);
    char *r = nullptr;

    r = strtok((char *)buf.data(), "=");
    r = strtok(NULL, "=");
    this->qos_constraint = atoi(r);

    cout << "qos_constraint " << qos_constraint << endl;
    while (getline(ifs, buf))
    {

        r = strtok((char *)buf.data(), "=");
        r = strtok(NULL, "=");
        this->base_cost = atoi(r);

        cout << "base_cost " << base_cost << endl;
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
        site_map.emplace(site_name, band);
        // cout<<"!!"<<endl;
        // Site site(site_name, band);
        // sites.push_back(site);
    }
    // for (int i = 0; i < sites.size(); i++)
    // {
    //     cout << "边缘节点 name " << sites[i]._name << endl;
    //     cout << "边缘节点 可用带宽 " << sites[i].available() << endl;
    // }
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
        // if(customer_id.find("\r\n")!=-1){
        //     customer_id = strtok((char *)customer_id.data(), "\r\n");
        //     cout<<"!!!"<<endl;
        // }

        customer_ids.push_back(customer_id);
        r = strtok(NULL, ",");
    }

    customer_ids[customer_ids.size() - 1].erase(customer_ids[customer_ids.size() - 1].size() - 1);

    while (getline(ifs, buf))
    {

        r = strtok((char *)buf.data(), ",");
        string site_name = r;
        site_names.push_back(site_name);
        r = strtok(NULL, ",");
        vector<int> rows;
        vector<bool> statu;
        int cus_num = 0;
        while (r != NULL)
        {
            int qos = atoi(r);
            if (qos < qos_constraint)
            {
                // rows.push_back(sites[site_names.size()]._max_bandwidth);
                rows.push_back(site_map[site_name]);
                statu.push_back(true);
                cus_num++;
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
        custormer_num_of_sites.push_back(cus_num);
        if (cus_num != 0)
        {
            // free_cost.push_back(sites[status.size() - 1]._available_bandwidth);
            free_cost.push_back(site_map[site_name]);
        }
        else
        {
            free_cost.push_back(0);
        }
    }
    for (int i = 0; i < site_names.size(); i++)
    {
        Site site(site_names[i], site_map[site_names[i]]);
        sites.push_back(site);
        // cout<<site_names[i]<<site_map[site_names[i]]<<endl;
    }

    int tep_sort_free_id[free_cost.size()] = {0};
    vector<int> tep_free_cost(free_cost.begin(), free_cost.end());
    BubbleSort(tep_free_cost, free_cost.size(), tep_sort_free_id);
    //    for (int i = 0; i < free_cost.size(); i++)
    // {
    //     cout << "value: " << free_cost[i] << "   Index: " << tep_sort_free_id[i] << endl;

    // }
    for (int i = 0; i < free_cost.size(); i++)
    {
        sort_free_id.push_back(tep_sort_free_id[free_cost.size() - i - 1]);
    }
    //        for (int i = 0; i < free_cost.size(); i++)
    // {
    //     cout << "value: " << free_cost[sort_free_id[i]] << "   Index: " << sort_free_id[i] << endl;

    // }

    // for (int i = 0; i < customer_ids.size(); i++)
    // {
    //     cout << "客户节点id " << customer_ids[i] << endl;
    // }

    // for (int i = 0; i < site_names.size(); i++)
    // {
    //     cout << "边缘节点id " << site_names[i] << endl;
    // }
    // for (int i = 0; i < table.size(); i++)
    // {
    //     cout << sites[i]._name << ":";
    //     // cout << "table " << ",";
    //     for (int j = 0; j < table[0].size(); j++)
    //     {
    //         cout << table[i][j] << ",";
    //     }
    //     cout << endl;
    // }
    // for (int i = 0; i < status.size(); i++)
    // {
    //     cout << "状态 "
    //          << ":";
    //     for (int j = 0; j < status[0].size(); j++)
    //     {
    //         cout << status[i][j] << ",";
    //     }
    //     cout << endl;
    // }
    ifs.close();
    for (int i = 0; i <= customer_ids.size(); i++)
    {
        for (int j = 0; j < site_names.size(); j++)
        {
            if (custormer_num_of_sites[j] == customer_ids.size() - i)
            {
                sort_site_id.push_back(j);
            }
        }
    }
    //     for (int i=0;i<sort_site_id.size();i++){
    //     cout << sort_site_id[i]<<":"<<custormer_num_of_sites[sort_site_id[i]] << endl;

    // }

    // get_information_table();
}

void Distributor::read_demand()
{
    ifstream ifs;
    ifs.open(_Input_demand, ios::in);

    if (!ifs.is_open())
    {
        cout << "failed to open " << _Input_demand << endl;
        return;
    }

    string buf;
    char *r = nullptr;
    getline(ifs, buf);
    // getline(ifs, buf);

    r = strtok((char *)buf.data(), ",");

    r = strtok(NULL, ",");
    r = strtok(NULL, ",");
    vector<string> tem_cus_id;
    while (r != NULL)
    {
        string customer_id = r;

        tem_cus_id.push_back(customer_id);
        r = strtok(NULL, ",");
    }

    tem_cus_id[tem_cus_id.size() - 1].erase(tem_cus_id[tem_cus_id.size() - 1].size() - 1);
    unordered_map<string, int> unomap;
    for (int i = 0; i < tem_cus_id.size(); i++)
    {
        unomap.emplace(tem_cus_id[i], i);
    }
    string tep_time_stamp;
    vector<Demans> rows;
    vector<Demans> new_rows;

    while (getline(ifs, buf))
    {

        r = strtok((char *)buf.data(), ",");
        string time_name = r;

        if (tep_time_stamp == time_name)
        {
            r = strtok(NULL, ",");
            string stream_id = r;

            r = strtok(NULL, ",");
            int i = 0;
            while (r != NULL)
            {

                int stream = atoi(r);
                Deman demand;
                demand.stream_id = stream_id;
                demand.stream = stream;
                rows[i].demans.push_back(demand);

                r = strtok(NULL, ",");
                i++;
            }
        }
        else
        {
            if (tep_time_stamp != "")
            {
                for (int i = 0; i < rows.size(); i++)
                {

                    new_rows.push_back(rows[unomap[customer_ids[i]]]);
                }
                demands.push_back(new_rows);
            }
            rows.resize(0);
            new_rows.resize(0);
            tep_time_stamp = time_name;

            times.push_back(time_name);
            r = strtok(NULL, ",");
            string stream_id = r;

            r = strtok(NULL, ",");

            int i = 0;

            while (r != NULL)
            {
                Demans tmp;
                rows.push_back(tmp);

                int stream = atoi(r);

                Deman demand;
                demand.stream_id = stream_id;
                demand.stream = stream;
                rows[i].demans.push_back(demand);

                r = strtok(NULL, ",");
                i++;
            }
        }
    }

    for (int i = 0; i < rows.size(); i++)
    {

        new_rows.push_back(rows[unomap[customer_ids[i]]]);
        // cout<<unomap[customer_ids[i]]<<endl;
    }
    demands.push_back(new_rows);

    // for (int i = 0; i < 1; i++)
    // {
    //     // cout << times[i];
    //     for (int j = 0; j < demands[0].size(); j++)
    //     {
    //         cout << demands[i][j].demans[0].stream << ",";
    //     }
    //     cout << endl;
    // }
    // cout << "demands_size:" << demands.size() << endl;
    // cout << "times_size:" << times.size() << endl;

    ifs.close();
    // get_information_demand();
}
// void Distributor::softmax(vector <double> x) {

// void Distributor::do_distribute_average(int dis)
// {

//     // 平均分配方案

//     // 客户节点长度
//     int customer_length = demands[dis].size();
//     // 边缘节点长度
//     int edge_length = site_names.size();

//     // 初始化流量分配
//     if (is_first_time)
//     {
//         for (int i = 0; i < edge_length; i++)
//         {
//             vector<int> temp;
//             for (int j = 0; j < customer_length; j++)
//             {
//                 // distirbute_table[i][j] = 0;
//                 temp.push_back(0);
//             }
//             distirbute_table.push_back(temp);
//         }
//     }
//     else
//     {
//         for (int i = 0; i < edge_length; i++)
//         {

//             for (int j = 0; j < customer_length; j++)
//             {
//                 distirbute_table[i][j] = 0;
//             }
//         }
//     }

//     // cout<<"!!!!"<<endl;
//     for (int i = 0; i < customer_length; i++)
//     {
//         // sum i-th bandwith
//         int sum_i_bandwith = 0;
//         for (int q = 0; q < edge_length; q++)
//         {
//             if (status[q][i])
//             {

//                 // 平均分配原则，可连接节点状态计数
//                 sum_i_bandwith++;

//                 // cout << endl;
//             }
//         }

//         // 计算最后的带宽分配

//         int count_sum_num = sum_i_bandwith;
//         int bandwith_sum = 0;
//         for (int j = 0; j < edge_length; j++)
//         {

//             // 平均分配原则
//             if (status[j][i])
//             {
//                 if (count_sum_num > 1)
//                 {
//                     distirbute_table[j][i] = demands[dis][i] / sum_i_bandwith;
//                     // 统计已为客户节点分配带宽，解决平均分配小数相加不为0的问题
//                     bandwith_sum += distirbute_table[j][i];
//                 }
//                 if (count_sum_num == 1)
//                 {
//                     // 最后一个边缘节点承担所有客户节点的剩余带宽需求
//                     distirbute_table[j][i] = demands[dis][i] - bandwith_sum;
//                 }
//                 count_sum_num -= 1;
//             }
//             // 至此，第i个节点的带宽分配完毕
//         }
//     }
// }

void Distributor::final_check(int dis)
{
    // cout<<"check"<<endl;
    vector<int> sites_used;
    vector<int> customers_get;
    vector<int> sites_available;
    vector<int> customer_nums; // the number of customer every site can distribute
    for (int i = 0; i < site_names.size(); i++)
    {
        int tem_used = 0;
        int tem_num = 0;
        for (int j = 0; j < customer_ids.size(); j++)
        {
            // int tem_used =0;
            for(int k=0;k<distirbute_table[i].line[j].demans.size();k++){
                tem_used += distirbute_table[i].line[j].demans[k].stream;

            }
            


            if (status[i][j])
            {
                tem_num++;
            }
        }
        sites_used.push_back(tem_used);
        sites_available.push_back(sites[i]._max_bandwidth - tem_used);
        customer_nums.push_back(tem_num);
    }
    for(int i=0;i<site_names.size();i++){
        if(sites_available[i]<0){
            cout<<"!!!!!"<<endl;
        }
    }
    // //  cout<<"check"<<endl;
    // for (int i = 0; i < customer_ids.size(); i++)
    // {
    //     int tem_get = 0;
    //     for (int j = 0; j < site_names.size(); j++)
    //     {
    //         // int tem_used =0;
    //         tem_get += distirbute_table[j][i];
    //     }
    //     customers_get.push_back(tem_get);
    // }
    // //  cout<<"check"<<endl;
    // for (int i = 0; i < customer_ids.size(); i++)
    // {

    //     if (demands[dis][i] != customers_get[i])
    //     {
    //         cout << "distribute false!!!" << endl;
    //         cout << "denmand:" << demands[dis][i] << endl;
    //         cout << "get:" << customers_get[i] << endl;
    //     }
    //     else
    //     {
    //         // cout<<"sucsess"<<endl;
    //     }
    // }
    bool flag = false;
    for (int i = 0; i < site_names.size(); i++)
    {
        if (sites_available[i] < 0)
        {
            int max_id = 0, max_num = 0, cus_id = 0;
            cout << i << sites_available[i] << endl;
            for (int j = 0; j < customer_ids.size(); j++)
            {
                if (status[i][j])
                {
                    for (int k = 0; k < site_names.size(); k++)
                    {

                        if (status[k][j] && sites_available[k] > 0)
                        {
                            if (max_num < customer_nums[k])
                            {
                                max_id = k;
                                cus_id = j;
                                max_num = customer_nums[k];
                            }

                            // distirbute_table[k][j] += sites_available[k];
                            // distirbute_table[i][j] -= sites_available[k];
                            // sites_available[i] += sites_available[k];
                            // sites_available[k] = 0;
                        }
                        if (sites_available[i] >= 0)
                        {
                            break;
                        }
                    }
                }
                if (sites_available[i] >= 0)
                {
                    break;
                }
            }
            if (sites_available[i] < 0)
            {
                // flag = true;
                // distirbute_table[max_id][cus_id] -= sites_available[i];
                // distirbute_table[i][cus_id] += sites_available[i];
                // sites_available[max_id] += sites_available[i];
                // sites_available[i] = 0;
            }
        }
        else
        {
            // cout<<sites_available[i]<<endl;
        }
    }
    if (flag)
    {
        final_check(dis);
    }
}

// void Distributor::optimize(int dis)
// {

//     vector<int> site_max;
//     double sum_max = 0.0;
//     for (int i = 0; i < site_names.size(); i++)
//     {
//         for (int j = 0; j < customer_ids.size(); j++)
//         {
//             if (status[i][j])
//             {
//                 site_max.push_back(sites[i]._max_bandwidth);
//                 sum_max += sites[i]._max_bandwidth;
//                 break;
//             }
//             if (j == customer_ids.size() - 1)
//             {
//                 site_max.push_back(0);
//             }
//         }
//     }

//     // cout<<"find a full"<<endl;
//     vector<int> site_average;
//     for (int i = 0; i < site_names.size(); i++)
//     {
//         // cout<<site_max[i]<<endl;
//         double weight = site_max[i] / sum_max;
//         // site_average.push_back(this->eveytime_demand[dis] * site_max[i] / sum_max);
//         site_average.push_back(this->expected_cost * weight);
//         // cout<<site_average[i]<<endl;
//     }
//     // cout<<"find a full"<<endl;
//     vector<int> site_used;
//     vector<int> cost_add;
//     for (int i = 0; i < site_names.size(); i++)
//     {
//         int tem_used = 0;

//         for (int j = 0; j < customer_ids.size(); j++)
//         {

//             // int tem_used =0;
//             tem_used += distirbute_table[i][j];
//         }
//         site_used.push_back(tem_used);

//         cost_add.push_back(0);
//     }

//     // int tep_demand = this->eveytime_demand[dis];
//     // // int tep_average_demand = average_demand;

//     // vector<int> useful_sort_free_id;
//     // for (int i = 0; i < site_names.size(); i++)
//     // {
//     //     if (sites[sort_free_id[i]]._full_distribute < (demands.size()) / 20)
//     //     {
//     //         if (free_cost[sort_free_id[i]] != 0)
//     //         {
//     //             useful_sort_free_id.push_back(sort_free_id[i]);
//     //         }
//     //     }
//     // }
//     // // for (int i = 0; i < useful_sort_free_id.size(); i++)
//     // // {
//     // //     cout << free_cost[useful_sort_free_id[i]] <<"!!"<< endl;
//     // // }
//     for (int i = 0; i < every_time_full_id[dis].size(); i++)
//     {
//         int id = every_time_full_id[dis][i];
//         cost_add[id] = site_used[id] - sites[id]._max_bandwidth;

//         sites[id]._full_distribute++;
//         sites[id].full_dised = true;
//     }

//     // if (tep_demand > 1.2 * expected_cost)
//     // {
//     //     for (int i = 0; i < useful_sort_free_id.size(); i++)
//     //     {
//     //         if (tep_demand <= 1.0 * expected_cost)
//     //         {
//     //             break;
//     //         }
//     //         if (tep_demand - 1.0 * expected_cost >= free_cost[useful_sort_free_id[i]])
//     //         {
//     //             tep_demand -= free_cost[useful_sort_free_id[i]];
//     //             cost_add[useful_sort_free_id[i]] = site_used[useful_sort_free_id[i]] - sites[useful_sort_free_id[i]]._max_bandwidth;

//     //             sites[useful_sort_free_id[i]]._full_distribute++;
//     //             sites[useful_sort_free_id[i]].full_dised = true;
//     //         }
//     //         else
//     //         {
//     //             if (i == useful_sort_free_id.size() - 2)
//     //             {
//     //                 tep_demand -= free_cost[useful_sort_free_id[i]];
//     //                 cost_add[useful_sort_free_id[i]] = site_used[useful_sort_free_id[i]] - sites[useful_sort_free_id[i]]._max_bandwidth;

//     //                 sites[useful_sort_free_id[i]]._full_distribute++;
//     //                 sites[useful_sort_free_id[i]].full_dised = true;

//     //                 // tep_demand -= free_cost[useful_sort_free_id[i+1]];
//     //                 // cost_add[useful_sort_free_id[i+1]] = site_used[useful_sort_free_id[i+1]] - sites[useful_sort_free_id[i+1]]._max_bandwidth;

//     //                 // sites[useful_sort_free_id[i+1]]._full_distribute++;
//     //                 // sites[useful_sort_free_id[i+1]].full_dised = true;
//     //                 break;
//     //             }

//     //             if (tep_demand - 1.0 * expected_cost < free_cost[useful_sort_free_id[i]] && tep_demand - expected_cost > free_cost[useful_sort_free_id[i + 1]])
//     //             {
//     //                 tep_demand -= free_cost[useful_sort_free_id[i]];
//     //                 cost_add[useful_sort_free_id[i]] = site_used[useful_sort_free_id[i]] - sites[useful_sort_free_id[i]]._max_bandwidth;

//     //                 sites[useful_sort_free_id[i]]._full_distribute++;
//     //                 sites[useful_sort_free_id[i]].full_dised = true;

//     //                 //                     tep_demand -= free_cost[useful_sort_free_id[i+1]];
//     //                 // cost_add[useful_sort_free_id[i+1]] = site_used[useful_sort_free_id[i+1]] - sites[useful_sort_free_id[i+1]]._max_bandwidth;

//     //                 // sites[useful_sort_free_id[i+1]]._full_distribute++;
//     //                 // sites[useful_sort_free_id[i+1]].full_dised = true;
//     //             }
//     //         }
//     //     }
//     // }

//     for (int i = 0; i < site_names.size(); i++)
//     {
//         if (!sites[i].full_dised)
//         {
//             if (custormer_num_of_sites[i] == 0)
//             {
//                 // cost_add.push_back(0);
//                 cost_add[i] = 0;
//                 continue;
//             }
//             cost_add[i] = site_used[i] - 1.0 * site_average[i];
//             // cout<<site_average[i]<<endl;

//             // cost_add[i] = site_used[i] - 1500;
//             // cost_add[i] = site_used[i] -average_demand/site_names.size();
//         }
//     }

//     // for (int i = 0; i < site_names.size(); i++)
//     // {
//     //     cout << cost_add[i] << endl;
//     // }

//     // for (int sort_id = 0; sort_id < site_names.size(); sort_id++)
//     // // for (int i = 0; i < site_names.size(); i++)
//     // {
//     //     int i = sort_site_id[sort_id];

//     //     // cout<<sort_site_id.size()<<endl;
//     //     // cout<<site_names.size()<<endl;
//     //     // cout<<i<<endl;
//     //     int tem_used = 0;

//     //     for (int j = 0; j < customer_ids.size(); j++)
//     //     {

//     //         // int tem_used =0;
//     //         tem_used += distirbute_table[i][j];
//     //     }
//     //     // site_used.push_back(tem_used);
//     //     // cost_add.push_back(tem_used - site_average[i]);

//     //     // cout<<"find a full"<<endl;
//     //     if (custormer_num_of_sites[i] == 0)
//     //     {
//     //         // cost_add.push_back(0);
//     //         cost_add[i] = 0;
//     //         continue;
//     //     }
//     //     // if (tep_demand > 0.5*this->max_demand )
//     //     // if (i%13==0)
//     //     // cout<<custormer_num_of_sites[i]<<endl;
//     //     // if (tep_average_demand >0.6*average_demand)
//     //     // if (custormer_num_of_sites[i] >= customer_ids.size() - 1)

//     //     // if(false)
//     //     // {
//     //     //     // cout<<i<<endl;

//     //     //     // cout<<"find a full"<<endl;
//     //     //     // cost_add.push_back(tem_used - sites[i]._max_bandwidth);
//     //     //     cost_add[i]=tem_used - sites[i]._max_bandwidth;
//     //     //     sites[i]._full_distribute++;

//     //     //     // count++;
//     //     //     // tep_demand -= (sites[i]._max_bandwidth - site_average[i]);
//     //     //     tep_average_demand -= sites[i]._max_bandwidth;
//     //     // }

//     //     // else if (tep_demand > 0.4 * this->max_demand)
//     //     // else if (tep_demand > 1.1* this->average_demand)
//     //     // else if (tep_demand > 0.5* this->average_demand)
//     //     else if (tep_demand > 1.8 * expected_cost)

//     //     {
//     //         // if (sites[i].full_able())
//     //         // if (true)
//     //         if (sites[i]._full_distribute < (demands.size()) / 20)
//     //         {
//     //             // cout<<"find a full"<<endl;
//     //             // cost_add.push_back(tem_used - sites[i]._max_bandwidth);
//     //             cost_add[i] = tem_used - sites[i]._max_bandwidth;

//     //             sites[i]._full_distribute++;
//     //             // count++;
//     //             double lamda = 0;
//     //             if (custormer_num_of_sites[i] >= customer_ids.size() / 4)
//     //             {
//     //                 lamda = 1.0;
//     //             }
//     //             else
//     //             {
//     //                 lamda = 1.0;
//     //             }
//     //             // tep_demand -= (sites[i]._max_bandwidth - site_average[i])*lamda;
//     //             tep_demand -= (sites[i]._max_bandwidth) * lamda;
//     //         }
//     //         else
//     //         {
//     //             // cost_add.push_back(tem_used - 300);
//     //             // cost_add.push_back(tem_used);
//     //             // cost_add[i]=tem_used- 0.1*site_average[i];
//     //             // cost_add[i]=tem_used;
//     //             cost_add[i] = tem_used - 1.5 * site_average[i];
//     //             // cost_add[i]=tem_used- (1.1-(1.1*average_demand-tep_demand)/average_demand)*site_average[i];
//     //             // cost_add[i]=tem_used- sites[i].cost[0];
//     //             // cost_add[i]=tem_used-500;
//     //             // cost_add.push_back(tem_used - 0.3*site_average[i]);
//     //             continue;
//     //         }
//     //     }
//     //     else
//     //     {
//     //         // cost_add.push_back(tem_used - sites[i].cost[0]);
//     //         // cost_add.push_back(tem_used - 0.3*site_average[i]);
//     //         // cost_add.push_back(tem_used);
//     //         cost_add[i] = tem_used - 1.5 * site_average[i];
//     //         //  cost_add[i]=tem_used;
//     //         // cost_add[i]=tem_used- 0.1*site_average[i];
//     //         // cost_add[i]=tem_used- (1.1-(1.1*average_demand-tep_demand)/average_demand)*site_average[i];
//     //         // cost_add[i]=tem_used- sites[i].cost[0];
//     //         //  cost_add[i]=tem_used-500;
//     //         // cost_add.push_back(tem_used - 300);
//     //         // sites_available.push_back(sites[i]._max_bandwidth - tem_used);
//     //     }
//     // }

//     if (true)
//     {
//         for (int i = 0; i < site_names.size(); i++)
//         {
//             //    int i=sort_site_id[id];
//             while (cost_add[i] < 0)
//             {
//                 int max_cost_add = 0;
//                 int max_id = -1;
//                 int can_add = 0;
//                 for (int j = 0; j < site_names.size(); j++)
//                 {

//                     if (cost_add[j] > max_cost_add)
//                     {
//                         for (int m = 0; m < customer_ids.size(); m++)
//                         {
//                             if (status[i][m] && status[j][m] && min(distirbute_table[i][m], distirbute_table[j][m]) != 0)
//                             {
//                                 max_cost_add = cost_add[j];
//                                 max_id = j;
//                             }
//                         }
//                     }
//                 }
//                 if (max_id != -1)
//                 {
//                     int j = max_id;
//                     for (int k = 0; k < customer_ids.size(); k++)
//                     {
//                         can_add = 0;
//                         if (status[i][k] && status[j][k])
//                         {
//                             // int can = min(distirbute_table[i][k], distirbute_table[j][k]);
//                             int can = distirbute_table[j][k];
//                             int want_have = 0;
//                             if (cost_add[j] > 10)
//                             {
//                                 want_have = 0.5 * cost_add[j];
//                             }
//                             else
//                             {
//                                 want_have = cost_add[j];
//                             }
//                             // want_have = cost_add[j] + 0.1 * site_average[j];
//                             int have = min(abs(cost_add[i]), want_have);
//                             // if(can==0){
//                             //     max_cost_add =0;
//                             // }
//                             can_add += can;
//                             if (can < have)
//                             {
//                                 distirbute_table[i][k] += can;
//                                 distirbute_table[j][k] -= can;
//                                 cost_add[i] += can;
//                                 cost_add[j] -= can;
//                             }
//                             else
//                             {
//                                 distirbute_table[i][k] += have;
//                                 distirbute_table[j][k] -= have;
//                                 cost_add[i] += have;
//                                 cost_add[j] -= have;
//                             }
//                             if (cost_add[i] == 0)
//                             {
//                                 break;
//                             }
//                         }
//                     }
//                 }
//                 if (max_id == -1)
//                 {
//                     break;
//                 }
//             }
//         }
//     }
//     int num = 55;
//     while (num > 0)
//     // while (false)
//     {
//         num--;
//         vector<int> tep_cost_add(cost_add.begin(), cost_add.end());
//         int tep_sort_cost_add_id[cost_add.size()] = {0};
//         BubbleSort(tep_cost_add, cost_add.size(), tep_sort_cost_add_id);
//         for (int i = 0; i < cost_add.size(); i++)
//         {
//             int max_id = tep_sort_cost_add_id[cost_add.size() - 1 - i];
//             if (cost_add[max_id] < 0)
//             {
//                 break;
//             }
//             for (int j = 0; j < cost_add.size(); j++)
//             {
//                 int min_id = tep_sort_cost_add_id[j];
//                 if (cost_add[max_id] <= cost_add[min_id])
//                 {
//                     break;
//                 }
//                 if (custormer_num_of_sites[min_id] != 0)
//                 {
//                     for (int k = 0; k < customer_ids.size(); k++)
//                     {
//                         if (status[max_id][k] && status[min_id][k] && distirbute_table[max_id][k] != 0)
//                         {
//                             int change_num = (cost_add[max_id] - cost_add[min_id]) / 2;
//                             int have = sites[min_id]._max_bandwidth;
//                             for (int m = 0; m < customer_ids.size(); m++)
//                             {
//                                 have -= distirbute_table[min_id][m];
//                             }
//                             int want_change = min(change_num, have);
//                             int can = distirbute_table[max_id][k];
//                             if (can > want_change)
//                             {
//                                 distirbute_table[max_id][k] -= want_change;
//                                 distirbute_table[min_id][k] += want_change;
//                                 cost_add[max_id] -= want_change;
//                                 cost_add[min_id] += want_change;
//                             }
//                             else
//                             {
//                                 distirbute_table[max_id][k] -= can;
//                                 distirbute_table[min_id][k] += can;
//                                 cost_add[max_id] -= can;
//                                 cost_add[min_id] += can;
//                             }
//                         }
//                     }
//                 }
//             }
//         }
//     }

//     // sort(cost_add.begin(), cost_add.end());
//     // cout << cost_add[cost_add.size() - 1] << endl;
//     for (int i = 0; i < site_names.size(); i++)
//     {
//         int tem_used = 0;
//         // int tem_num = 0;
//         for (int j = 0; j < customer_ids.size(); j++)
//         {
//             // int tem_used =0;
//             tem_used += distirbute_table[i][j];
//         }
//         sites[i].final_get(tem_used);
//     }
// }

void Distributor::distribute()
{
    string dir = "./output"; //文件夹路径

    // int flag = mkdir(dir.c_str(), S_IRWXU);  //Linux创建文件夹
    ofs.open(_Output, ios::trunc);
    for (int dis = 0; dis < this->times.size(); dis++)
    {
        // cout<<"!!!"<<endl;nabieren
        do_distribute(dis);
        // cout<<"!!!"<<endl;
        // final_check(dis);
        // final_check(dis);
        // optimize(dis);
        output();
        // cout<<sites[0]._max_bandwidth*0.95<<endl;
    }
    ofs.close();
}

void Distributor::output()
{

    for (int i = 0; i < customer_ids.size(); i++)
    {
        bool is_first = true;

        // if (i == distirbute_table[0].size() - 1)
        // {
        //     if (is_first_time)
        //     {
        //         // ofs<<customer_ids[i].size();
        //         // customer_ids[i].erase(customer_ids[i].size() - 1);
        //         ofs << customer_ids[i] << ":";
        //         // ofs<<customer_ids[i]<<":";
        //         is_first_time = false;
        //     }
        //     else
        //     {
        //         ofs << customer_ids[i] << ":";
        //     }
        // }
        // else
        // {
            ofs << customer_ids[i] << ":";
        // }
        for (int j = 0; j < distirbute_table.size(); j++)
        {
            if(distirbute_table[j].line[i].demans.size()!=0){
                if (is_first)
                {
                    is_first = false;
                }
                else
                {
                    ofs << ",";
                }
                ofs << "<" << site_names[j] ;
                for(int k=0;k<distirbute_table[j].line[i].demans.size();k++){
                    ofs<< "," << distirbute_table[j].line[i].demans[k].stream_id ;
                }
                ofs<< ">";

            }
                
            

            }
  
        ofs << endl;
    }
}

Distributor::~Distributor()
{
}