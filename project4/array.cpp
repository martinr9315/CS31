//
//  main.cpp
//  array
//
//  Created by Roxane Martin on 11/3/19.
//  Copyright © 2019 RM. All rights reserved.
//

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

void printArray(string a[], int n);

int appendToAll(string a[], int n, string value);
int lookup(const string a[], int n, string target);
int positionOfMax(const string a[], int n);
int rotateLeft(string a[], int n, int pos);
int countRuns(const string a[], int n);
int flip(string a[], int n);
int differ(const string a1[], int n1, const string a2[], int n2);
int subsequence(const string a1[], int n1, const string a2[], int n2);
int lookupAny(const string a1[], int n1, const string a2[], int n2);
int separate(string a[], int n, string separator);

int main() {
    //string people[5] = { "donald", "lindsey", "fiona", "rudy", "mick" };
    //int j = appendToAll(people, 5, "!!!");  // returns 5
    
    //string officeholders[5] = { "donald", "lindsey", "mike", "adam", "nancy" };
    //int j = lookup(officeholders, 5, "adam");  // returns 3
    
   // string people[5] = { "donald", "lindsey", "fiona", "rudy", "mick" };
    //int i = lookup(people, 3, "mick");  // should return -1 (not found)
    
    //string persons[6] = { "donald", "lindsey", "marie", "rudy", "fiona", "adam" };
   // int k = positionOfMax(persons, 6);   // returns 3, since  rudy  is latest
                                         // in alphabetic order
    
   // string politician[5] = { "mike", "donald", "lindsey", "nancy", "adam" };
   // int m = rotateLeft(politician, 5, 1);  // returns 1
        // politician now contains:  "mike", "lindsey", "nancy", "adam", "donald"
    
    //string d[9] = {"rudy", "adam", "mike", "mike", "fiona", "fiona", "fiona", "mike", "mike"};
    //int p = countRuns(d, 9);  //  returns 5
    
   // string folks[7] = { "adam", "", "fiona", "mike", "rudy", "nancy", "donald" };
  //  int q = flip(folks, 5);  // returns 5
    // folks now contains:  "rudy"  "mike"  "fiona"  ""  "adam"  "nancy"  "donald"
    
    //string folks[7] = { "adam", "", "fiona", "mike", "rudy", "nancy", "donald" };
   // string group[6] = { "adam", "", "fiona", "donald", "mike", "rudy" };
    //int r = differ(folks, 7, group, 6);  //  returns 3
    //int s = differ(folks, 2, group, 3);  //  returns 2
    
    //string names[10] = { "marie", "gordon", "marie", "nancy", "mick", "adam", "lindsey" };
    //string names1[10] = { "marie", "nancy", "mick" };
    //int t = subsequence(names, 7, names1, 3);  // returns 1
    //string names2[10] = { "gordon", "mick" };
    //int u = subsequence(names, 5, names2, 2);  // returns -1
    
    //string names[10] = { "gordon", "lucie", "nancy", "ally", "adam", "lindsey" };
    //string set1[10] = { "donald", "adam", "mick", "marie" };
    //int v = lookupAny(names, 6, set1, 4);  // returns 1 (a1 has "marie" there)
    //string set2[10] = { "rudy", "fiona" };
    //int w = lookupAny(names, 6, set2, 2);  // returns -1 (a1 has none)
    
    //string persons[6] = { "donald", "lindsey", "marie", "rudy", "fiona", "adam" };
    //int x = separate(persons, 6, "marie");  //  returns 3
    
    //cout << x << endl;
    //printArray(persons, 6);
    //cout << endl;
    string h[7] = { "mick", "marie", "lucie", "roxy", "", "gordon", "lindsey" };
    assert(lookup(h, 7, "gordon") == 5);
    assert(lookup(h, 7, "fiona") == -1);
    assert(lookup(h, 2, "roxy") == -1);
    assert(positionOfMax(h, 7) == 3);
    
    string g[4] = { "mick", "marie", "lindsey", "nancy" };
    assert(differ(h, 4, g, 4) == 2);
    assert(appendToAll(g, 4, "?") == 4 && g[0] == "mick?" && g[3] == "nancy?");
    assert(rotateLeft(g, 4, 1) == 1 && g[1] == "lindsey?");
    
    string e[4] = { "fiona", "rudy", "", "gordon" };
    assert(subsequence(h, 7, e, 4) == -1);
    string j[4] = { "roxy", "", "gordon", "lindsey"};
    assert(subsequence(h, 7, j, 4) == 3);
    string d[5] = { "mary", "mary", "mary", "ally", "ally" };
    assert(countRuns(d, 5) == 2);
    string f[3] = { "lindsey", "fiona", "mike"};
    assert(lookupAny(h, 7, f, 3) == 6);
    string k[3] = { "allison", "becca", "mike" };
    assert(lookupAny(h, 7, k, 3) == -1);
    assert(flip(k, 3) == 3 && k[0] == "mike" && k[2] == "allison");
    
    assert(separate(h, 7, "patrick") == 6 && h[6] == "roxy");
    
    assert(lookup(h, -5, "patrick") == -1);
    assert(differ(h, 4, g, -5) == -1);
    assert(positionOfMax(h, -5) == -1);
    assert(separate(h, -5, "patrick") == -1);
    assert(rotateLeft(g, -5, 1) == -1);
    assert(countRuns(d, -5) == -1);
    assert(subsequence(h, 7, j, -5) == -1);
    assert(flip(k, -5) == -1);
    assert(appendToAll(g, -5, "?") == -1);
    assert(lookupAny(h, 7, k, -5) == -1);


    cout << "All tests succeeded" << endl;

}

