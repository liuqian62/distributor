#include "Distributor.h"
#include <sys/stat.h>
#include <sys/types.h>

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
    // istringstream sin;
    // sin.str(buf);
    // string customer_id;
    // std::getline(sin, customer_id, ',');
    // while (std::getline(sin, customer_id, ',')) //将字符串流sin中的字符读到field字符串中，以逗号为分隔符
    //     {
    //         customer_ids.push_back(customer_id); //将每一格中的数据逐个push

    //     }

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
}

void Distributor::do_distribute(int dis)
{
    /*

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

                cout << endl;
            }
        }



        // 计算最后的带宽分配

        int count_sum_num = sum_i_bandwith;
        double bandwith_sum = 0.0;
        for (int j = 0; j < edge_length; j++)
        {

            // 平均分配原则
            if (status[j][i]){
                if(count_sum_num >1){
                    distirbute_table[j][i] = status[j][i] * demands[dis][i] /sum_i_bandwith;
                    // 统计已为客户节点分配带宽，解决平均分配小数相加不为0的问题
                    bandwith_sum += distirbute_table[j][i];
                }
                if(count_sum_num ==1){
                    // 最后一个边缘节点承担所有客户节点的剩余带宽需求
                    distirbute_table[j][i] =  demands[dis][i] - bandwith_sum ;

                }
                count_sum_num -=1;

            }
            // 至此，第i个节点的带宽分配完毕
        }
    }
    */

    // 按剩余带宽权重分配；还未使用延时

    // 客户节点长度
    int customer_length = demands[dis].size();
    // 边缘节点长度
    int edge_length = site_names.size();
    // 初始化边缘节点的剩余带宽上限
    vector<double> bandwidth_left;
    for (int j = 0; j < edge_length; j++)
    {
        bandwidth_left.push_back(sites[j].available());
        /*
        cout << "节点" << j << "可用带宽上限" << bandwidth_left[j] <<"";
        cout << endl;
        */
    }

    // 初始化流量分配
    if (is_first_time)
    {
        for (int i = 0; i < edge_length; i++)
        {
            vector<int> temp;
            for (int j = 0; j < customer_length; j++)
            {

                temp.push_back(0);
            }
            distirbute_table.push_back(temp);
        }
    }
    else
    {
        for (int i = 0; i < edge_length; i++)
        {
            // vector<int> temp;
            for (int j = 0; j < customer_length; j++)
            {
                distirbute_table[i][j] = 0;
                // temp.push_back(0);
                // 打印查看，最后关闭
                // cout << distirbute_table[i][j] << " ";
                // cout << endl;
            }
            // distirbute_table.push_back(temp);
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
                // 权重分配原则，但是带宽计数有问题
                // sum_i_bandwith += sites[q].available();
                // 平均分配原则
                sum_i_bandwith += bandwidth_left[q];
                /*
                cout << "带宽" << bandwidth_left[q]<< "求和" << sum_i_bandwith << " ";
                cout << endl;
                */
            }
        }
        // cout<<"客户节点 i 可用带宽"<< sum_i_bandwith << endl;
        //  权重分配原则

        vector<double> weight_d;
        int count_sum_num = 0;
        for (int j = 0; j < edge_length; j++)
        {
            if (status[j][i])
            {
                // 权重weight_d = qos * 边缘节点j的剩余带宽值 / 客户节点 i 的可接触带宽总量
                weight_d.push_back(bandwidth_left[j] / sum_i_bandwith);
                count_sum_num += 1;
                // cout << "节点" << j << i << "延时" << var_qoss[j][i];
                // cout << "权重" << weight_d[j] << "";
                //  cout << endl;
            }

            else
            {
                weight_d.push_back(0.0);
            }
        }
        // cout<<"!!!!"<<endl;
        // 权重求和
        double sum_weight_d = 0;
        for (int i = 0; i < edge_length; i++)
        {
            sum_weight_d += weight_d[i];
        }

        // 计算最后的带宽分配
        // d(i,t) 为demands[dis][i]

        double bandwith_sum = 0.0;
        for (int j = 0; j < edge_length; j++)
        {
            // 权重分配原则

            if (weight_d[j])
            {
                if (count_sum_num > 1)
                {
                    // 未使用延时，只使用边缘结点带宽分配权重
                    distirbute_table[j][i] = weight_d[j] * demands[dis][i];
                    // 统计已为客户节点分配带宽，解决平均分配小数相加不为0的问题
                    bandwith_sum += distirbute_table[j][i];
                    bandwidth_left[j] -= distirbute_table[j][i];

                    // cout << "权重和" << sum_weight_d << "";
                    /*
                    cout << "需求" << demands[dis][i] << "已满足需求" << bandwith_sum ;
                    cout << endl;

                    cout << "节点" << j << "剩余带宽" << bandwidth_left[j] << "" ;
                    cout << endl;
                    */
                }
                if (count_sum_num == 1)
                {
                    // 最后一个边缘节点承担所有客户节点的剩余带宽需求
                    distirbute_table[j][i] = demands[dis][i] - bandwith_sum;
                    bandwidth_left[j] -= distirbute_table[j][i];
                }
                count_sum_num -= 1;
                // 第j个节点剩余带宽 = 第j个节点带宽 -  分配走的带宽
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
    for (int i = 0; i < site_names.size(); i++)
    {
        int tem_used = 0;
        for (int j = 0; j < customer_ids.size(); j++)
        {
            // int tem_used =0;
            tem_used += distirbute_table[i][j];
        }
        sites_used.push_back(tem_used);
        sites_available.push_back(sites[i]._max_bandwidth - tem_used);
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
            cout << "get;" << customers_get[i] << endl;
        }
        else
        {
            // cout<<"sucsess"<<endl;
        }
    }
    for (int i = 0; i < site_names.size(); i++)
    {
        if (sites_available[i] < 0)
        {
            cout << i << sites_available[i] << endl;
            for (int j = 0; j < customer_ids.size(); j++)
            {
                if (status[i][j])
                {
                    for (int k = 0; k < site_names.size(); k++)
                    {
                        if (status[k][j] && sites_available[k] > 0)
                        {

                            distirbute_table[k][j] += sites_available[k];
                            distirbute_table[k][i] -= sites_available[k];
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
        if(sites_available[i] <0)
        {

        }
        }
        else
        {
            // cout<<sites_available[i]<<endl;
        }
    }
}

void Distributor::optimize()
{
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
void Distributor::distribute()
{
     string dir = "/output"; //文件夹路径

      int flag = mkdir(dir.c_str(), 0777);  //Linux创建文件夹
      cout<<flag<<endl;
    ofs.open(_Output, ios::trunc);
    for (int dis = 0; dis < this->times.size(); dis++)
    {
        // cout<<"!!!"<<endl;
        do_distribute(dis);
        // cout<<"!!!"<<endl;
        final_check(dis);
        optimize();
        output();
    }
    ofs.close();
}

Distributor::~Distributor()
{
}