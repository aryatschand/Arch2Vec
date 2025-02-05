
    #include <bits/stdc++.h>
     
    using namespace std;
     
    int check(int a[],int sum ,int csum , int cind,int siz)
    { if(csum==sum)
        return 1;
        if(cind==siz)
      return 0;
      return (check(a,sum,csum+a[cind],cind+1,siz) || check(a,sum,csum,cind+1,siz));
     
    }
     
    int main()
    {
        int n,i,a,b,j;
        cin>>n;
        for(i=1;i<=n;i++)
        {
            cin>>a>>b;
            int A[a];
            for(j=0;j<a;j++)
            cin>>A[j];
            if(check(A,b,0,0,a))
                cout<<"Yes"<<endl;
            else
                cout<<"No"<<endl;
        }
    }

