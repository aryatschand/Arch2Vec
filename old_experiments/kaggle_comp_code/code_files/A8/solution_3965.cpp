
    #include <iostream>
    #include <set>
    #include <vector>
     
    typedef unsigned int uint;
     
    #define MAXNODES 600
    #define nullentry 0xffffffff
     
    class Graph {
      friend class RootedTree;
      struct Node {
        uint numedges;
        uint edges[MAXNODES];
      };
      Node nodes[MAXNODES];
      uint numnodes;
      void clear() {
        for (uint x = 0 ; x < MAXNODES ; x++) nodes[x].numedges = 0;
        numnodes = 0;
      }
    public:
      Graph() { clear(); }
      void addEdge(uint x, uint y) {
        if (x >= numnodes) numnodes = x + 1;
        if (y >= numnodes) numnodes = y + 1;
        nodes[x].edges[(nodes[x].numedges)++] = y;
        nodes[y].edges[(nodes[y].numedges)++] = x;
      }
      uint size() const { return numnodes; }
    };
     
    class Stack {
      std::vector<uint> stack;
      uint pos;
    public:
      Stack() : pos(0) { }
      void push(uint x) {
        if (pos == stack.size()) {
          stack.push_back(x);
          pos++;
        } else {
          stack[pos++] = x;
        }
      }
      uint pop() { return stack[--pos]; }
      uint size() { return pos; }
    };
     
    class RootedTree {
      struct Node {
        uint parent;
        uint firstchild;
        uint nextsibling;
        bool marked;
        std::set<uint> criticalset;
      };
      Node nodes[MAXNODES];
      uint size;
      bool lexCompare(uint array1[], uint l1,
    		  uint array2[], uint l2) {
        for (uint x = 0 ; x < l1 && x < l2 ; x++)
          if (array1[x] != array2[x]) return (array1[x] < array2[x]);
        return (l1 < l2);
      }
      void doMe(uint r, uint children[], uint numchildren,
    	    uint branchlabels[], uint unionlhats[],
    	    uint scratch[], uint scratch2[]) {
        uint unlabeled_branch = numchildren;
        std::set<uint> K;
        std::set<uint> lhat[numchildren];
        for (uint i = 0 ; i < numchildren ; i++) {
          branchlabels[i] = 0;
          lhat[i] = nodes[children[i]].criticalset;
        }
        uint alpha, largestinlhat;
        uint gamma, w;
        uint counter;
        while (unlabeled_branch) {
          if ((++counter) == 20) break;
          for (uint i = 0 ; i < size ; i++) unionlhats[i] = 0;
          largestinlhat = 0;
          for (uint i = 0 ; i < numchildren ; i++) {
    	for (std::set<uint>::const_iterator it = lhat[i].begin() ;
    	     it != lhat[i].end() ; ++it) {
    	  unionlhats[*it] += 1;
    	  if ((*it) >= largestinlhat) largestinlhat = (*it) + 1;
    	}
          }
          if (K.size() < 2) {
    	alpha = 0;
    	for (uint i = 0 ; i < largestinlhat ; i++)
    	  if (unionlhats[i] > 1) alpha = i;
    	if (alpha) {
    	  K.clear();
    	  for (uint i = 0 ; i < numchildren ; i++)
    	    if (lhat[i].find(alpha) != lhat[i].end()) K.insert(i);
    	} else {
    	  K.clear();
    	  for (uint i = 0 ; i < numchildren ; i++)
    	    if (branchlabels[i] == 0) K.insert(i);
    	}
          }
          if (largestinlhat <= (alpha+1)) {
    	w = K.size();
    	for (uint i = 0 ; i < numchildren ; i++)
    	  unionlhats[branchlabels[i]] += 1;
          } else {
    	for (gamma = alpha + 1 ; unionlhats[gamma] == 0 ; gamma++) { }
    	for (uint i = 0 ; i < numchildren ; i++)
    	  unionlhats[branchlabels[i]] += 1;
    	w = 0;
    	for (uint i = alpha + 1 ; i < gamma ; i++)
    	  if (unionlhats[i] == 0) w++;
          }
          if ((alpha > 0 && w < (K.size() - 1)) ||
    	  (alpha == 0 && w < K.size())) {
    	uint x = gamma + 1;
    	while (unionlhats[x]) x++;
    	uint j = nullentry; uint jval = 0;
    	for (uint i = 0 ; i < numchildren ; i++) {
    	  uint isbest = 0;
    	  for (std::set<uint>::const_iterator it = lhat[i].begin();
    	       it != lhat[i].end() ; ++it) {
    	    // this is stupid.  you can do log instead of linear
    	    // time.  but i'm lazy
    	    const uint y = *it;
    	    if (y < x) isbest = y;
    	    else       break;
    	  }
    	  if (isbest > jval) { j = i; jval = isbest; }
    	}
    	if (branchlabels[j] == 0) unlabeled_branch--;
    	branchlabels[j] = x;
    	K.erase(j);
    	while (lhat[j].size() && *(lhat[j].begin()) < x) {
    	  lhat[j].erase(lhat[j].begin());
    	}	
          } else if (alpha > 0) {
    	uint currentlhattruncsize = 0;
    	uint currenti0 = nullentry;
    	for (std::set<uint>::const_iterator it = K.begin() ;
    	     it != K.end() ; ++it) {
    	  const uint i = *it;
    	  uint compsize = 0;
    	  for (std::set<uint>::const_iterator it2 = lhat[i].begin() ; 
    	       it2 != lhat[i].end() ; ++it2) {
    	    const uint j = *it2;
    	    if (j <= alpha) compsize++;
    	    else break;
    	  }
    	  uint counter = 0;
    	  for (std::set<uint>::const_iterator it2 = lhat[i].begin() ;
    	       it2 != lhat[i].end() ; ++it2) {
    	    if (counter == compsize) break;
    	    scratch2[compsize-1-counter] = *it2;
    	    counter++;
    	  }
    	  if (currentlhattruncsize == 0 ||
    	      lexCompare(scratch2, compsize,
    			 scratch, currentlhattruncsize)) {
    	    currentlhattruncsize = compsize;
    	    for (uint x = 0 ; x < compsize ; x++)
    	      scratch[x] = scratch2[x];
    	    currenti0 = i;
    	  }
    	}
    	uint nextlabel = alpha;
    	for (std::set<uint>::const_iterator it = K.begin() ;
    	     it != K.end() ; ++it) {
    	  const uint i = *it;
    	  if (i != currenti0) {
    	    nextlabel++;
    	    while (unionlhats[nextlabel]) nextlabel++;
    	    if (branchlabels[i] == 0)
    	      unlabeled_branch--;
    	    branchlabels[i] = nextlabel;
    	    while (lhat[i].size() && *(lhat[i].begin()) < nextlabel) {
    	      lhat[i].erase(lhat[i].begin());
    	    }    
    	  }
    	}
    	K.clear();
          } else {
    	uint nextlabel = 0;
    	for (std::set<uint>::const_iterator it = K.begin() ;
    	     it != K.end() ; ++it) {
    	  nextlabel++;
    	  while (unionlhats[nextlabel]) nextlabel++;
    	  const uint i = *it;
    	  branchlabels[i] = nextlabel;
    	  unlabeled_branch--;
    	  while (lhat[i].size() && *(lhat[i].begin()) < nextlabel) {
    	    lhat[i].erase(lhat[i].begin());
    	  }
    	}
    	K.clear();
          }
        }
        for (uint i = 0 ; i < numchildren ; i++) {
          for (std::set<uint>::const_iterator it = lhat[i].begin() ;
    	   it != lhat[i].end() ; ++it)
    	nodes[r].criticalset.insert(*it);
          nodes[r].criticalset.insert(branchlabels[i]);
        }
      }
    public:
      RootedTree(const Graph& g, uint root) {
        nodes[0].parent = nullentry;
        nodes[0].firstchild = nullentry;
        nodes[0].nextsibling = nullentry;
        nodes[0].marked = false;
        size = 1;
        Stack stack;
        stack.push(0);
        std::vector<uint> nodeidxes(g.size());
        nodeidxes[0] = root;
        while (stack.size()) {
          const uint r = stack.pop();
          const uint s = nodeidxes[r];
          const uint numedges = g.nodes[s].numedges;
          const uint parent =
    	(nodes[r].parent == nullentry)
    	? nullentry
    	: nodeidxes[nodes[r].parent];
          uint currentsibling = nullentry;
          for (uint i = 0 ; i < numedges ; i++) {
    	const uint t = g.nodes[s].edges[i];
    	if (parent != t) {
    	  nodeidxes[size] = t;
    	  nodes[size].parent = r;
    	  nodes[size].firstchild = nullentry;
    	  nodes[size].nextsibling = nullentry;
    	  nodes[size].marked = false;
    	  if (currentsibling == nullentry) {
    	    nodes[r].firstchild = size;
    	  } else {
    	    nodes[currentsibling].nextsibling = size;
    	  }
    	  currentsibling = size++;
    	  stack.push(currentsibling);
    	}
          }
        }
      }
      void print() const {
        for (uint s = 0 ; s < size ; s++) {
          uint child = nodes[s].firstchild;
          while (child != nullentry) {
    	std::cout << s << " -> " << child << '\n';
    	child = nodes[child].nextsibling;
          }
        }
      }
      void recursivelyDo() {
        uint children[MAXNODES]; uint numchildren = 0;
        uint branchlabels[MAXNODES];
        uint scratch[MAXNODES], scratch2[MAXNODES], scratch3[MAXNODES];
        Stack stack;
        stack.push(0);
        while (stack.size()) {
          const uint r = stack.pop();
          if (nodes[r].marked) {
    	numchildren = 0;
    	for (uint child = nodes[r].firstchild ; child != nullentry ;
    	     child = nodes[child].nextsibling) {
    	  children[numchildren++] = child;
    	}
    	doMe(r, children, numchildren, branchlabels,
    	     scratch, scratch2, scratch3);
          } else {
    	stack.push(r);
    	for (uint child = nodes[r].firstchild ; child != nullentry ;
    	     child = nodes[child].nextsibling) {
    	  stack.push(child);
    	}
    	nodes[r].marked = true;
          }
        }
      }
      uint largestLabel() const {
        return *(nodes[0].criticalset.rbegin());
      }
    };
     
    void doTestCase() {
      uint k;
      std::cin >> k;
      Graph g;
      for (uint x = 1 ; x < k ; x++) {
        uint y; std::cin >> y;
        g.addEdge(x, y);
      }
      RootedTree rt(g, 0);
      rt.recursivelyDo();
      std::cout << rt.largestLabel() << '\n';
    }
     
    int main() {
      uint t;
      std::cin >> t;
      for (uint c = 0 ; c < t ; c++) doTestCase();
      return 0;
    }

