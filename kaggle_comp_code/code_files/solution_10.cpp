
    // Program to print all combination of size r in an array of size n
    #include<iostream>
    using namespace std;
    int b=0;
    void combinationUtil(int arr[], int data[], int start, int end,int index, int r,int m);
     
    // The main function that prints all combinations of size r
    // in arr[] of size n. This function mainly uses combinationUtil()
    void printCombination(int arr[], int n, int r,int m)
    {
        // A temporary array to store all combination one by one
        int data[r];
     
        // Print all combination using temprary array 'data[]'
        combinationUtil(arr, data, 0, n-1, 0, r,m);
    }
     
    /* arr[]  ---> Input Array
       data[] ---> Temporary array to store current combination
       start & end ---> Staring and Ending indexes in arr[]
       index  ---> Current index in data[]
       r ---> Size of a combination to be printed */
    void combinationUtil(int arr[], int data[], int start, int end,int index, int r,int m)
    {   int h=0;
        // Current combination is ready to be printed, print it
        if (index == r)
        {
            for (int j=0; j<r; j++)
                h=h+data[j];
            if(h==m)
    		{ 
    		   b=1;
    		} 
    		return;
        }
     
        // replace index with all possible elements. The condition
        // "end-i+1 >= r-index" makes sure that including one element
        // at index will make a combination with remaining elements
        // at remaining positions
        for (int i=start; i<=end && end-i+1 >= r-index; i++)
        {
            data[index] = arr[i];
            combinationUtil(arr, data, i+1, end, index+1, r,m);
        }
    }
     
    // Driver program to test above functions
    int main()
    {
        int t;
        cin>>t;
        while(t--)
        {
        	 b=0;
        	int n;
        	int m;
        	cin>>n;
        	cin>>m;
          int a[n];
          int  i;
          for(i=0;i<n;i++)
          {
          	 cin>>a[i];
    	  }
    	 for(i=1;i<=n;i++)
       {
      
        printCombination(a, n, i,m);
       }
        if(b==1)
    	 cout<<"Yes\n";
    	 else
    	 cout<<"No\n"; 
    }
    return 0;
    }

