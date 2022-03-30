#include "Distributor.h"

void Distributor::do_distribute_weight(int dis)
{

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

            for (int j = 0; j < customer_length; j++)
            {
                distirbute_table[i][j] = 0;
            }
        }
    }

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
        // cout<<sum_weight_d<<"!!"<<endl;

        // 计算最后的带宽分配
        // d(i,t) 为demands[dis][i]
        int n = 99;

        float random = rand() % (n + 1) / (float)(n + 1);
        // cout << "rand"  << random << endl;

        int bandwith_sum = 0;
        for (int j = 0; j < edge_length; j++)
        {
            // 权重分配原则

            if (weight_d[j]>0)
            {
                if (count_sum_num > 1)
                {
                    // 未使用延时，只使用边缘结点带宽分配权重
                    distirbute_table[j][i] = weight_d[j] * demands[dis][i];
                    // 随机权重
                    // distirbute_table[j][i] = random * demands[dis][i];
                    // 统计已为客户节点分配带宽，解决平均分配小数相加不为0的问题
                    bandwith_sum += distirbute_table[j][i];
                    if (bandwith_sum > demands[dis][i])
                    {
                        distirbute_table[j][i] = distirbute_table[j][i] - (bandwith_sum - demands[dis][i]);
                        break;
                    }
                    bandwidth_left[j] -= distirbute_table[j][i];

                    // cout << "权重和" << sum_weight_d << "";
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
void Distributor::do_distribute_softmax(int dis)
{
    // softmax归一化权重分配

    // 客户节点长度
    int customer_length = demands[dis].size();
    // 边缘节点长度
    int edge_length = site_names.size();
    // 初始化边缘节点的剩余带宽上限
    vector<double> bandwidth_left;
    for (int j = 0; j < edge_length; j++)
    {
        bandwidth_left.push_back(sites[j].available());
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

    for (int i = 0; i < customer_length; i++)
    {
        // sum i-th bandwith
        double sum_i_bandwith = 0.0;
        int edge_count = 0;
        for (int q = 0; q < edge_length; q++)
        {
            if (status[q][i])
            {
                // 权重分配原则，但是带宽计数有问题
                // sum_i_bandwith += sites[q].available();
                // 平均分配原则
                sum_i_bandwith += bandwidth_left[q];
                edge_count += 1;
            }
        }
        // cout<<"客户节点 i 可用带宽"<< sum_i_bandwith << endl;
        //  随机权重产生
        // 产生随机数

        vector<double> random;
        srand((int)time(NULL));
        for (int i = 0; i < edge_count; i++)
        {
            int num = rand() % 10;
            // cout << rand()%100<< endl;
            // random.push_back( rand() % (n + 1) / (float)(n + 1));
            random.push_back(num);
        }
        // 对权重作softmax归一化
        vector<float> weight_random;
        vector<float> exp_random;
        // cout << "边缘节点数量" << edge_count<< endl;

        float sum = 0.0;

        for (int i = 0; i < edge_count; i++)
        {
            exp_random.push_back(random[i]);

            // cout << exp_random[i]<< endl;
        }
        for (int i = 0; i < edge_count; i++)
        {

            sum += exp_random[i];
        }
        for (int i = 0; i < edge_count; i++)
        {

            weight_random.push_back(exp_random[i] / sum);
        }
        // 查看weight_random
        /*

        float sum_weight_r = 0.0;
        for (int j = 0; j < edge_count; j++){
            sum_weight_r += weight_random[j];

        }

        cout << "sum weight_d " << sum_weight_r << endl;
        */

        int count_sum_num = edge_count;
        double bandwith_sum = 0.0;
        for (int j = 0; j < edge_length; j++)
        {
            // 权重分配原则

            if (status[j][i])
            {
                if (count_sum_num > 0)
                {
                    // 未使用延时，只使用边缘结点带宽分配权重
                    // cout << "权重" << weight_random[count_sum_num-1] << endl;
                    distirbute_table[j][i] = weight_random[edge_count - count_sum_num] * demands[dis][i];
                    // cout << "weight " << weight_random[edge_count-count_sum_num] ;
                    //  随机权重
                    //  distirbute_table[j][i] = random * demands[dis][i];
                    //  统计已为客户节点分配带宽，解决平均分配小数相加不为0的问题
                    bandwith_sum += distirbute_table[j][i];
                    if (bandwith_sum > demands[dis][i])
                    {
                        distirbute_table[j][i] = distirbute_table[j][i] + (demands[dis][i] - bandwith_sum);
                        bandwith_sum += (demands[dis][i] - bandwith_sum);
                        // cout << demands[dis][i]- bandwith_sum<< endl;
                        break;
                    }
                    if (bandwith_sum < demands[dis][i] and count_sum_num == 1)
                    {
                        distirbute_table[j][i] = distirbute_table[j][i] + (demands[dis][i] - bandwith_sum);
                        bandwith_sum += (demands[dis][i] - bandwith_sum);
                        // cout << demands[dis][i]- bandwith_sum<< endl;
                    }

                    bandwidth_left[j] -= distirbute_table[j][i];

                    // cout << "权重和" << sum_weight_d << "";
                }
                /*
                if (count_sum_num == 1)
                {
                    // 最后一个边缘节点承担所有客户节点的剩余带宽需求
                    distirbute_table[j][i] = demands[dis][i] - bandwith_sum;
                    bandwidth_left[j] -= distirbute_table[j][i];
                }
                */
            count_sum_num -= 1;
            if (distirbute_table[j][i] < 0)
            {
                cout << "error" << endl;
            }
                // 第j个节点剩余带宽 = 第j个节点带宽 -  分配走的带宽
            }
            // 至此，第i个节点的带宽分配完毕
        }
    }
}

void Distributor::do_distribute_minsite(int dis)
{
    

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
    // 26 节点 AS 31 Ag节点

    /*
    for (int i = 0; i < edge_length; i++){
    cout << "节点" << i << "name " << site_names[i] << endl;
    }
    */

    // 实现最少节点搜索实现，并将节点划分为 main_edge  和 lazy_edge

    vector<int> main_edge;
    vector<int> lazy_edge;
    vector<bool> lazy_edge_statu;
    
    main_edge.push_back(26);
    main_edge.push_back(31);



    // 策略，大流量时段先使用lazy_edge;lazy_edge使用结束后使用main_edge;小流量时段拟采用平均或权重分配原则



    for (int i = 0; i < customer_length; i++)
    {
        // 只采用几个主要节点来实现带宽分配

        
        for (int j =0 ;j =main_edge.size();j++){

            //j ,i 节点有链接，且节点j有带宽可用
            if (status[main_edge[j]][i] and sites[main_edge[j]].available()){
                
                distirbute_table[main_edge[j]][i] = demands[dis][i] ;
                //cout << " 已分配" << distirbute_table[24][i] << "需求"  << demands[dis][i];
                sites[main_edge[j]].get(distirbute_table[main_edge[j]][i]);
                // 统计已为客户节点分配带宽，解决平均分配小数相加不为0的问题
            }
     

        }

            // 至此，第i个节点的带宽分配完毕
        
    }
    

}

void Distributor::do_distribute(int dis)
{
    // do_distribute_average(dis);
    do_distribute_weight(dis);
    // do_distribute_softmax(dis);
    // do_distribute_minsite(dis);

    // do_distribute_lesscost(dis);

    // sites[0].get(50);//get 50 from site0;
    // int num = sites[0].used();//site0 used;
}