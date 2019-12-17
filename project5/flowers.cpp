#define _CRT_SECURE_NO_DEPRECATE
#include "utilities.h"
#include <iostream>
using namespace std;

const char WORDFILENAME[] = "/Users/roxanemartin/desktop/CS31/flowers/flowers/words.txt";
const int MAXWORDS = 9000;

int playOneRound(const char words[][MAXWORDLEN+1], int nWords, int wordnum);

int main()
{
    int maxWords = 7265;
    int totalRounds = 0; //keep track of number of rounds user wants to play
    char wordList[MAXWORDS][MAXWORDLEN+1]; //create wordList array
    int nWords = getWords(wordList, 9000, WORDFILENAME);//fill wordList
    if (nWords < 1)
    {
        cout << "No words were loaded, so I can't play the game." << endl;
    }
    else if (nWords >= 1 && nWords <= maxWords) //check if nWords is greater than 1 and less than maxWords
    {
        cout << "How many rounds do you want to play? "; //read in number of rounds
        cin >> totalRounds;
        cin.ignore(10000, '\n');
        
        //set up variables to hold stats
        int minimum = MAXWORDS;
        int maximum = 0;
        int sum = 0;
        int currentScore = 0;
        
        if (totalRounds<=0) //check if number of rounds is positive
            cout << "The number of rounds must be positive. ";
        else
        {
            for (int currentRound=1; currentRound<=totalRounds; currentRound++) //loop through number of rounds
            {
                cout << endl<< "Round " << currentRound << endl; //print round number
                int random = randInt(0, nWords-1); //generate random int
                cout << "The mystery word is "<< strlen(wordList[random]) << " letters long." << endl;
                currentScore = playOneRound(wordList, nWords, random); //call playOneRound
                if (currentScore == 1)
                    cout << "You got it in "<< currentScore <<" try."<<endl;
                else
                    cout << "You got it in "<< currentScore <<" tries."<<endl;
                //update stats
                if (currentScore<minimum)
                    minimum = currentScore;
                if (currentScore>maximum)
                    maximum = currentScore;
                sum += currentScore;
                
                cout.setf(ios::fixed); //set decimal precision to two decimal places
                cout.precision(2);
                
                cout <<"Average: "<< static_cast<double>(sum)/currentRound <<", minimum: "<< minimum << ", maximum: " << maximum<< endl<<endl; //print out calculated average, minimum, and maximum scores

            }
        }
    }
}

int playOneRound(const char words[][MAXWORDLEN+1], int nWords, int wordnum)
{
    // if nWords is not positive, or if wordnum is less than 0 or greater than or equal to nWords, return −1
    if (nWords<=0 || wordnum<0 || wordnum >= nWords)
        return -1;
    
    int score = 1;
    
    for (;;) { //start for loop of getting trial words and finding flowers and bees
        //get trial word from user
        char trial[MAXWORDLEN+1];
        cout << "Trial word: ";
        cin >> trial;
        
        //set up boolean that will hold whether trial word meets all conditions
        bool trialCheck = true;
        //check if trial word contains any uppercase characters
        int i=0;
        while ( trial[i] != '\0')
        {
            if (islower(trial[i])==0)
                trialCheck = false; //if it does, set trialCheck boolean to false
            i++;
        }
        //check that the trial word is between 4 and 6 characters
        if (strlen(trial)<4 || strlen(trial)>6)
            trialCheck = false; //if it is not, set trialCheck boolean to false


        //check that the trial word is in the words array
        bool contains = false;
        for (int i=0; i<nWords; i++)
        {
            if (strcmp(trial, words[i]) == 0)
            {
                contains = true;
            }
        }
        //if does not pass lower/length check, print error message
        if (!trialCheck)
            cout << "Your trial word must be a word of 4 to 6 lower case letters."<<endl;
        
        //else if is not contained, print error message
        else if (!contains)
            cout << "I don't know that word."<<endl;
        
        else if (contains && trialCheck) //else if trial conditions are passed, continue to counting flowers and bees
        {
            
            //copy random word to 'mystery'
            char mystery[MAXWORDLEN+1];
            strcpy(mystery, words[wordnum]);
            
            if (strcmp(trial, mystery)==0)
                break;
            //find flowers & bees
            int flowers = 0, bees = 0; //initialize flowers and bees to 0
            int k=0;
            while (trial[k] != '\0') //loop through characters in trial word
            {
                if (trial[k]==mystery[k]) // if character in trial word matches character in mystery word at same index
                {
                    flowers++; //increment number of flowers
                    mystery[k] = '*'; //set character at this index in mystery word to '*' so it cannot be double counted
                }
                else // if character in trial word does not match character in mystery word at same index
                {
                    int j = 0;
                    while (mystery[j] != '\0') //loop through characters in mystery word
                    {
                        if (trial[k]==mystery[j]) // if character in trial word matches any character in mystery word
                        {
                            bees++; //increment bees
                            mystery[j] = '*'; //set character at this index in mystery word to '*' so it cannot be double counted
                            break; //break out of while loop of characters in mystery word
                        }
                        j++;
                    }
                }
                k++;
            }
            cout << "Flowers: "<< flowers<< ", Bees: " << bees << endl;
            score++; //increment score
        }
    }

    return score; //return score
}

