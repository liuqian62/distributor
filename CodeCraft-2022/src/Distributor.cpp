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

        cout << "qos_constraint " << qos_constraint << endl;
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
        // if(customer_id.find(" ")!=-1&&customer_id.find("\t")!=-1&&customer_id.find("\n")!=-1){
        //     customer_id = strtok((char *)customer_id.data(), " ");
        //     cout<<"!!!"<<endl;
        // }

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
        int cus_num = 0;
        while (r != NULL)
        {
            int qos = atoi(r);
            if (qos < qos_constraint)
            {
                rows.push_back(sites[site_names.size()]._max_bandwidth);
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
    }
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
    // r = strtok((char *)buf.data(), ",");

    // r = strtok(NULL, ",");
    // while (r != NULL)
    // {
    //     string customer_id = r;
    //     customer_ids.push_back(customer_id);
    //     r = strtok(NULL, ",");
    // }

    while (getline(ifs, buf))
    {

        r = strtok((char *)buf.data(), ",");
        string time_name = r;
        times.push_back(time_name);
        r = strtok(NULL, ",");
        vector<int> rows;
        // vector<bool> statu;
        while (r != NULL)
        {
            int demand = atoi(r);
            rows.push_back(demand);
            r = strtok(NULL, ",");
        }
        demands.push_back(rows);
    }

    // for (int i = 0; i < demands.size(); i++)
    // {
    //     cout << times[i];
    //     for (int j = 0; j < demands[0].size(); j++)
    //     {
    //         cout << demands[i][j] << ",";
    //     }
    //     cout << endl;
    // }

    ifs.close();
    get_information_demand();
}
// void Distributor::softmax(vector <double> x) {

