
    #include<iostream>
    using namespace std;
    // void mergesortwithrec(int a,int b){
    //     int n=b-a;
    //     if(a+1==b){
    //         brr[a] = arr[a];
    //         return;
    //     }
       
    //     mergesort(a,a+(n/2));
    //     mergesort(a+(n/2),a+n);
    //     int k=a,l=a+n/2;
    //     for(int i=a;i<b;i++){
    //         if((k<a+(n/2)&&l<n+a&&(arr[k][0]<arr[l][0]||(arr[k][0]==arr[l][0]&&arr[k][1]<arr[l][1])))||l>=n+a){
    //             brr[i]=arr[k];
    //             k++;
    //         }
    //         else{
    //             brr[i]=arr[l];
    //             l++;
    //         }
    //     }
    //     for(int i=a;i<b;i++)
    //         arr[i]=brr[i];
    // }
    void mergesort(int** arr,int n){
        int** brr =  new int*[n];
            for(int i=2;i/2<n;i*=2){
            for(int j=0;j<n;j+=i){
                if(n-j<=i/2)break;
                else{
                    int mid = j+i/2;
                    int start = j;
                    int end = (j+i);
                    if(end>n)end=n;
                    int a1=start,a2=mid;
                    for(int p=start;p<end;p++){
                        if(a2==end||(a1<mid&&(arr[a1][0]<arr[a2][0]||(arr[a1][0]==arr[a2][0]&&arr[a1][1]<arr[a2][1]))))
                            brr[p]=arr[a1++];
                        else
                            brr[p]=arr[a2++];
                        
                    }
                    for(int p=start;p<end;p++)arr[p]=brr[p];
                }
            }
            
        }
        delete [] brr;
    }
    int main(){
        int t;
        cin>>t;
        for(int kl=0;kl<t;kl++){
            int k;
            cin>>k;
            int K=k;
            int** arr;
            arr = new int*[k];
          for(int i=0;i<k;i++)arr[i] = new int[2];
            for(int i=0;i<k;i++){
                int a;
                char b,c[4];
                cin>>b;
                cin>>a;
                cin>>c;
                if(b=='<'){
                    if(c[0]=='Y'){
                    	if(a==1){
                        
                    		k--;
                    		i--;
                    		continue;
                    	}
                        arr[i][0]=a-1;
                        arr[i][1]=4;
                    }
                    else{
                        arr[i][0]=a;
                        arr[i][1]=2;
                    }
                }
                else if(b=='>'){
                    if(c[0]=='Y'){
                    	if(a==1000000000){
                    
                                
                    		k--;
                    		i--;
                    		continue;
                    	}
                        arr[i][0]=a+1;
                        arr[i][1]=2;
                    }
                    else{
                        arr[i][0]=a;
                        arr[i][1]=4;
                    }
                }
                else{
                    if(c[0]=='Y'){
                        arr[i][0]=a;
                        arr[i][1]=3;
                    }
                    else{
                    	if(a==1){
                             
                                
                    		arr[i][0]=2;
                    		arr[i][1]=2;
                    	}
                    	else if(a==1000000000){
                             
                                
                    	 arr[i][0]=999999999;
                        arr[i][1]=4;
                    	}
                    	else{
                        arr[i][0]=a;
                        arr[i][1]=1;
                    	}
                    }
                }
                
            }
    
            mergesort(arr,k);
         
            int count=0,max=0,equal,now=0,extra;
            for(int i=0;i<k;i++){
                //   if(kl==16){
                //  cout<<arr[i][0]<<" ";
                // switch(arr[i][1]){
                //     case 1: cout<<"!=";break;
                //     case 2: cout<<">=";break;
                //     case 3: cout<<"=";break;
                //     case 4: cout<<"<=";break;
                // }
                // }
            	if(arr[i][1]==4)
            		max++;
            	else if(arr[i][1]==2){
            		count++;
                    if(now==arr[i][0]){
            		  if(count-extra>max)max=count-extra;
                    }
                    else
                        if(count>max)max=count;
            	}
            	else if(arr[i][1]==3){
            		if(i>0&&arr[i-1][0]==arr[i][0]&&arr[i-1][1]==3)
            			equal++;
            		else equal=1;
                    if(now==arr[i][0]){
                		if((count+equal-extra)>max)
                			max=count+equal-extra;
                    }
                    else
                        if((count+equal)>max)
                            max=count+equal;
            	}
            	else{
            		max++;
                    count++;
                    if(i>0&&arr[i-1][0]==arr[i][0]&&arr[i-1][1]==1)
                        extra++;
                    else extra=1;
            		now=arr[i][0];
            	}
                // if(kl==16)
                // cout<<" max = "<<max<<" count = "<<count<<endl;
            }
            if(count>max)max=count;
            cout<<(K-max)<<endl;
            for(int i=0;i<k;i++)
            	delete [] arr[i];
            delete [] arr;
        }
    }
    

