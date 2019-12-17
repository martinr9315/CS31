#include "grid.h"
#include <cctype>
#include <iostream>
#include <cassert>
#include <string>
using namespace std;

const int HORIZ = 0;
const int VERT = 1;

const int FG = 0;
const int BG = 1;

bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg);
int performCommands(string commandString, char& plotChar, int& mode, int& badPos);

bool processHorVCommand(string text, int& pos, int& distance);

int main()
{
    setSize(20, 30);
    char currentChar = '*';
    int currentMode = FG;
    for (;;)
    {
        cout << "Enter a command string: ";
        string cmd;
        getline(cin, cmd);
        if (cmd == "")
            break;
        int position;
        int status = performCommands(cmd, currentChar, currentMode, position);
        switch (status)
        {
            case 0:
                draw();
                break;
            case 1:
                cout << "Syntax error at position " << position+1 << endl;
                break;
            case 2:
                cout << "Cannot perform command at position " << position+1 << endl;
                break;
            default:
                // It should be impossible to get here.
                cerr << "performCommands returned " << status << "!" << endl;
                
        }
    }
}

bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg)
{
    if (isprint(plotChar)) //check if plotChar is printable
    {
        if (dir == HORIZ) //horizontal lines
        {
            if (distance>=0) //to the right (positive distance)
            {
                //check if points are plottable, if not: return false
                for (int i=0; i<=distance; i++)
                    if (!(r >= 1 && r <= getRows() && c+i >=1 && c+i <=getCols()))
                        return false;
                //check if line (which we now know to be plottable) should be in the foreground or background
                if (fgbg == FG) //if foreground
                    for (int i=0; i<=distance; i++)
                        setChar(r, c+i, plotChar); //plot all points
                else if (fgbg == BG) //if background
                {
                    for (int i=0; i<=distance; i++)
                        if (getChar(r,c+i)==' ') //check if char at given point is ' '
                            setChar(r, c+i, plotChar); //then plot only those points
                }
                else //if anything else, return false
                    return false;
                
                //if we get this far, the points have been plotted and we are done, so return true.
                return true;
            }
            else //to the left (negative distance)
            {
                //check if points are plottable, if not: return false
                for (int i=0; i>=distance; i--)
                    if (!(r >= 1 && r <= getRows() && c+i >=1 && c+i <=getCols()))
                        return false;
                //check if line (which we now know to be plottable) should be in the foreground or background
                if (fgbg == FG) //if foreground
                    for (int i=0; i>=distance; i--)
                        setChar(r, c+i, plotChar); //plot all points
                else if (fgbg == BG) //if background
                {
                    for (int i=0; i>=distance; i--)
                        if (getChar(r,c+i)==' ') //check if char at given point is ' '
                            setChar(r, c+i, plotChar); //then plot only those points
                }
                else
                    return false;
                //if we get this far, the points have been plotted and we are done, so return true.
                return true;
            }
        }
        if (dir == VERT) //vertical lines
        {
            if (distance>=0) //downwards (positive distance)
            {
                //check if points are plottable, if not: return false
                for (int i=0; i<=distance; i++)
                    if (!(r+i >= 1 && r+i <= getRows() && c >=1 && c <=getCols()))
                        return false;
                //check if line (which we now know to be plottable) should be in the foreground or background
                if (fgbg == FG) //if foreground
                    for (int i=0; i<=distance; i++)
                        setChar(r+i, c, plotChar); //plot all points
                else if (fgbg == BG) //if background
                {
                    for (int i=0; i<=distance; i++)
                        if (getChar(r+i,c)==' ') //check if char at given point is ' '
                            setChar(r+i, c, plotChar); //then plot only those points
                }
                else
                    return false;
                
                //if we get this far, the points have been plotted and we are done, so return true.
                return true;
            }
            else //upwards (negative distance)
            {
                //check if points are plottable, if not: return false
                for (int i=0; i>=distance; i--)
                    if (!(r+i >= 1 && r+i <= getRows() && c >=1 && c <=getCols()))
                        return false;
                //check if line (which we now know to be plottable) should be in the foreground or background
                if (fgbg == FG) //if foreground
                    for (int i=0; i>=distance; i--)
                        setChar(r+i, c, plotChar); //plot all points
                else if (fgbg == BG) //if background
                {
                    for (int i=0; i>=distance; i--)
                        if (getChar(r+i,c)==' ') //check if char at given point is ' '
                            setChar(r+i, c, plotChar); //then plot only those points
                }
                else
                    return false;
                
                //if we get this far, the points have been plotted and we are done, so return true.
                return true;
            }
        }
    }
                    
    return false;
} //close plotLine

