
        #include <iostream>
        #include <cstdio>
        #include <cstdlib>
        #include <vector>
        #include <algorithm>
        #include <cstring>
        #define MAX_NUM 1000000000
        #define MAX_LIES_POSSIBLE 1000000
        struct range {
        int rangeStart;
        int lies;
        range(){};
        range(int _rangeStart, int _lies) :rangeStart(_rangeStart), lies(_lies){};
        };
        bool sortByRangeStart(const range &a, const range &b);
        int computeMinLies();
        void addGuessAnswer( char symbol, int number, bool answer);
        std::vector<range>* lyingRanges;
        int fullLyingRangesCount;
        int main(int argc, char* argv[]){
        int cases;
        scanf("%d ",&cases);
        for(int i=0; i<cases; i++) {
        int hints;
        scanf("%d ",&hints);
        fullLyingRangesCount=0;
        lyingRanges= new std::vector<range>();
        for(int j=0; j< hints; j++){
        char symbol;
        int number;
        char answer[5];
        scanf("%c %d %s ",&symbol, &number, answer);
        if(strcmp(answer,"Yes")==0)
        addGuessAnswer( symbol, number, true);
        else
        addGuessAnswer( symbol, number, false);
        }
        printf("%d\n",computeMinLies());
        delete lyingRanges;
        }
        return 0;
        }
        bool sortByRangeStart(const range &a, const range &b){
        return a.rangeStart < b.rangeStart;
        }
        int computeMinLies(){
        std::sort(lyingRanges->begin(),lyingRanges->end(),sortByRangeStart);
        int liesCount=fullLyingRangesCount;
        std::vector<int> uniqueLyingRanges;
        std::vector<int> rangeLiesCount;
        uniqueLyingRanges.push_back(1);
        rangeLiesCount.push_back(liesCount);
        for(std::vector<range>::iterator curGuess=lyingRanges->begin(); curGuess!=lyingRanges->end(); ++curGuess){
        liesCount+=curGuess->lies;
        if(curGuess->rangeStart!= uniqueLyingRanges.back()){
        uniqueLyingRanges.push_back(curGuess->rangeStart);
        rangeLiesCount.push_back(liesCount);
        } else
        rangeLiesCount.back()=liesCount;
        }
        if(uniqueLyingRanges.back()==MAX_NUM+1){
        uniqueLyingRanges.pop_back();
        rangeLiesCount.pop_back();
        }
        int minLies=rangeLiesCount.front();
        //std::vector<int>::iterator rangeStart=uniqueLyingRanges.begin();
        for(std::vector<int>::iterator curLiesIt=rangeLiesCount.begin(); curLiesIt!=rangeLiesCount.end(); curLiesIt++){
        if(*curLiesIt<minLies)
        minLies=*curLiesIt;
        //rangeStart++;
        }
        return minLies;
        }
        void addGuessAnswer( char symbol, int number, bool answer){
        switch(symbol) {
        case '>':
        if(answer){
        fullLyingRangesCount++;
        lyingRanges->push_back( range(number+1,-1) );
        }else
        lyingRanges->push_back( range(number+1,1) );
        break;
        case '<':
        if(answer)
        lyingRanges->push_back( range(number,1) );
        else {
        fullLyingRangesCount++;
        lyingRanges->push_back( range(number,-1) );
        }
        break;
        case '=':
        if(answer) {
        lyingRanges->push_back( range(number+1,1) );
        fullLyingRangesCount++;
        lyingRanges->push_back( range(number,-1) );
        } else {
        lyingRanges->push_back( range(number,1) );
        lyingRanges->push_back( range(number+1,-1) );
        }
        }
        } 

