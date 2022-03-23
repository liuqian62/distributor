#include "Distributor.h"

void  Distributor::do_distribute_lesscost(int dis){
    init_distribute_table();


}


void Distributor::get_information_demand(){
    // double average_demand = 0.0;
    double all_demand = 0.0;
    // vector<int> eveytime_demand;
    for (int i = 0; i < demands.size(); i++)
    {
        int tem_demand = 0;
        for (int j = 0; j < customer_ids.size(); j++)
        {
            // tem_demand=0;
            tem_demand += demands[i][j];
            all_demand += demands[i][j];
        }
        eveytime_demand.push_back(tem_demand);
        if(tem_demand>max_demand){
            max_demand=tem_demand;
        }
    }
    average_demand = all_demand / demands.size();
    cout<<"max_demand"<<max_demand<<endl;
    cout<<"average_demand"<<average_demand<<endl;
    for(int i=0;i<times.size();i++){
        // cout<<"demand at "<<times[i]<<":"<<eveytime_demand[i]<<endl;
    }

}

void Distributor::init_distribute_table(){
       // 初始化流量分配
    if (is_first_time)
    {
        for (int i = 0; i < site_names.size(); i++)
        {
            vector<int> temp;
            for (int j = 0; j < customer_ids.size(); j++)
            {

                temp.push_back(0);
            }
            distirbute_table.push_back(temp);
        }
    }
    else
    {
        for (int i = 0; i < site_names.size(); i++)
        {

            for (int j = 0; j < customer_ids.size(); j++)
            {
                distirbute_table[i][j] = 0;
            }
        }
    }
}