int performCommands(string commandString, char& plotChar, int& mode, int& badPos)
{
    int r=1;
    int c=1; //set initial cursor position to 1,1
    
    int i=0;
    while (i!=commandString.size())
    {
        if (commandString[i]=='H'||commandString[i]=='h') //detect horizontal line command
        {
            int distance=0;
            int cannotPlotI = 0;
            cannotPlotI = i;
            if (processHorVCommand(commandString, i, distance)) //if characters after the h/H constitute correct command
            {
                //plot horizontal line with read-in distance
                if (!plotLine(r, c, distance, HORIZ, plotChar, mode))
                {
                    //otherwise return position of unplottable call with error code 2
                    badPos = cannotPlotI;
                    return 2;
                }
            }
            else
            {
              // return position of syntax error call with error code 1
              badPos = i;
              return 1;
            }
          
          c += distance; //update c to new position
            
        }// close H command
        
        else if (commandString[i]=='V'||commandString[i]=='v') //detect vertical line command
        {
            int distance=0;
            int cannotPlotI = 0;
            cannotPlotI = i;
            if (processHorVCommand(commandString, i, distance)) //if characters after the v/V constitute correct command
            {
                //plot vertical line with read-in distance
                if (!plotLine(r, c, distance, VERT, plotChar, mode))
                {
                    //otherwise return position of unplottable call with error code 2
                    badPos = cannotPlotI;
                    return 2;
                }
            }
            else
            {
              // return position of syntax error call with error code 1
              badPos = i;
              return 1;
            }
            r += distance; //update r to new postion
        }// close V command
        
        else if (commandString[i]=='F'||commandString[i]=='f') //detect foreground command
        {
            i++; //move to next position
            if (isprint(commandString[i]))
            {
                plotChar = commandString[i];
                mode = FG;
            }
            else
            {
                // return position of syntax error call with error code 1
                badPos = i;
                return 1;
            }
            
            i++; //update position
        }
        else if (commandString[i]=='B'||commandString[i]=='b') //detect background command
        {
            i++; //move to next position
            if (isprint(commandString[i]))
            {
                plotChar = commandString[i];
                mode = BG;
            }
            else
            {
            // return position of syntax error call with error code 1
                badPos = i;
                return 1;
            }
            
            i++; //update position
        }
        else if (commandString[i]=='C'||commandString[i]=='c') //detect clear command
        {
            for (int k=1; k <= getRows(); k++)
                for (int j=1; j <= getCols(); j++)
                    setChar(k, j, ' ');
            
            r=1;
            c=1; //set cursor position to 1,1
            
            plotChar = '*'; //set current character to *
           
            mode = FG; //set mode to foreground
            
            i++; //move to next position
        }
        else
        {
            // return position of syntax error call with error code 1
                badPos = i;
                return 1;
        }
      
  }
    return 0;
}


bool processHorVCommand(string text, int& pos, int& distance)
{
    //Get distance.  We know text[pos] is a h or H
    
    pos++; //move position to char after H/h
    string distanceStr;
    
     //if next two characters are both digits
    if (isdigit(text[pos]) && isdigit(text[pos+1]))
    {
       distanceStr = text[pos];
       distanceStr += text[pos+1];
       distance = stoi(distanceStr); //set distance to two digit integer after h
       pos=pos+2; //update position
    }
    //if next three characters are minus sign and two digits
    else if (text[pos]=='-' && isdigit(text[pos+1]) && isdigit(text[pos+2]))
    {
        distanceStr = text[pos+1];
        distanceStr += text[pos+2];
        distance = stoi(distanceStr) * -1; //set distance to two digit negative integer after h
        pos=pos+3; //update position
   }
   //if next two characters are minus sign and one digit
   else if (text[pos]=='-' && isdigit(text[pos+1]))
   {
       distanceStr = text[pos+1];
       distance = stoi(distanceStr) * -1; //set distance to one digit negative integer after h
       pos=pos+2; //update position
   }
   //if next character is one digit
   else if (isdigit(text[pos]))
   {
       distanceStr = text[pos];
       distance = stoi(distanceStr); //set distance to one digit integer after h
       pos++; //update position
   }
    else
        return false; //if not any of these cases return false, pos still equal to original i
    
    return true; //if get to here, must have been one of the correct syntax cases so return true
}