void Distributor::do_distribute_average(int dis)
{

    // 平均分配方案

    // 客户节点长度
    int customer_length = demands[dis].size();
    // 边缘节点长度
    int edge_length = site_names.size();

    // 初始化流量分配
    if (is_first_time)
    {
        for (int i = 0; i < edge_length; i++)
        {
            vector<int> temp;
            for (int j = 0; j < customer_length; j++)
            {
                // distirbute_table[i][j] = 0;
                temp.push_back(0);
            }
            distirbute_table.push_back(temp);
        }
    }
    else
    {
        for (int i = 0; i < edge_length; i++)
        {

            for (int j = 0; j < customer_length; j++)
            {
                distirbute_table[i][j] = 0;
            }
        }
    }

    // cout<<"!!!!"<<endl;
    for (int i = 0; i < customer_length; i++)
    {
        // sum i-th bandwith
        double sum_i_bandwith = 0.0;
        for (int q = 0; q < edge_length; q++)
        {
            if (status[q][i])
            {

                // 平均分配原则，可连接节点状态计数
                sum_i_bandwith += status[q][i];

                // cout << endl;
            }
        }

        // 计算最后的带宽分配

        int count_sum_num = sum_i_bandwith;
        double bandwith_sum = 0.0;
        for (int j = 0; j < edge_length; j++)
        {

            // 平均分配原则
            if (status[j][i])
            {
                if (count_sum_num > 1)
                {
                    distirbute_table[j][i] = status[j][i] * demands[dis][i] / sum_i_bandwith;
                    // 统计已为客户节点分配带宽，解决平均分配小数相加不为0的问题
                    bandwith_sum += distirbute_table[j][i];
                }
                if (count_sum_num == 1)
                {
                    // 最后一个边缘节点承担所有客户节点的剩余带宽需求
                    distirbute_table[j][i] = demands[dis][i] - bandwith_sum;
                }
                count_sum_num -= 1;
            }
            // 至此，第i个节点的带宽分配完毕
        }
    }
}

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
            tem_used += distirbute_table[i][j];
            if (distirbute_table[i][j] < 0)
            {
                cout << "get a num < 0 at (" << i << "," << j << ")" << endl;
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
    //  cout<<"check"<<endl;
    for (int i = 0; i < customer_ids.size(); i++)
    {
        int tem_get = 0;
        for (int j = 0; j < site_names.size(); j++)
        {
            // int tem_used =0;
            tem_get += distirbute_table[j][i];
        }
        customers_get.push_back(tem_get);
    }
    //  cout<<"check"<<endl;
    for (int i = 0; i < customer_ids.size(); i++)
    {

        if (demands[dis][i] != customers_get[i])
        {
            cout << "distribute false!!!" << endl;
            cout << "denmand:" << demands[dis][i] << endl;
            cout << "get:" << customers_get[i] << endl;
        }
        else
        {
            // cout<<"sucsess"<<endl;
        }
    }
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

                            distirbute_table[k][j] += sites_available[k];
                            distirbute_table[i][j] -= sites_available[k];
                            sites_available[i] += sites_available[k];
                            sites_available[k] = 0;
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
                flag = true;
                distirbute_table[max_id][cus_id] -= sites_available[i];
                distirbute_table[i][cus_id] += sites_available[i];
                sites_available[max_id] += sites_available[i];
                sites_available[i] = 0;
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

void Distributor::optimize(int dis)
{
    vector<int> site_max;
    double sum_max = 0.0;
    for (int i = 0; i < site_names.size(); i++)
    {
        for (int j = 0; j < customer_ids.size(); j++)
        {
            if (status[i][j])
            {
                site_max.push_back(sites[i]._max_bandwidth);
                sum_max += sites[i]._max_bandwidth;
                break;
            }
            if (j == customer_ids.size() - 1)
            {
                site_max.push_back(0);
            }
        }
    }

    // cout<<"find a full"<<endl;
    vector<int> site_average;
    for (int i = 0; i < site_names.size(); i++)
    {
        // cout<<"find a full"<<i<<endl;
        // cout<<dis<<endl;
        // cout<<this->eveytime_demand.size()<<endl;
        // site_average.push_back(this->eveytime_demand[dis] * site_max[i] / sum_max);
        site_average.push_back(this->average_demand * site_max[i] / sum_max);
        // cout<<site_average[i]<<endl;
    }
    // cout<<"find a full"<<endl;
    vector<int> site_used;
    vector<int> cost_add;
    for(int i=0;i<site_names.size();i++){
        cost_add.push_back(0);
    }

    int tep_demand = this->eveytime_demand[dis];
    int tep_average_demand = average_demand;
    int count = 0;
    for (int sort_id = 0; sort_id < site_names.size(); sort_id++)
    {
        int i = sort_site_id[sort_id];
        // cout<<sort_site_id.size()<<endl;
        // cout<<site_names.size()<<endl;
        // cout<<i<<endl;
        int tem_used = 0;
        int tem_num = 0;
        for (int j = 0; j < customer_ids.size(); j++)
        {

            // int tem_used =0;
            tem_used += distirbute_table[i][j];
        }
        // site_used.push_back(tem_used);
        // cost_add.push_back(tem_used - site_average[i]);

        // cout<<"find a full"<<endl;
        if (custormer_num_of_sites[i] == 0)
        {
            // cost_add.push_back(0);
            cost_add[i]=0;
            continue;
        }
        // if (tep_demand > 0.5*this->max_demand )
        // if (i%13==0)
        // cout<<custormer_num_of_sites[i]<<endl;
        if (tep_average_demand >0)
        // if (custormer_num_of_sites[i] >= customer_ids.size() - 1)
        // if(false)
        {
            // cout<<i<<endl;

            // cout<<"find a full"<<endl;
            // cost_add.push_back(tem_used - sites[i]._max_bandwidth);
            cost_add[i]=tem_used - sites[i]._max_bandwidth;
            sites[i]._full_distribute++;

            // count++;
            tep_demand -= (sites[i]._max_bandwidth - site_average[i]);
            tep_average_demand -= sites[i]._max_bandwidth;
        }
        // else if (tep_demand > 0.4 * this->max_demand)
        else if (tep_demand > 1.0 * this->average_demand)
        {
            if (sites[i].full_able())
            // if (true)
            {
                // cout<<"find a full"<<endl;
                // cost_add.push_back(tem_used - sites[i]._max_bandwidth);
                cost_add[i]=tem_used - sites[i]._max_bandwidth;

                sites[i]._full_distribute++;
                // count++;
                tep_demand -= (sites[i]._max_bandwidth - site_average[i]);
            }
            else
            {
                // cost_add.push_back(tem_used - 300);
                // cost_add.push_back(tem_used);
                cost_add[i]=tem_used;
                // cost_add.push_back(tem_used - 0.3*site_average[i]);
                continue;
            }
        }
        else
        {
            // cost_add.push_back(tem_used - sites[i].cost[0]);
            // cost_add.push_back(tem_used - 0.3*site_average[i]);
            // cost_add.push_back(tem_used);
             cost_add[i]=tem_used;
            // cost_add.push_back(tem_used - 300);
            // sites_available.push_back(sites[i]._max_bandwidth - tem_used);
        }
    }
    if (true)
    {
        for (int i = 0; i < site_names.size(); i++)
        {
        //    int i=sort_site_id[id];
            while (cost_add[i] < 0)
            {
                int max_cost_add = 0;
                int max_id = -1;
                int can_add = 0;
                for (int j = 0; j < site_names.size(); j++)
                {

                    if (cost_add[j] > max_cost_add)
                    {
                        for (int m = 0; m < customer_ids.size(); m++)
                        {
                            if (status[i][m] && status[j][m] && min(distirbute_table[i][m], distirbute_table[j][m]) != 0)
                            {
                                max_cost_add = cost_add[j];
                                max_id = j;
                            }
                        }
                    }
                }
                if (max_id != -1)
                {
                    int j = max_id;
                    for (int k = 0; k < customer_ids.size(); k++)
                    {
                        can_add = 0;
                        if (status[i][k] && status[j][k])
                        {
                            int can = min(distirbute_table[i][k], distirbute_table[j][k]);
                            int have = min(abs(cost_add[i]), cost_add[j]);
                            // if(can==0){
                            //     max_cost_add =0;
                            // }
                            can_add += can;
                            if (can < have)
                            {
                                distirbute_table[i][k] += can;
                                distirbute_table[j][k] -= can;
                                cost_add[i] += can;
                                cost_add[j] -= can;
                            }
                            else
                            {
                                distirbute_table[i][k] += have;
                                distirbute_table[j][k] -= have;
                                cost_add[i] += have;
                                cost_add[j] -= have;
                            }
                            if (cost_add[i] == 0)
                            {
                                break;
                            }
                        }
                    }
                }
                if (max_id == -1)
                {
                    break;
                }
            }
        }
    }

    for (int i = 0; i < site_names.size(); i++)
    {
        int tem_used = 0;
        // int tem_num = 0;
        for (int j = 0; j < customer_ids.size(); j++)
        {
            // int tem_used =0;
            tem_used += distirbute_table[i][j];
        }
        sites[i].final_get(tem_used);
    }
}

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
        final_check(dis);
        // final_check(dis);
        optimize(dis);
        output();
        // cout<<sites[0]._max_bandwidth*0.95<<endl;
    }
    ofs.close();
}

void Distributor::output()
{

    for (int i = 0; i < distirbute_table[0].size(); i++)
    {
        bool is_first = true;

        if (i == distirbute_table[0].size() - 1)
        {
            if (is_first_time)
            {
                // ofs<<customer_ids[i].size();
                customer_ids[i].erase(customer_ids[i].size() - 1);
                ofs << customer_ids[i] << ":";
                // ofs<<customer_ids[i]<<":";
                is_first_time = false;
            }
            else
            {
                ofs << customer_ids[i] << ":";
            }
        }
        else
        {
            ofs << customer_ids[i] << ":";
        }
        for (int j = 0; j < distirbute_table.size(); j++)
        {
            if (distirbute_table[j][i] != 0)
            {
                if (is_first)
                {
                    is_first = false;
                }
                else
                {
                    ofs << ",";
                }
                ofs << "<" << site_names[j] << "," << distirbute_table[j][i] << ">";
            }
        }
        ofs << endl;
    }
}

Distributor::~Distributor()
{
}