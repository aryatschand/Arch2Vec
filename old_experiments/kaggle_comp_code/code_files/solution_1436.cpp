
    #include <stdio.h>
    #include <map>
    
    using namespace std;
    
    struct HintBlock_t{
        int Less, LessOrEqual, Bigger, BiggerOrEqual, Equal, NotEqual;
    
        HintBlock_t();
    };
    
    HintBlock_t::HintBlock_t(){
        Less=0; LessOrEqual=0;
        Bigger=0; BiggerOrEqual=0;
        Equal=0;
        NotEqual=0;
    }
    
    typedef map<int, HintBlock_t> HintBlock_cont;
    typedef map<int, HintBlock_t>::iterator HintBlock_it;
    
    int main() {
        int T, K;
        char Op;
        int limit;
        char rep[4];
    
        HintBlock_t *Hint_aux;
        HintBlock_cont Hints;
    
        int BigLies, SmallLies, EqualLies;
        int minLies, curLies;
    
    	scanf("%d ", &T);
    
    	for(int i=0; i<T; i++)
    	{
    	    scanf("%d ", &K);
    	    BigLies=0; SmallLies=0; EqualLies=0;
    
    	    for(int j=0; j<K; j++)
    	    {
    	        scanf("%c %d %s ", &Op, &limit, rep);
    
    	        Hint_aux = &Hints[limit];
    
    	        switch(Op)
    	        {
    	        case '<':
    	            if(rep[0]=='Y')
    	                Hint_aux->Less ++;
    	            else
    	                Hint_aux->BiggerOrEqual ++;
    	            break;
    	        case '>':
    	            if(rep[0]=='Y')
    	                Hint_aux->Bigger ++;
    	            else
    	                Hint_aux->LessOrEqual ++;
    	            break;
    	        case '=':
    	            if(rep[0]=='Y')
    	                Hint_aux->Equal ++;
    	            else
    	                Hint_aux->NotEqual ++;
    	            break;
    
    	        }
    	    }
    
    
    
    	    /*printf("Numb   < <=  > >=  = !=\n");
    	    for(HintBlock_it it = Hints.begin(); it!=Hints.end(); it++)
    	    {
    	        printf("%4d: %2d %2d %2d %2d %2d %2d\n", it->first, (it->second).Less,
    	              (it->second).LessOrEqual, (it->second).Bigger,
    	              (it->second).BiggerOrEqual, (it->second).Equal,
    	              (it->second).NotEqual);
    	    }*/
    
    	    for(HintBlock_it it = Hints.begin(); it!=Hints.end(); it++)
    	    {
    	        BigLies += (it->second).Bigger;
    	        BigLies += (it->second).BiggerOrEqual;
    	        EqualLies += (it->second).Equal;
    	    }
    
    	    HintBlock_it it = Hints.begin();
    	    HintBlock_it it2 = it;
    	    it2++;
    
    	    //handle before 1st element
    	    if(it->first > 1)
    	        minLies = BigLies + EqualLies;
    	    else
    	        minLies = K; //the maximum number os lies possible
    
    	    for(; it!=Hints.end(); it++, it2++)
    	    {
    	        //Handle at element
    	        BigLies -= (it->second).BiggerOrEqual;
    	        SmallLies += (it->second).Less;
    
    	        curLies = BigLies + SmallLies + EqualLies + (it->second).NotEqual
    	                - (it->second).Equal;
    	        if(curLies < minLies)
    	            minLies = curLies;
    
    
    	        //Handle after element
    	        BigLies -= (it->second).Bigger;
    	        SmallLies += (it->second).LessOrEqual;
    	        //it2++;
    	        if((it2==Hints.end() && it->first < 1e9) || it2->first>it->first+1)
    	        {
    	            curLies = BigLies + SmallLies + EqualLies;
    	            if(curLies < minLies)
    	                minLies = curLies;
    	        }
    
    	    }
    
    	    printf("%d\n", minLies);
    
    	    Hints.clear();
    	}
    
    	return 0;
    }
    

