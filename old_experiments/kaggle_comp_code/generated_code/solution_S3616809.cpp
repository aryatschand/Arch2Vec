
    #include <stdio.h>
    #include <stdlib.h>
     
    #define MAX_K 100000
    #define MAX_L 1000000000
     
    struct constr{
    int l;
    int eq;
    int ne;
    int gr;
    int lo;
    };
     
    int k;//num constraints
    constr cst[100000];//constraints
     
    void read(){
    int i,j;
    scanf("%d",&k);
    for(j=0;j<k;j++){
    char c[2]; int x; char s[4];
    scanf("%1s %d %3s",c,&x,s);
    cst[j].eq=0;
    cst[j].ne=0;
    cst[j].gr=0;
    cst[j].lo=0;
    if(s[0]=='Y'){
    if(c[0]=='<'){
    cst[j].l=x;
    cst[j].lo++;
    }else if(c[0]=='>'){
    cst[j].l=x;
    cst[j].gr++;
    }else{
    cst[j].l=x;
    cst[j].eq++;
    }
    }else{
    if(c[0]=='<'){
    cst[j].l=--x;
    cst[j].gr++;
    }else if(c[0]=='>'){
    cst[j].l=++x;
    cst[j].lo++;
    }else{
    cst[j].l=x;
    cst[j].ne++;
    }
    }
    }
    }
     
    void filter(){
    int i,j=0;
    for(i=1;i<k;i++){
    if(cst[j].l==cst[i].l){
    cst[j].eq+=cst[i].eq;
    cst[j].ne+=cst[i].ne;
    cst[j].gr+=cst[i].gr;
    cst[j].lo+=cst[i].lo;
    }else{
    cst[++j]=cst[i];
    }
    }
    k=j+1;
    }
     
    int conflicts(){
    int i,cnt[k],neq=0,c=0,min=MAX_K;
    for(i=0;i<k;i++){
    c+=cst[i].lo;
    cnt[i]=c+cst[i].ne;
    c+=cst[i].eq;
    }
    c=0;
    for(i=k-1;i>=0;i--){
    c+=cst[i].gr;
    cnt[i]+=c;
    c+=cst[i].eq;
    if(cnt[i]<=min){
    min=cnt[i];
    int m1=0,m2=0;
    if(cst[i].l>1 && (i==0 || cst[i-1].l<=cst[i].l-2))
    m1=cst[i].ne+cst[i].lo-cst[i].eq;
    if(cst[i].l<MAX_L && (i==k-1 || cst[i+1].l>=cst[i].l+2))
    m2=cst[i].ne+cst[i].gr-cst[i].eq;
    if(m1>0 && m1>=m2) min-=m1;
    else if(m2>0) min-=m2;
    }
    }
    return min;
    }
     
    int compare(const void *a, const void *b){
    return ((constr*)a)->l-((constr*)b)->l;
    }
     
    int main(){
    int t,i;
    scanf("%d",&t);
    for(i=0;i<t;i++){
    read();
    qsort(cst,k,sizeof(constr),compare);
    filter();
    printf("%d\n",conflicts());
    }
    return 0;
    }
    

