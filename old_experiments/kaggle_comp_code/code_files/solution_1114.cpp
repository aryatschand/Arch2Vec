
    #include<iostream>
    #include<string>
    #include<map>
    
    #define DEBUG 0
    
    using namespace std;
    
    typedef unsigned int uint;
    
    const string yesAnswer = "Yes";
    const string noAnswer = "No";
    const uint maximumValue = 1000000000;
    const uint maximumHintsCount = 100000;
    
    struct Hint
    {
    public:
        uint equalsCounter;
        uint diffsCounter;
        uint lessThenCounter;
        uint greaterThenCounter;
    
        Hint();
    };
    
    Hint::Hint()
    {
        equalsCounter = 0;
        diffsCounter = 0;
        lessThenCounter = 0;
        greaterThenCounter = 0;  
    }
    
    map<uint, Hint> hints;
    
    ostream& operator<<(ostream& os, const Hint& hint)
    {
        os << "[";
        os << "eq: " << hint.equalsCounter;
        os << ", df: " << hint.diffsCounter;
        os << ", lt: " << hint.lessThenCounter;
        os << ", gt: " << hint.greaterThenCounter;
        os << "]";
    
        return os;
    }
    
    ostream& operator<<(ostream& os, const map<uint, Hint>& hints)
    {
       for (map<uint, Hint>::const_iterator it = hints.begin(); it != hints.end(); ++it)
       {
           os << it->first << " => " << it->second << endl;
       }
    
       return os;
    }
    
    void readHint()
    {
        char sign;
        uint value;
        string answer;
    
        cin >> sign >> value >> answer;
    
        if (sign == '=' || (sign == '<' && answer == yesAnswer) ||
            (sign == '>' && answer == yesAnswer) )
        {
            Hint& hint = hints[value];
    
            if (sign == '=')
            {
                if (answer == yesAnswer)
                {
                    hint.equalsCounter++;
                } else {
                    hint.diffsCounter++;
                }
            }
    
            if (sign == '<')
            {
                hint.lessThenCounter++;
            }
    
            if (sign == '>')
            {
                hint.greaterThenCounter++;
            }
    
        }
    
        else if (sign == '<' && answer == noAnswer && value > 1)
        {
            Hint& hint = hints[value - 1];
            hint.greaterThenCounter++;
        }
    
        else if (sign == '>' && answer == noAnswer && value < maximumValue)
        {
            Hint& hint = hints[value + 1];
            hint.lessThenCounter++;
        }
    }
    
    void countHintsLessThen()
    {
        uint lessThenCounter = 0;
    
        for (map<uint, Hint>::iterator it = hints.begin(); it != hints.end(); ++it)
        {
            Hint& hint = it->second;
            hint.lessThenCounter += lessThenCounter;
            lessThenCounter = hint.lessThenCounter + hint.equalsCounter;
        }
    }
    
    void countHintsGreaterThen()
    {
        uint greaterThenCounter = 0;
    
        for (map<uint, Hint>::reverse_iterator it = hints.rbegin(); it != hints.rend(); ++it)
        {
            Hint& hint = it->second;
            hint.greaterThenCounter += greaterThenCounter;
            greaterThenCounter = hint.greaterThenCounter + hint.equalsCounter;
        }
    }
    
    uint findMinCount()
    {
        uint value = 0;
        uint previousValue = 0;
        uint lessThenOrEqualToPrevious = 0;
        uint currentCount = 0;
        uint minCount = maximumHintsCount + 1;
    
        for (map<uint, Hint>::iterator it = hints.begin(); it != hints.end(); ++it)
        {
            value = it->first;
            Hint& hint = it->second;
    
            if (value > 1 && (value - 1 > previousValue))
            {
                currentCount = lessThenOrEqualToPrevious + hint.equalsCounter + hint.greaterThenCounter;
                minCount = min(currentCount, minCount);
            }
    
            currentCount = hint.diffsCounter + hint.greaterThenCounter + hint.lessThenCounter;
            minCount = min(currentCount, minCount);
    
            previousValue = value;
            lessThenOrEqualToPrevious = hint.lessThenCounter + hint.equalsCounter;
        }
    
        if (value < maximumValue)
        {
            minCount = min(lessThenOrEqualToPrevious, minCount);
        }
    
        if (minCount == maximumHintsCount + 1)
        {
            return 0;
        }
        return minCount;
    }
    
    void guessTestCase()
    {
        uint numberOfHints;
        cin >> numberOfHints;
    
        hints.clear();
    
        for (uint i = 0; i < numberOfHints; ++i)
        {
            readHint();
        }
    
        if (DEBUG)
        {
            cout << "-------------------------------------" << endl;
            cout << hints;
            cout << "SIZE: " << hints.size() << endl;
        }
    
        countHintsLessThen();
        countHintsGreaterThen();
    
        if (DEBUG)
        {
            cout << "-------------------------------------" << endl;
            cout << hints;
            cout << "MIN COUNT: ";
        }
        cout << findMinCount() << endl;
    }
    
    void guessExperiment()
    {
        uint numberOfTestCases;
        cin >> numberOfTestCases;
    
        for (uint i = 0; i < numberOfTestCases; ++i)
        {
            guessTestCase();
        }
    }
    
    int main()
    {
        guessExperiment();
        return 0;
    }
    
    
    

