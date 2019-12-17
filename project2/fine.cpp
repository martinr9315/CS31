#include <iostream>
#include <string>
using namespace std;

int main() {
    
    // get defendant's name from user
    cout << "Defendant: ";
    string defendantsName;
    getline(cin, defendantsName);
    
    // get amount paid (in thousands) from user
    cout << "Amount paid (in thousands): ";
    double amtPaid;
    cin >> amtPaid;
    // ignore all characters until next line
    cin.ignore(10000,'\n');
    
    // get a 'y' or 'n' from user about fake athlete status and put answer in string isFakeAthlete
    cout << "Fake athlete? (y/n): ";
    string isFakeAthlete;
    getline(cin, isFakeAthlete);
    
    cout << "---" << endl;
    
    //set base fine to 20
    double fine = 20;
    
    // check if defendant name is an empty string
    if (defendantsName == "")
        cout << "You must enter a defendant name." << endl;
    // check if amount paid is negative
    else if (amtPaid < 0)
        cout << "The amount paid must not be negative." << endl;
    //check if is fakeAthlete is something other than 'y' or 'n'
    else if (isFakeAthlete != "y" && isFakeAthlete != "n")
        cout << "You must enter y or n." << endl;
    else
    {
        //check if amtPaid is less than 40
        if (amtPaid <= 40)
            fine += .66 * amtPaid; //add 66% of total amtPaid
        else
        {
            fine = fine + .66*40; //add 66% of the first 40 to fine
            
            //check if amount paid is less than 250 (but more than 40)
            if (amtPaid <= 250)
            {
                //if is NOT fake athlete
                if (isFakeAthlete == "n")
                    fine += (amtPaid - 40) * .1; //add 10% of amount paid above first 40
                //if IS fake athlete
                if (isFakeAthlete == "y")
                    fine += (amtPaid - 40) * .22; // add 22% of amount paid above first 40
            }
            //if amount paid is greater than 250
            else
            {
                fine += (amtPaid-250) * .14; // add 14% of amount paid above 250
                
                //if is NOT fake athlete
                if (isFakeAthlete == "n")
                    fine += 210 * .1; //add 10% of middle 210
                //if IS fake athlete
                if (isFakeAthlete == "y")
                    fine += 210 * .22; //add 22% of middle 210
            }
        }
        //set precision to only one digit after decimal point
        cout.setf(ios::fixed);
        cout.precision(1);
        
        //print suggested fine statement with defendant's name and calculated fine
        cout << "The suggested fine for " << defendantsName << " is $" << fine << " thousand.";
    }
}
