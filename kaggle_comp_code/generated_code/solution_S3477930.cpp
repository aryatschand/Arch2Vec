
    #include <iostream>
    #include <map>
    #include <string>
    using namespace std;
     
    int main(int argc, char* argv[])
    {
    	int test_cases;
    	int hints;
    	cin >> test_cases;
    	if((test_cases > 20) || (test_cases < 0))
    	{
    		cout << " Cases Out of bounds";
    		return 0;
    	}
    	while(test_cases--)
    	{
    		map<int,int> less_than,greater_than,equal;
    		int sum_less_true_total = 0,sum_greater_true_total = 0;
    		int total_true;
    		map<int,int>::iterator it1,it2,it3;
    		char op;
    		string dec;
    		int log_val;
    		cin >> hints;
    		if((hints < 1) || (hints > 100000))
    		{
    			cout << "Hints out of bounds";
    			return 0;
    		}
    		for(int i = 0; i < hints; i++)
    		{
    			cin >> op >> log_val >> dec;
    			if((log_val < 1) || (log_val > 1000000000))
    			{
    				cout << "Value out of bounds";
    				return 0;
    			}
    			if((op == '<') && (dec.compare("Yes") == 0))
    			{
    				if(log_val > 1)
    				less_than[log_val - 1] += 1;
    				else
    				less_than[log_val - 1] += 0;
    				greater_than[log_val - 1] += 0;
    				equal[log_val - 1] += 0;
    			}
    			else if((op == '<') && (dec.compare("No") == 0))
    			{
    				greater_than[log_val] += 1;
    				less_than[log_val] += 0;
    				equal[log_val] += 0;
    			}
    			else if((op == '>') && (dec.compare("Yes") == 0))
    			{
    				if(log_val < 1000000000)
    				greater_than[log_val + 1] += 1;
    				else
    				greater_than[log_val + 1] += 0;
    				less_than[log_val + 1] += 0;
    				equal[log_val + 1] += 0;
    			}
    			else if((op == '>') && (dec.compare("No") == 0))
    			{
    				less_than[log_val] += 1;
    				greater_than[log_val] += 0;
    				equal[log_val] += 0;
    			}
    			else if((op == '=') && (dec.compare("Yes") == 0))
    			{
    				equal[log_val] += 1;
    				less_than[log_val] += 0;
    				greater_than[log_val] += 0;
    			}
    			else if((op == '=') && (dec.compare("No") == 0))
    			{
    				less_than[log_val - 1] += 1;
    				less_than[log_val + 1] += 0;
    				greater_than[log_val + 1] += 1;
    				greater_than[log_val - 1] += 0;
    				equal[log_val + 1] += 0;
    				equal[log_val - 1] += 0;
    			}
    		}
    		for(it1 = less_than.begin();it1 != less_than.end();++it1)
    		{
    			sum_less_true_total += it1->second;
    		}
    		total_true = sum_less_true_total;
    		for(it1 = less_than.begin(),it2 = greater_than.begin(),it3 = equal.begin();it1 != less_than.end();++it1,++it2,++it3)
    		{
    			sum_greater_true_total += it2->second;
    			if((sum_less_true_total+ sum_greater_true_total + it3->second) > total_true)
    			{
    				total_true = sum_less_true_total + sum_greater_true_total + it3->second;
    			}
    			sum_less_true_total -= it1->second;
    		}
    		cout << hints - total_true << endl;
    		less_than.clear();greater_than.clear();equal.clear();	
    	}
    	return 0;
    }
     

