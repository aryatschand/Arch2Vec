
    #include <iostream>
    #include <string>
    #include <cstdio>
    #include <cstdlib>
    #include <map>
     
    using namespace std;
     
    int main() {
        int T;
        scanf("%d",&T);
        while(T--) {
            map <long long, long long> sweep_markers;
            int N;
            scanf("%d",&N);
            string preds[100001];
            const long long maxx = 1000000000ll;
            for (int i = 0; i < N; ++i) {
                char op;
                long long num;
                string answer;
                cin >> op;
                cin >> num;
                cin >> answer;
                bool yes = answer[0] == 'Y';
                switch(op) {
                    case '<':
                        if (yes) {
                            sweep_markers[num] ++;
                        } else {
                            // >=
                            sweep_markers[0]++;
                            sweep_markers[num]--;
                        }
                        break;
                    case '>':
                        if (yes) {
                            sweep_markers[0] ++;
                            sweep_markers[num+1]--;
                        } else {
                            // <=
                            sweep_markers[num + 1] ++;
                        }
                        break;
                    case '=':
                        if (yes) {
                            sweep_markers[0]++;
                            sweep_markers[num]--;
                            sweep_markers[num + 1]++;
                        } else {
                            sweep_markers[num]++;
                            sweep_markers[num+1]--;
                        }
                }
            }
            map<long long, long long>::const_iterator it;
            long long mcount = 10000000;
            long long total = 0;
            for (it = sweep_markers.begin(); it != sweep_markers.end(); it++) {
                total += it->second;
                if (it-> first <= maxx && mcount > total) {
                    mcount = total;
                }
            }
            printf("%lld\n", mcount);
        }
    }
     