void printArray(string a[], int n)
{
     for (int i=0; i<n; i++)
     {
         cout << a[i] << ", ";
     }
}

int appendToAll(string a[], int n, string value)
{
    if (n>=0) //check that n is positive
    {
        for (int i=0; i<n; i++)
        {
            a[i] += value; //append value to each string in array
        }
        return n;
    }
    return -1; //if input is negative return -1
}

int lookup(const string a[], int n, string target)
{
    if (n>=0) //check that n is positive
    {
        for (int i=0; i<n; i++)
        {
            if (a[i]==target)
                return i; //if element matches target return element index
        }
    }
    return -1; //if input is negative return -1 or if no match is found in array
}

int positionOfMax(const string a[], int n)
{
    if (n>=0) //check that n is positive
      {
          int maxPos = 0;
          for (int i=0; i<n; i++)
          {
              if (a[i]>a[maxPos])
                  maxPos = i;
          }
          return maxPos;
      }
    return -1;
}

int rotateLeft(string a[], int n, int pos)
{
    if (n>=0) //check that n is positive
    {
        string deletedElement = a[pos];
        for (int i=pos; i<n-1; i++)
        {
            a[i]=a[i+1];
        }
        a[n-1] = deletedElement;
        return pos;
    }
    return -1;
}

int countRuns(const string a[], int n)
{
    if (n>=0) //check that n is positive
      {
          int numRuns = 1;
          for (int i=0; i<n-1; i++)
          {
              if (a[i]!=a[i+1])
                  numRuns++;
          }
          return numRuns;
      }
    return -1;
}

int flip(string a[], int n)
{
    if (n>=0) //check that n is positive
       {
           int j = n-1;
           int i = 0;
           while(i<j)
           {
               string temp=a[i];
               a[i]=a[j];
               a[j]=temp;
               i++;
               j--;
           }
           return n;
       }
    return -1;
}

int differ(const string a1[], int n1, const string a2[], int n2)
{
    if (n1 >= 0 && n2 >= 0)
    {
        int n=0;
        if (n1 < n2)
            n = n1; //set loop max to smaller of two array sizes
        else
            n = n2;
        
        for (int i=0; i<n; i++)
            if (a1[i] != a2[i])
                return i;
        
        return n;
    }
    return -1;
}

int subsequence(const string a1[], int n1, const string a2[], int n2)
{
    if (n1 >= 0 && n2 >= 0)
    {
        
        for (int i=0; i<n1; i++)
        {
            int n = 0; //sets counter variable to 0
            if (a1[i] == a2[0]) //search through a1 for first element of a2 sequence
            {
                for (int j=0; j<n2; j++) //loop through all of a2 to see if they appear consecutively in a1
                {
                    if (a1[i+j] == a2[j])
                        n++; //increment counter variable every time next element is found
                }
            }
            if (n == n2) //if entire sequence is found
                return i; //return start index of sequence
        }
    }
    return -1;
}

int lookupAny(const string a1[], int n1, const string a2[], int n2)
{
    if (n1 >= 0 && n2 >= 0)
    {
        for (int i=0; i<n1; i++)
        {
            for (int j=0; j<n2; j++)
            {
                if (a1[i] == a2[j])
                    return i;
            }
        }
    }
    return -1;
}

int separate(string a[], int n, string separator)
{
    if (n>=0) //check that n is positive
    {
        int lessThan = -1;
        for (int i=0; i<n; i++) //go through array and find index of separation
        {
            if (a[i]<separator)
            lessThan++;
        }
        for (int i=0; i<n; i++)
        {
            if (a[i]>separator && i<=lessThan) //if element is greater than separator and less than lessThan index
            {
                for (int j=lessThan; j<n; j++)
                {
                    if (a[j]<separator) //loop through elements after lessThan and find element greater than separator
                    {
                        //flip these two elements
                        string temp = a[i];
                        a[i]=a[j];
                        a[j] = temp;
                    }
                }
            }
        }
        return lessThan+1;
    }
    return -1;
}
