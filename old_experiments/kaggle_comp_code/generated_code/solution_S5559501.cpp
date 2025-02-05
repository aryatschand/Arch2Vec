
    #include<string>
    #include<cmath>
    #include<map>
    #include<algorithm>
    #include<vector>
    #include<iostream>
    using namespace std;
    
    typedef unsigned int ui;
    const ui OO = 1e9;
    struct range{
    	ui rs, re;
    	bool nE;
    	range(ui s, ui e, bool ne = 0):rs(s), re(e), nE(ne){};
    	bool operator < (const range& r) const{ return ((rs == r.rs) ? (re < r.re) : (rs < r.rs)); }
    };
    
    int main(){
    	vector<range> ranges;
    	map<ui, ui> lEs;
    	
    	int t;
    	cin >> t;
    	while(t--){
    		ranges.clear();
    		lEs.clear();
    		
    		ui k;
    		cin >> k;
    		for(ui i = 0; i<k; i++){
    			char op;
    			ui li;
    			string logic;
    			cin >> op >> li >> logic;
    			
    			switch(op){
    				case '<':
    					if(logic == "Yes"){
    						if(li != 1) ranges.push_back(range(1, li-1));
    					}else 
    						ranges.push_back(range(li, OO));
    					break;
    				case '>':
    					if(logic == "Yes"){
    						if(li != OO) ranges.push_back(range(li+1, OO));
    					}else 
    						ranges.push_back(range(1, li));
    					break;
    				case '=':
    					if(logic == "Yes") ranges.push_back(range(li, li));
    					else{
    						ranges.push_back(range(1, li-1, 1));
    						ranges.push_back(range(li+1, OO));
    					}
    					break;
    			}
    		}
    		
    		sort(ranges.begin(), ranges.end());
    		//cout << endl << ranges.size() << endl;
    		ui mTrue = 0;
    		ui cCnt = 1;
    		vector<ui> nEs;
    		
    		if(ranges.size() > 0){
    			lEs[ranges[0].re] = 1;
    			if(ranges[0].nE) nEs.push_back(0);
    		}
    		
    		for(ui i=1; i<ranges.size(); i++){
    			//cout << ranges[i].rs << "\t" << ranges[i].re << endl;
    			/*
    			for(vector<ui>::iterator it = nEs.begin(); it != nEs.end(); it++){
    				//cout << ranges[*it].nE;
    				if(ranges[i].rs < (ranges[*it].re + 1) && ranges[i].re > (ranges[*it].re + 1))
    				{
    					k++;
    					nEs.erase(it);
    					it--;					
    				}else if(ranges[i].rs > ranges[*it].re){
    					nEs.erase(it);
    					it--;
    				}
    			}
    			*/
    			if(ranges[i].nE) nEs.push_back(i);
    			if(ranges[i].rs <= ranges[i-1].re && ranges[i].rs <= lEs.begin()->first){
    				cCnt++;
    				if(lEs.find(ranges[i].re) == lEs.end()) lEs[ranges[i].re] = 1;
    				else lEs[ranges[i].re]++;
    			}else{
    				mTrue = max(mTrue, cCnt);
    				
    				
    				
    				while(lEs.size() > 0 && ranges[i].rs > lEs.begin()->first){
    					cCnt -= lEs.begin()->second;
    					lEs.erase(lEs.begin());
    					//cout << i <<"d";
    				}
    				
    				if(lEs.find(ranges[i].re) == lEs.end()) lEs[ranges[i].re] = 1;
    				cCnt++;
    				//cCnt = i - lEs.begin()->second + 1;
    			}
    			//cout << "ddd" << cCnt;
    		}
    		if(ranges.size() > 0){mTrue = max(mTrue, cCnt);}
    		cout << k-mTrue;
    		if(t) cout << endl;
    	}
    	return 0;
    }

