
    #include <iostream>
    #include <vector>
    #include <string>
    #include <algorithm>
    
    const int maxbound = 1000000001;
    
    struct Edge
    {
    	Edge() = default;
    	Edge(int bound, bool isEnd) : bound(bound), isEnd(isEnd) {}
    	int bound = 1;
    	bool isEnd = false;
    
    	bool operator<(const Edge& that) const {
    		return bound < that.bound || (bound == that.bound && isEnd && !that.isEnd);
    	}
    };
    
    int main()
    {
    	int t = 0;
    	std::cin >> t;
    
    	for (int i = 0; i < t; ++i) {
    		int k = 0;
    		std::cin >> k;
    
    		std::vector<Edge> bounds(4 * k);
    		int index = 0;
    
    		for (int j = 0; j < k; ++j) {
    			char op = 0;
    			int bound = 0;
    			std::string s;
    
    			std::cin >> op >> bound >> s;
    
    			Edge begin;
    			Edge end(maxbound, true);
    
    			if (op == '<') {
    				if (s == "Yes") {
    					end.bound = bound;
    				} else {
    					begin.bound = bound;
    				}
    			} else if (op == '>') {
    				if (s == "Yes") {
    					begin.bound = bound + 1;
    				} else {
    					end.bound = bound + 1;
    				}
    			} else {
    				if (s == "Yes") {
    					begin.bound = bound;
    					end.bound = bound + 1;
    				} else {
    					end.bound = bound;
    					Edge begin2(bound + 1, false);
    					Edge end2(maxbound, true);
    					bounds[index++] = begin2;
    					bounds[index++] = end2;
    				}
    			}
    
    			bounds[index++] = begin;
    			bounds[index++] = end;
    		}
    
    		std::sort(bounds.begin(), bounds.begin() + index);
    
    		int intersects = 0;
    		int max = 0;
    
    		for (int j = 0; j < index; ++j) {
    			const Edge& edge = bounds[j];
    			if (edge.isEnd) --intersects;
    			else ++intersects;
    
    			if (intersects > max) max = intersects;
    		}
    
    		std::cout << (k - max) << std::endl;
    	}
    	
    	return 0;
    }
    

