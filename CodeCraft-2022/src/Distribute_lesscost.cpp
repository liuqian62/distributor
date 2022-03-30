#include "Distributor.h"

void Distributor::do_distribute_lesscost(int dis)
{
    init_distribute_table();
}
void Distributor::BubbleSort(vector<int> &p, int length, int *ind_diff)
{
    for (int m = 0; m < length; m++)
    {
        ind_diff[m] = m;
    }

    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < length - i - 1; j++)
        {
            if (p[j] > p[j + 1])
            {
                int temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;

                int ind_temp = ind_diff[j];
                ind_diff[j] = ind_diff[j + 1];
                ind_diff[j + 1] = ind_temp;
            }
        }
    }
}

void Distributor::get_information_demand()
{
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
        if (tem_demand > max_demand)
        {
            max_demand = tem_demand;
        }
    }
    average_demand = all_demand / demands.size();
    cout << "max_demand" << max_demand << endl;
    cout << "average_demand" << average_demand << endl;
    vector<int> sort_demand(eveytime_demand.begin(), eveytime_demand.end());
    // sort(sort_demand.begin(), sort_demand.end(), greater<int>());
    vector<int> temp_max_demand;

    // int demand_id[sort_demand.size()] = {0};
    // vector<int> tep_free_cost(free_cost.begin(), free_cost.end());
    // BubbleSort(sort_demand, sort_demand.size(), demand_id);
    //    for (int i = 0; i < sort_demand.size(); i++)
    // {
    //     cout << "value: " << sort_demand[i] << "   Index: " << demand_id[i] << endl;

    // }
    int demand_id[sort_demand.size()] = {0};
    // for (int i = 0; i < free_cost.size(); i++)
    // {
    //     cout << "value: " << free_cost[sort_free_id[i]] << "   Index: " << sort_free_id[i] << endl;//from max to min
    // }

    // init every_time_full_id;
    for (int i = 0; i < times.size(); i++)
    {
        vector<int> v;
        every_time_full_id.push_back(v);
    }
    for (int i = 0; i < site_names.size(); i++)
    {
        full_used_time.push_back(0);
    }
    // int num = 3*times.size();
    // while (true)
    // {
    //     num--;
    //     vector<int> new_sort_demand(sort_demand.begin(), sort_demand.end());

    //     BubbleSort(new_sort_demand, new_sort_demand.size(), demand_id); // from min to max
    //     int time_id = demand_id[sort_demand.size() - 1];
    //     if(sort_demand[time_id]<=0){
    //         break;

    //     }
    //     int max_can =0;
    //     int max_can_id =-1;
    //     for (int i = 0; i < sort_free_id.size(); i++)
    //     {
    //         int site_id = sort_free_id[i];
    //         if(free_cost[site_id] == 0){
    //             break;
    //         }
    //         if (full_used_time[site_id]>=times.size() / 20)
    //         {
    //             continue;;
    //         }
    //         bool used=false;
    //         for(int find_id=0;find_id<every_time_full_id[time_id].size();find_id++){
    //             if(site_id==every_time_full_id[time_id][find_id]){
    //                 used =true;
    //                 break;
    //             }
    //         }
    //         if(used){
    //             continue;
    //         }

    //         int can = 0;

    //         for (int k = 0; k < customer_ids.size(); k++)
    //         {

    //             if (status[site_id][k])
    //             {
    //                 can += demands[time_id][k];
    //             }
    //         }
    //         can =min(free_cost[site_id], can);
    //         if(can>max_can){
    //             max_can =can;
    //             max_can_id =site_id;
    //         }
    //     }
    //     if(max_can_id==-1){
    //         break;
    //     }
    //             sort_demand[time_id] -= max_can;
    //         every_time_full_id[time_id].push_back(max_can_id);
    //         full_used_time[max_can_id]++;
    // }
    // best_covered_site_id0 = sort_site_id[0];
    // best_covered_site_id1 = sort_site_id[1];
    for (int i = 0; i < sort_site_id.size(); i++)
    {

        int site_id = sort_site_id[i];
        // if(site_id==sort_site_id[0]){
        //     cout<<custormer_num_of_sites[site_id]<<"!!!"<<endl;
        //     continue;
        // }
        // sort demand
        if (free_cost[site_id] == 0)
        {
            break;
        }
        vector<int> new_sort_demand(sort_demand.begin(), sort_demand.end());

        BubbleSort(new_sort_demand, new_sort_demand.size(), demand_id); // from min to max
        for (int j = 0; j < times.size() / 20; j++)
        {
            // count how much can free
            int time_id = demand_id[sort_demand.size() - j - 1];
            int can = 0;

            for (int k = 0; k < customer_ids.size(); k++)
            {

                if (status[site_id][k])
                {
                    can += demands[time_id][k];
                }
            }
            if (every_time_full_id[time_id].size()>=3)
            {
                sort_demand[time_id] -= 0.8*min(free_cost[site_id], can);
                every_time_full_id[time_id].push_back(site_id);
            }
            else
            {
                sort_demand[time_id] -= min(free_cost[site_id], can);
                every_time_full_id[time_id].push_back(site_id);
            }
        }
    }

    sort(sort_demand.begin(), sort_demand.end(), greater<int>());
    this->expected_cost = sort_demand[0];
    // vector<int> time_custoermer_num;
    // for(int i=0;i<times.size();i++){
    //     int num=0;
    //     for(int j=0;j<customer_ids.size();j++){
    //         for(int k=0;k<every_time_full_id[i].size();k++){
    //             if(status[every_time_full_id[i][k]][j]){
    //                 num ++;
    //                 break;
    //             }
    //         }
    //     }
    //     time_custoermer_num.push_back(num);

    // }
    // int sort_num_id[time_custoermer_num.size()] = {0};
    //     vector<int> new_time_custoermer_num(time_custoermer_num.begin(), time_custoermer_num.end());

    //     BubbleSort(new_time_custoermer_num, time_custoermer_num.size(), sort_num_id); // from min to max

    // int num =0;
    // for(int i=0;i<times.size();i++){
    //     int id=sort_num_id[i];

    //     int thresh_hold=times.size()/20;
    //     if(every_time_full_id[id].size()>0&&num<thresh_hold){
    //         every_time_full_id[id].push_back(best_covered_site_id0);
    //         // cout<<time_custoermer_num[id]<<endl;
    //         num++;
    //     }
    //     //         if(every_time_full_id[id].size()>0&&num>thresh_hold&&num<2*thresh_hold){
    //     //     every_time_full_id[id].push_back(best_covered_site_id1);
    //     //     // cout<<time_custoermer_num[id]<<endl;
    //     //     num++;
    //     // }
    // }

    // for (int i = 0; i < times.size() / 20; i++)
    // {
    //     for (int j = 0; j < free_cost.size(); j++)
    //     {
    //         if (temp_max_demand.size() == sort_demand.size())
    //         {
    //             break;
    //         }
    //         if (temp_max_demand.size() == 0)
    //         {
    //             temp_max_demand.push_back(sort_demand[0]);
    //         }
    //         if (temp_max_demand[0] >= sort_demand[temp_max_demand.size()])
    //         {
    //             temp_max_demand[0] -= xishu * free_cost[j];
    //             sort(temp_max_demand.begin(), temp_max_demand.end(), greater<int>());
    //         }
    //         else
    //         {
    //             temp_max_demand.push_back(sort_demand[temp_max_demand.size()] - xishu * free_cost[j]);
    //             sort(temp_max_demand.begin(), temp_max_demand.end(), greater<int>());
    //         }
    //     }
    // }

    this->expected_cost = max(expected_cost, 0);
    if (this->expected_cost == 0)
    {
        this->expected_cost = 0.1 * this->average_demand;
    }
    cout << "expected_cost" << this->expected_cost << endl;
    // for(int i=0;i<every_time_full_id.size();i++){
    //     for(int j=0;j<every_time_full_id[i].size();j++){
    //         cout<<every_time_full_id[i][j]<<",";
    //     }
    //     cout<<endl;
    // }

    int tep_sort_demand_id[times.size()] = {0};
    vector<int> tep_every_demand(eveytime_demand.begin(), eveytime_demand.end());
    BubbleSort(tep_every_demand, times.size(), tep_sort_demand_id);
    //    for (int i = 0; i < free_cost.size(); i++)
    // {
    //     cout << "value: " << free_cost[i] << "   Index: " << tep_sort_free_id[i] << endl;

    // }
    for (int i = 0; i < times.size(); i++)
    {
        sort_demand_id.push_back(tep_sort_demand_id[eveytime_demand.size() - i - 1]);
    }
    // for (int i = 0; i < eveytime_demand.size(); i++)
    // {
    //     cout << "demand: " << eveytime_demand[sort_demand_id[i]] << "   Index: " << sort_demand_id[i] << endl;
    // }
    // for(int i=0;i<times.size();i++){
    //     cout<<"demand at "<<times[i]<<":"<<sort_demand[i]<<endl;
    // }
    //     for(int i=0;i<site_names.size();i++){
    //     cout<<"free_cost at "<<i<<":"<<free_cost[i]<<"="<<sites[i]._max_bandwidth<<endl;
    // }
}

void Distributor::init_distribute_table()
{
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
    for (int i = 0; i < site_names.size(); i++)
    {
        sites[i]._available_bandwidth = sites[i]._max_bandwidth;
    }
}