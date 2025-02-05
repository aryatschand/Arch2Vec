
        #include <iostream>
        #include <algorithm>
        using namespace std;
        int main()
        {
            int t;
            cin>>t;
            while(t--)
            {
                int n,m,i,j;
                cin>>n>>m;
                bool a[m+1][n+1];
                int notes[n];
                for(i=0;i<n;i++)
                    cin>>notes[i];
                sort(notes , notes + n);
                for(i=0;i<n+1;i++)
                    a[0][i]=true;
                for(i=1;i<=m;i++)
                    a[i][0]=false;
                for(i=1;i<=m;i++)
                {
                    for(j=1;j<=n;j++)
                    {
                        a[i][j]=a[i][j-1];
                        if(i>=notes[j-1])
                            {
                                a[i][j]=(a[i][j] + a[i-notes[j-1]][j-1]);
                            }
                    }
                }
                if(a[m][n]==true)
                    cout<<"Yes\n";
                else
                    cout<<"No\n";
            }
            return 0;
        } 

