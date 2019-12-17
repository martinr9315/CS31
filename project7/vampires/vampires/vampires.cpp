// vampires.cpp

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;                // max number of rows in the arena
const int MAXCOLS = 20;                // max number of columns in the arena
const int MAXVAMPIRES = 100;           // max number of vampires allowed

const int NORTH = 0;
const int EAST  = 1;
const int SOUTH = 2;
const int WEST  = 3;
const int NUMDIRS = 4;

const int EMPTY      = 0;
const int HAS_POISON = 1;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
              // type name, since it's mentioned in the Vampire declaration.

class Vampire
{
  public:
      // Constructor
    Vampire(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    void move();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;

    int m_poison; // can be 0, 1, 2, if 2 then vampire is dead
    bool m_shouldMove; //alternate whether or not vampire should move if it has drank one poison vial
};

class Player
{
  public:
      // Constructor
    Player(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    string dropPoisonVial();
    string move(int dir);
    void   setDead();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
};

class Arena
{
  public:
      // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();

      // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     vampireCount() const;
    int     getCellStatus(int r, int c) const;
    int     numberOfVampiresAt(int r, int c) const;
    void    display(string msg) const;

      // Mutators
    void setCellStatus(int r, int c, int status);
    bool addVampire(int r, int c);
    bool addPlayer(int r, int c);
    void moveVampires();

  private:
    int      m_grid[MAXROWS][MAXCOLS];
    int      m_rows;
    int      m_cols;
    Player*  m_player;
    Vampire* m_vampires[MAXVAMPIRES];
    int      m_nVampires;
    int      m_turns;

      // Helper functions
    void checkPos(int r, int c, string functionName) const;
    bool isPosInBounds(int r, int c) const;
};

class Game
{
  public:
      // Constructor/destructor
    Game(int rows, int cols, int nVampires);
    ~Game();

      // Mutators
    void play();

  private:
    Arena* m_arena;

