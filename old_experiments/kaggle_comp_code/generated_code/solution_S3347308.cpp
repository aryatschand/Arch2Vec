
    #include<iostream>
    #include<cassert>
    #include<map>
    using namespace std;
    
    map<int, int> List;
     int minimum = 1;
     int maximum = 1000000000;
    int main() {
    	int t;
    	cin >> t;
    	assert(t <= 20 && t > 0);
    	while (t--) {
    		int k;
    		cin >> k;
    		assert (0<=k && k<=100000);
    		List.clear();
    		List[minimum] = 0;
    		for (int j = 0; j < k; j++) {
    			int l;
    			char logical_value[4];
    			char op[2];
    
    			cin >> op >> l >> logical_value;
    			assert(l >= minimum && l <= maximum);
    
    			if (*op == '<' && *logical_value == 'Y')
    				List[l]++;
    			if (*op == '<' && *logical_value == 'N') {
    				List[minimum]++;
    				List[l]--;
    			}
    			if (*op == '>' && *logical_value == 'Y') {
    				List[minimum]++;
    				List[l + 1]--;
    			}
    			if (*op == '>' && *logical_value == 'N') {
    				List[l + 1]++;
    			}
    			if (*op == '=' && *logical_value == 'Y') {
    				List[minimum]++;
    				List[l]--;
    				List[l + 1]++;
    			}
    			if (*op == '=' && *logical_value == 'N') {
    				List[l]++;
    				List[l + 1]--;
    			}
    		}
    		int sum = 0;
    		int mistakes = k;
    		for (map<int, int>::iterator iterator = List.begin(); iterator
    				!= List.end(); iterator++) {
    			sum += iterator->second;
    			if (iterator->first <= maximum)
    				mistakes = min(mistakes, sum);
    		}
    		cout  << mistakes << "\n";
    	}
    
    	return 0;
    }
    

