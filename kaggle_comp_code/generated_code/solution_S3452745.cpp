
    #include <iostream>
    #include <map>
    #include <string>
    #include <algorithm>
    
    using namespace std;
    
    void solve()
    {
      int k;
      cin >> k;
      map<int, int> seg;
      for (int i = 0; i != k; ++i) {
        char c;
        int x;
        bool yes;
        cin >> c;
        cin >> x;
        {
          string s;
          cin >> s;
          yes = (s == "Yes");
        }
        switch (c) {
        case '<':
          if (yes)
            ++seg[x];
          else
            ++seg[1], --seg[x];
          break;
        case '>':
          if (yes)
            ++seg[1], --seg[x+1];
          else
            ++seg[x+1];
          break;
        case '=':
          if (yes)
            ++seg[1], --seg[x], ++seg[x+1];
          else
            ++seg[x], --seg[x+1];
        }
    //    for (auto p: seg)
    //      cout << p.first << ": " << p.second << endl;
      }
     
      int count = seg[1];
      seg.erase(1);
      int mincount = count;
    //  cout << "count = " << count << endl;
      for (auto p: seg) {
        if (p.first > 1000000000)
          break;
        count += p.second;
    //    cout << "count = " << count << endl;
        mincount = min(count, mincount);
      }
      cout << mincount << endl;
    }
    
    
    int main()
    {
      int t;
      cin >> t;
      for (; t; --t)
        solve();
    }