      // Helper functions
    string takePlayerTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int lowest, int highest);
bool decodeDirection(char ch, int& dir);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Vampire implementation
///////////////////////////////////////////////////////////////////////////

Vampire::Vampire(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** A vampire must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "***** Vampire created with invalid coordinates (" << r << ","
             << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    
    m_poison = 0; //set initial poison to 0
    m_shouldMove = true; //initialize should move to true for new, unpoisoned vampire
}

int Vampire::row() const
{
    return m_row;
}

int Vampire::col() const
{
    return m_col;
}

bool Vampire::isDead() const
{
    if (m_poison == 2)
        return true; //if vampire has been poisoned twice return true
    return false;
}

void Vampire::move()
{
    if (m_shouldMove == true)
    {
        int dir = randInt(0, 3);
        
        attemptMove(*m_arena, dir, m_row, m_col);
        
        if (m_arena -> getCellStatus(m_row, m_col) == HAS_POISON)
        {
            m_poison++; //up poison status of vampire
            //cout<< "Poison of vampire has been increased to: "<<m_poison<<endl; //tester printout
            m_arena -> setCellStatus(m_row, m_col, EMPTY); //set spot that previously had poisoned blood vial to empty
        }
        
        if (m_poison == 1)
            m_shouldMove = false; //set shouldMove to false so if vampire still has poison value 1 and moved last turn it does not move next turn
    }
    else if (m_shouldMove == false)
    {
        //if it didn't move this turn, switch shouldMove so it moves next turn
        m_shouldMove = true;
        //cout << "vamp didnt move"<<endl; //tester printout
    }
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

string Player::dropPoisonVial()
{
    if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
        return "There's already a poisoned blood vial at this spot.";
    m_arena->setCellStatus(m_row, m_col, HAS_POISON);
    return "A poisoned blood vial has been dropped.";
}

string Player::move(int dir)
{
    if (!attemptMove(*m_arena, dir, m_row, m_col))
        return "Player couldn't move; player stands.";
    else if (m_arena->numberOfVampiresAt(m_row, m_col)>0)
    {
        setDead();
        return "Player walked into a vampire and died.";
    }
    else if (dir==NORTH)
        return "Player moved north.";
    else if (dir==SOUTH)
        return "Player moved south.";
    else if (dir==EAST)
        return "Player moved east.";
    else if (dir==WEST)
        return "Player moved west.";
    
    return "shouldn't get here";
}

bool Player::isDead() const
{
    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
             << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nVampires = 0;
    m_turns = 0;
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            setCellStatus(r, c, EMPTY);
}

Arena::~Arena()
{
  //Deallocate the player and all remaining dynamically allocated vampires.
    delete m_player;
    for (int i=0; i<m_nVampires; i++)
    {
        delete m_vampires[i];
    }
}

int Arena::rows() const
{
    return m_rows;
}

int Arena::cols() const
{
      return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::vampireCount() const
{
    return m_nVampires;
}

int Arena::getCellStatus(int r, int c) const
{
    checkPos(r, c, "Arena::getCellStatus");
    return m_grid[r-1][c-1];
}

int Arena::numberOfVampiresAt(int r, int c) const
{
    // Return the number of vampires at row r, column c
    int numV = 0;
    if (isPosInBounds(r, c))
    {
        for (int i=0; i<m_nVampires; i++)
            if (m_vampires[i]->row() == r && m_vampires[i]->col() == c) //if the row/col of any vampire matches arena position increment numV count by 1
                numV++;
    }
    return numV;
}

void Arena::display(string msg) const
{
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;
    
      // Fill displayGrid with dots (empty) and stars (poisoned blood vials)
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r-1][c-1] = (getCellStatus(r,c) == EMPTY ? '.' : '*');

      // If one vampire is at some grid point, set the displayGrid char to 'V'.  If it's 2 though 8, set it to '2' through '8'. For 9 or more, set it to '9'.
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
        {
            if (numberOfVampiresAt(r, c) == 1)
                displayGrid[r-1][c-1] = 'V';
            else if (numberOfVampiresAt(r, c) >= 2 && numberOfVampiresAt(r, c) <= 8)
                displayGrid[r-1][c-1] = numberOfVampiresAt(r, c) + '0';
            else if (numberOfVampiresAt(r, c) >= 9)
                displayGrid[r-1][c-1] = '9';
        }

      // Indicate player's position
    if (m_player != nullptr)
        displayGrid[m_player->row()-1][m_player->col()-1] = (m_player->isDead() ? 'X' : '@');

      // Draw the grid
    clearScreen();
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
            cout << displayGrid[r-1][c-1];
        cout << endl;
    }
    cout << endl;

      // Write message, vampire, and player info
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << vampireCount() << " vampires remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player!" << endl;
    else if (m_player->isDead())
        cout << "The player is dead." << endl;
    cout << m_turns << " turns have been taken." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
    checkPos(r, c, "Arena::setCellStatus");
    m_grid[r-1][c-1] = status;
}

bool Arena::addVampire(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;

      // Don't add a vampire on a spot with a poisoned blood vial
    if (getCellStatus(r, c) != EMPTY)
        return false;

      // Don't add a vampire on a spot with a player
    if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
        return false;

      // If there are MAXVAMPIRES existing vampires, return false.  Otherwise,
      // dynamically allocate a new vampire at coordinates (r,c).  Save the
      // pointer to newly allocated vampire and return true.
    if (m_nVampires >= MAXVAMPIRES)
        return false;
    else
    {
        m_vampires[m_nVampires] = new Vampire(this, r, c);
        m_nVampires++;
    }
    
    return true;
}

bool Arena::addPlayer(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;

      // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

      // Don't add a player on a spot with a vampire
    if (numberOfVampiresAt(r, c) > 0)
        return false;

    m_player = new Player(this, r, c);
    return true;
}

void Arena::moveVampires()
{
      // Move all vampires. Move each vampire.  Mark the player as dead if necessary. Deallocate any dead dynamically allocated vampire.
  
    for (int i=0; i<m_nVampires; i++)
    {
        m_vampires[i] -> move();
        
        if (m_vampires[i]->row() == m_player->row() && m_vampires[i]->col() == m_player->col()) //check if vampire moved onto player
            m_player->setDead();
        
        if (m_vampires[i]->isDead())
        {
            delete m_vampires[i];
            //cout<< "vamp is dead"<<endl; //tester print out
            m_vampires[i] = m_vampires[m_nVampires-1]; //set current vampire pointer to last interesting vampire pointer
            m_nVampires--; //decrease number of interesting vampire pointers
            i--; //decrease i so this loop runs again for the vampire pointer that was just moved to the current spot
        }
    }
    
      // Another turn has been taken
    m_turns++;
}

bool Arena::isPosInBounds(int r, int c) const
{
    return (r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols);
}

void Arena::checkPos(int r, int c, string functionName) const
{
    if (r < 1  ||  r > m_rows  ||  c < 1  ||  c > m_cols)
    {
        cout << "***** " << "Invalid arena position (" << r << ","
             << c << ") in call to " << functionName << endl;
        exit(1);
    }
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nVampires)
{
    if (nVampires < 0)
    {
        cout << "***** Cannot create Game with negative number of vampires!" << endl;
        exit(1);
    }
    if (nVampires > MAXVAMPIRES)
    {
        cout << "***** Trying to create Game with " << nVampires
             << " vampires; only " << MAXVAMPIRES << " are allowed!" << endl;
        exit(1);
    }
    int nEmpty = rows * cols - nVampires - 1;  // 1 for Player
    if (nEmpty < 0)
    {
        cout << "***** Game created with a " << rows << " by "
             << cols << " arena, which is too small too hold a player and "
             << nVampires << " vampires!" << endl;
        exit(1);
    }

      // Create arena
    m_arena = new Arena(rows, cols);

      // Add player
    int rPlayer;
    int cPlayer;
    do
    {
        rPlayer = randInt(1, rows);
        cPlayer = randInt(1, cols);
    } while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
    m_arena->addPlayer(rPlayer, cPlayer);

      // Populate with vampires
    while (nVampires > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (r == rPlayer && c == cPlayer)
            continue;
        m_arena->addVampire(r, c);
        nVampires--;
    }
}

Game::~Game()
{
    delete m_arena;
}

string Game::takePlayerTurn()
{
    for (;;)
    {
        cout << "Your move (n/e/s/w/x or nothing): ";
        string playerMove;
        getline(cin, playerMove);

        Player* player = m_arena->player();
        int dir;

        if (playerMove.size() == 0)
        {
            if (recommendMove(*m_arena, player->row(), player->col(), dir))
                return player->move(dir);
            else
                return player->dropPoisonVial();
        }
        else if (playerMove.size() == 1)
        {
            if (tolower(playerMove[0]) == 'x')
                return player->dropPoisonVial();
            else if (decodeDirection(playerMove[0], dir))
                return player->move(dir);
        }
        cout << "Player move must be nothing, or 1 character n/e/s/w/x." << endl;
    }
}

void Game::play()
{
    m_arena->display("");
    Player* player = m_arena->player();
    if (player == nullptr)
        return;
    while ( ! player->isDead()  &&  m_arena->vampireCount() > 0)
    {
        string msg = takePlayerTurn();
        m_arena->display(msg);
        if (player->isDead())
            break;
        m_arena->moveVampires();
        m_arena->display(msg);
    }
    if (player->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

  // Return a uniformly distributed random int from lowest to highest, inclusive
int randInt(int lowest, int highest)
{
    if (highest < lowest)
        swap(highest, lowest);
    static random_device rd;
    static default_random_engine generator(rd());
    uniform_int_distribution<> distro(lowest, highest);
    return distro(generator);
}

bool decodeDirection(char ch, int& dir)
{
    switch (tolower(ch))
    {
      default:  return false;
      case 'n': dir = NORTH; break;
      case 'e': dir = EAST;  break;
      case 's': dir = SOUTH; break;
      case 'w': dir = WEST;  break;
    }
    return true;
}

  // Return false without changing anything if moving one step from (r,c)
  // in the indicated direction would run off the edge of the arena.
  // Otherwise, update r and c to the position resulting from the move and
  // return true.
bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
    if (dir==NORTH)
    {
        if (r-1 > a.rows() || r-1 < 1)
            return false;
        r--;
    }
    else if (dir==SOUTH)
    {
        if (r+1 > a.rows() || r+1 < 1)
            return false;
        r++;
    }
    else if (dir==EAST)
    {
        if (c+1 > a.cols() || c+1 < 1)
            return false;
        c++;
    }
    else if (dir==WEST)
    {
        if (c-1 > a.cols() || c-1 < 1)
            return false;
        c--;
    }

    return true;
}

  // Recommend a move for a player at (r,c):  A false return means the
  // recommendation is that the player should drop a poisoned blood vial and
  // not move; otherwise, this function sets bestDir to the recommended
  // direction to move and returns true.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{

    
    int risk[5] = {100, 100, 100, 100, 0}; //risk of moving [north, east, south, west, staying]
    
    //calculate risk of moving north
    int northR = r-1; //row value of position one move north
    if (northR>=1 && a.numberOfVampiresAt(northR, c)==0) //check if moving north is a valid move and that there is no vampire already at that spot
    {
        risk[0] = 0; //set risk back to zero
        //increment risk by number of vampires in 3 spaces around potential spot to move to
        risk[0]+=a.numberOfVampiresAt(northR-1, c);
        risk[0]+=a.numberOfVampiresAt(northR, c+1);
        risk[0]+=a.numberOfVampiresAt(northR, c-1);
    }
    
    //calculate risk of moving east
    int eastC = c+1; //col value of position one move east
    if (eastC<=a.cols() && a.numberOfVampiresAt(r, eastC)==0 ) //check if moving east is a valid move and that there is no vampire already at that spot
    {
        risk[1] = 0; //set risk back to zero
       //increment risk by number of vampires in 3 spaces around potential spot to move to
        risk[1]+=a.numberOfVampiresAt(r-1, eastC);
        risk[1]+=a.numberOfVampiresAt(r+1, eastC);
        risk[1]+=a.numberOfVampiresAt(r, eastC+1);
    }

    
    //calculate risk of moving south
    int southR = r+1; //row value of position one move south
    if (southR<=a.rows() && a.numberOfVampiresAt(southR, c)==0) //check if moving south is a valid move and that there is no vampire already at that spot
    {
        risk[2] = 0; //set risk back to zero
       //increment risk by number of vampires in 3 spaces around potential spot to move to
        risk[2]+=a.numberOfVampiresAt(southR+1, c);
        risk[2]+=a.numberOfVampiresAt(southR, c-1);
        risk[2]+=a.numberOfVampiresAt(southR, c+1);
    }

    
    //calculate risk of moving west
    int westC = c-1; //col value of position one move west
    if (westC>=1 && a.numberOfVampiresAt(r, westC)==0) //check if moving west is a valid move and that there is no vampire already at that spot
    {
        risk[3] = 0; //set risk back to zero
        //increment risk by number of vampires in 3 spaces around potential spot to move to
        risk[3]+=a.numberOfVampiresAt(southR+1, c);
        risk[3]+=a.numberOfVampiresAt(southR, c-1);
        risk[3]+=a.numberOfVampiresAt(southR, c+1);
    }

    //calculate risk of staying in same spot
    risk[4]+=a.numberOfVampiresAt(r+1, c);
    risk[4]+=a.numberOfVampiresAt(r-1, c);
    risk[4]+=a.numberOfVampiresAt(r, c+1);
    risk[4]+=a.numberOfVampiresAt(r, c-1);
    
    int lowestRisk=100;
    int lowestRiskIndex = 4;
    //cout<<"Risk array: "; //test print out
    for (int i=0; i<5; i++)
    {
        //cout<<risk[i]<<", "; //test print out
        if (risk[i]<lowestRisk)
        {
            lowestRisk = risk[i];
            lowestRiskIndex=i;
        }
    }
    //cout << endl; //test print out
    if (lowestRiskIndex==4 || risk[lowestRiskIndex]==risk[4]) //if the lowest risk index is the fourth position (the stay risk value) then return false and stay
        return false;
    
    bestDir = lowestRiskIndex; //otherwise set the bestDir to 0,1,2,3, the position of the direction with the lowest risk
    return true;
    
}

///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

int main()
{
      // Create a game
      // Use this instead to create a mini-game: Game g(3, 5, 2);
   Game g(10, 12, 40);

      // Play the game
    g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++/g31 UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _MSC_VER  //  Microsoft Visual C++

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                                                        &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif

