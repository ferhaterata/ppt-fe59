<!--
// -----------------------------------------------------------------------------
// File: notes.txt -------------------------------------------------------------
// Created by Ferhat Erata <ferhat.erata@yale.edu> on October 29, 2019
// Modified by Ferhat Erata <ferhat.erata@yale.edu> on November 11, 2019
// Modified by Ferhat Erata <ferhat.erata@yale.edu> on November 24, 2019
// -----------------------------------------------------------------------------
-->

Report (PS7)
======

Testing and Valgrind
--------------------
* I created a bash script (test.sh) to run 32 test cases; and the outputs with and
without valgrind can be reached at `output` and `output.valgrind` files. Those test 
cases are also run from the binary by calling `kalah --test`. I implemented all the 
cases instructed in the problem set. 

Snapshot Class
--------------
* I designed the Snapshot class with two private data members: Player activePlayer 
and vector<int> seeds. This class could be a *struct* to be used as a plain data 
object with public members; however, in the specification, it is requested to be 
a class having private data members. Therefore, it is implemented as a class and
I overloaded construction of snapshots in order to use properly in the late bind-
ing of Game in Referee class. 
* This class also implements insertion and extraction operators in order to be 
able to serialize and deserialize a Snapshot object from an output or input streams 
respectively. This is due to delegation principle, it is expert at both operation.
* Additionally, Snapshot class had value semantics and The rule of Zero applies 
here; therefore, I defaulted five special member functions, namely copy/move 
constructor copy/move assignment and default destructor.
````
class Snapshot {
  private:
    Player activePlayer;
    std::vector<int> seeds;

  public:
    explicit Snapshot(const Game& g); // Normal construction/initialization
    // For Late Binding
    explicit Snapshot(Player pl = Player::SOUTH, std::vector<int> seeds = {});

    // Snapshot class had value semantics and The rule of Zero applies here:
    Snapshot(const Snapshot&) = default;
    Snapshot(Snapshot&&) = default;
    Snapshot& operator=(const Snapshot&) = default;
    Snapshot& operator=(Snapshot&&) = default;
    ~Snapshot() = default;

    // Snapshot is a expert at serializing the snapshot.
    std::ostream& writeSnapshot(std::ostream& out) const;
    // Snapshot is a expert at extracting the snapshot.
    std::istream& readSnapshot(std::istream& in);

    [[nodiscard]] Player getPlayer() const { return activePlayer; }
    [[nodiscard]] const std::vector<int>& getSeeds() const { return seeds; }
};

// serialize a snapshot to output stream
inline std::ostream& operator<<(std::ostream& out, const Snapshot& snapshot) {
    return snapshot.writeSnapshot(out);
}

// deserialize a snapshot from input stream
inline std::istream& operator>>(std::istream& in, Snapshot& snapshot) {
    return snapshot.readSnapshot(in);
}
````

Referee Class
-------------

Due to late binding issue, I turned the static storage member Game class into 
dynamic storage member (heap allocated) using pointer. Consequently, Referee
is responsible for the Game object as well as North and South agents. Due to 
exclusive ownerships in the Referee, I decided to use std::unique_ptr instead 
of using raw pointers as recommended by [ISO C++ Core Guidelines]
(https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rh-smart)
in order to prevent accidental leaks because of forgetting to destroy objects.
Unique pointers are initialized as nullptr as usual pointer semantics.
In this class, there is also en enumeration for Agent type having implemented 
extraction operator to delegate deserialization of an AgentType literal from 
input stream.
```` 
// -----------------------------------------------------------------------------
// There are two legal string arguments: human and machine.
enum class AgentType { HUMAN, MACHINE };

// -----------------------------------------------------------------------------
// The interface between the user and the abstract game. It creates, manages,
// and has custody of a Game object. It displays the board, announces whose turn
// it is, and prompts the user to enter a move (i.e., a house number)
class Referee {
  private:
    // The Referee creates and has the ownership of Game and Agents
    // unique pointers are initialized as nullptr as usual pointer semantics
    std::unique_ptr<Game> game{};     // Unique ownership with std::unique_ptr
    std::unique_ptr<Agent> north{};   // The Referee has one Agent* for North
    std::unique_ptr<Agent> south{};   // The Referee has one Agent* for South
    std::vector<Snapshot> undo_stack; // undo stack of game states
    std::vector<Snapshot> redo_stack; // redo stack of game states

    Agent* getAgent(Player pl); // gets the Agent

  public:
    Referee(int numHouses, int numSeeds, AgentType n, AgentType s);

    explicit Referee(const std::string& filename);

    ~Referee() = default;

    void playGame();                   // The top-level function
    void save(const string& filename); // saves the game to a file
    void undo();                       // undo operation
    void redo();                       // redo operation
};

// -----------------------------------------------------------------------------
// deserialize an AgentType literal from input stream
inline std::istream& operator>>(std::istream& in, AgentType& agentType) {
    std::string agent;
    if (!(in >> agent))
        throw Fatal("Error reading from stream during Agent's type extraction");
    if (agent == "human")
        agentType = AgentType::HUMAN;
    else if (agent == "machine")
        agentType = AgentType::MACHINE;
    else
        throw Fatal("Wrong agent type '%s': expecting 'machine' or 'human'",
                    agent.data());
    return in;
}
````

The following operations are declared and defined as requested in the specification.

`void Referee::playGame() ` 
`void Referee::save(const string& filename)` 
`void Referee::undo() ` 
`void Referee::redo()`

In `Referee::save` operation, in order to know if an agent is human or machine 
at run time, I had to use `dynamic_cast` in an if statement as introduced in the 
lecture. A failed dynamic pointer cast `dynamic cast<T>()` returns nullptr.
````
// -----------------------------------------------------------------------------
// saves the game's snapshot to a file.
void Referee::save(const string& filename) {
    const auto& gr = *game; // const reference to game
    std::ofstream fout(filename);
    fout << gr.getNumHouses() << " ";
    if (dynamic_cast<const HumanAgent*>(south.get()))
        fout << "human ";
    else
        fout << "machine ";
    if (dynamic_cast<const HumanAgent*>(north.get()))
        fout << "human";
    else
        fout << "machine";
    fout << "\n";
    fout << Snapshot(gr); // inserts snapshot to the file.
    fout.close();
}
````

I introduced a new Referee constructor that takes a filename as its only parameter 
and builds a new Game from the data in the given file. This maintains "Late Binding" 
strategy using a unique pointer to a Game object. The implementation is clear and 
operations carefully ordered in order to prevent possible memory leaks because of 
throwing exceptions. Nevertheless, in any case, unique pointer as a smart pointer
does the job.
````
// -----------------------------------------------------------------------------
Referee::Referee(const std::string& filename) { ... }

Referee::Referee(int numHouses, int numSeeds, AgentType s, AgentType n)
    : game(std::make_unique<Game>(numHouses, numSeeds)) { ... }
````

I used `emplace_back` because it avoids the extra copy or move operation required 
when using `push_back`.
````
// -----------------------------------------------------------------------------
void Referee::undo() {
    // If the undo operation would cause an empty stack to be popped, the
    // operation fails. In this case, the game state and stacks should be left
    // unchanged.
    auto& gr = *game; // const reference to game
    if (!undo_stack.empty()) {
        redo_stack.emplace_back(gr);
        const auto s = undo_stack.back(); // const Snapshot Referee::s
        undo_stack.pop_back();
        gr.restoreState(s);
    } else {
        std::cout << "The undo stack is empty!\n";
    }
}
````

The playGame logic is extended to be able to catch exceptions thrown by human agent. 
Here is the code excerpt showing the implementation details.
````
void Referee::playGame() {
    string str;
    int move = 0;
    auto& gr = *game; // reference to game
    while (!gr.isOver()) {
        std::cout << "\n";
        Player active = gr.activePlayer();
        std::cout << active << "'s turn:\n";
        std::cout << gr << std::endl;
        auto pl = getAgent(active);
        try {
            move = pl->chooseMove(active);
            if (!gr.isValidMove(move)) {
                std::cout << "This is not a valid move: " << str << "\n";
                continue;
            }
        } catch (Save& s) {
            std::cout << "Saving game at user's request\n";
            save(s.getFilename());
            continue;
        } catch (Redo& r) {
            std::cout << "Redoing game at user's request\n";
            redo();
            continue;
        } catch (Undo& u) {
            std::cout << "Undoing game at user's request\n";
            undo();
            continue;
        } catch (Quit& q) {
            std::cout << "Quitting game at user's request\n";
            return;
        } catch (Fatal& e) {
            throw e;
        }

        std::cout << gr.activePlayer() << " playing move " << move << "\n";
        // during normal operation of the game, a new snapshot is created and
        // pushed onto the undo stack just before each move.
        undo_stack.emplace_back(gr);
        // perform a move in the game.
        gr.doMove(move);
        ....
}
````

Game Class
----------

I added Forward declarating in order to prevent circular dependencies among tightly
coupled Snapshot, Game and Referee Classes. 

````
....
class Snapshot; // Forward Declaration due to circular dependencies among
                // Snapshot, Game, and Referee Classes

//------------------------------------------------------------------------------
// A Game manages the overall play of the game. It creates and has custody of
// the Board object that is used to play the game. It has Knowledge of the rules
// of Kalah game.
class Game { .... }
````

I introduced default parameter to the constructor so that a game object can be
initialized from a file without knowing initial the number of seeds.
Apart from that, 4 special member functions are deleted because of const data member
and/or because of deleting copy/move constructor of Board. Details can be seen in the
following code excerpt.
````
....
  public:
    explicit Game(int numHouses, int numSeeds = 0); // having default value

    Game(const Game&) = delete; // Board's copy constructor is deleted.
    Game(Game&&) = delete;      // Board's move constructor is deleted.
    Game& operator=(const Game&) = delete; // Board's copy assign. is deleted.
    Game& operator=(Game&&) = delete;      // Game has a const data member.
    ~Game() = default;
....
````
More importantly I allowed Game class to restore its state from a Snapshot. Additionally, 
I delegated seed-gathering operation to an existing function in Board as recommended in the 
specification.
````
....
    void doMove(int move);
    void endGame();
    // modifies the game state to be the one contained in ss.
    void restoreState(const Snapshot& ss);
....
    // delegate seed-gathering operation to an existing function in Board.
    [[nodiscard]] int getContent(Player pl, int n) const;
````

I also made small changes to the board class. I applied unique_pointer approach
the pointer array to claim the ownership of Pits. Because of const unique pointer
I deleted copy/move constructors as well as copy/move assignments.
````
// -----------------------------------------------------------------------------
class Board {
  private:
    const int numHouses;
    const int numSeeds;
    const std::unique_ptr<Pit[]> pits{}; // indexed in the clockwise order.

  public:
    Board(int numHouses, int numSeeds);

    Board(const Board&) = delete;            // due to no copy of unique_ptr
    Board(Board&&) = delete;                 // due to no move of unique_ptr
    Board& operator=(const Board&) = delete; // due to const data members
    Board& operator=(Board&&) = delete;      // due to const data members

    [[nodiscard]] Pit* getPit(Player pl, int pitNum) const;
    [[nodiscard]] Pit* getStore(Player pl) const;
    std::ostream& print(std::ostream& out) const;
    std::ostream& prettyPrint(std::ostream& out) const;

    [[nodiscard]] int getNumHouses() const { return numHouses; }
    [[nodiscard]] bool isAllHousesEmpty(Player pl) const;
    [[nodiscard]] bool isEmpty(Player player, int i) const;
};

inline std::ostream& operator<<(std::ostream& out, const Board& board) {
    return board.print(out);
}
````

Exception.hpp
-------------

Base KalahException derived from std::exception; similar to Fatal(),
std::exception has virtual destructor and virtual "exception::what()" method
that returns a C-style character string describing the general cause of the
current error. This class also classifies other Kalah Exceptions. If needed,
later this methods can be overridden.

````
// -----------------------------------------------------------------------------
class KalahException : std::exception {};

// -----------------------------------------------------------------------------
// Undo Exception to undo a move
class Undo : KalahException {};

// -----------------------------------------------------------------------------
// Redo Exception to redo a move
class Redo : KalahException {};

// -----------------------------------------------------------------------------
// The Save exception object should have a constructor that takes a string
// argument and saves it in a private data member. This will be used to store
// the filename following the ’s’ command.
class Save : KalahException {
  private:
    std::string filename;

  public:
    explicit Save(std::string filename) : filename(std::move(filename)) {}
    [[nodiscard]] const std::string& getFilename() const { return filename; }
};

// -----------------------------------------------------------------------------
// Quit Exception to quit the game
class Quit : KalahException {};
````

I also altered Fatal() exception class in the tools as follows, making it inherited
from std::exception in order to prevent annoying clang-tidy warnings.

````
class Fatal : public std::exception {
  private:
    char msg[256];

  public:
    explicit Fatal(const char* format, ...);
    [[nodiscard]] const char* what() const noexcept override { return msg; }
};
````

And main catches all other exceptions as follows:

````
// -----------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    banner();
    try {
        kalah::run(argc, argv);
    } catch (Fatal& e) {
        std::cout << "\n\nCatching Fatal exception...\n\n" << e.what() << "\n";
        kalah::instructions();
    } catch (std::runtime_error& e) {
        std::cerr << "\n\nCatching Runtime Error...\n\n" << e.what() << "\n";
    } catch (std::bad_alloc& e) { // handle memory exhaustion
        std::cerr << "\n\nCatching Allocation error...\n\n" << e.what() << "\n";
        return 1; // exit main
    } catch (...) {
        std::cout << "Uncaught exception..." << std::endl;
        throw;
    }
    bye();
    return 0;
}
````


Report (PS6)
======

* In this assignment I changed the PS5 codes involved in interacting with the human to obtain the next move. I moved it from the Referee class to a new `HumanAgent` class; I added two information member functions to `Game` class. I refined the human interaction code in the `HumangAgent` class. Please find in the following my remarks about those changes and additions.

* I prepared an extensive test suite that can be run using this command: ` ./kalah --test` ; I implemented a special `Test` class that runs each kalah game as a system command and compare the results with the test data.  

1. Firstly, I organized polymorphic derivation exactly requested by the requirements as follows:
````
class Agent {
  public:
    virtual int chooseMove(Player pl) = 0; // pure virtual choose move function
    virtual ~Agent() = default;
};
````

````
class HumanAgent : public Agent {
  public:
    HumanAgent() = default;
    int chooseMove(Player pl) override;
};
````

````
class MachineAgent : public Agent {
  private:
    const Game& game;

  public:
    explicit MachineAgent(const Game& game) : game(game) {}
    int chooseMove(Player pl) override;
};
````

2. The Referee has additional two `Agent` pointers and corresponding destructor; 

````
class Referee {
  private:
    Game game;    // The Referee creates and has custody of the Game object
    Agent* north; // The Referee has one Agent* for North
    Agent* south; // The Referee has one Agent* for South
    Agent* getAgent(Player pl) { return pl == SOUTH ? south : north; }

  public:
    Referee(int numHouses, int numSeeds, AgentType n, AgentType s);
    ~Referee() {
        delete north;
        delete south;
    }
    void playGame(); // The top-level function
};
````

3. I added also an Enum type to pass as parameters to the Referee constructor instead of using strings.

````
// There are two legal string arguments: human and machine.
enum AgentType { HUMAN, MACHINE };
````

4. The Referee's constructors now dynamically creates Human or Machine players based on the parameters passed. The first parameter is for the South player and the second one is for the North player.

````
Referee::Referee(int numHouses, int numSeeds, AgentType s, AgentType n)
    : game(numHouses, numSeeds) {
    // creates the Player South
    if (s == HUMAN)
        south = new HumanAgent();
    else
        south = new MachineAgent(game);
    // creates the Player North
    if (n == MACHINE)
        north = new MachineAgent(game);
    else
        north = new HumanAgent();
}
````

5. The Referee's `void Referee::playGame() ` method includes the following dynamic dispatch without knowing whether the player is a human or machine:

````
    Agent* pl = getAgent(active);
    move = pl->chooseMove(active);
````


6. In the previous problem set, I had already marked all the getter functions as `const` member functions except for `void Game::doMove(int move)` and `void Game::endGame()` since only those two functions mutate the `Game` state.

````
class Game {
  ...
  public:
    explicit Game(int numHouses, int numSeeds) : board(numHouses, numSeeds){};

    ~Game() = default;

    void doMove(int move);
    void endGame();
    // determines if the user input is a legal move in the current state.
    bool isValidMove(int move) const;
    bool isOver() const;
    // delegate the task to the corresponding isEmpty() function in Board
    bool isEmpty(Player pl, int num) const {
        return board.isEmpty(pl, num);
    }
    Player opponent() const {
        return (player == NORTH) ? SOUTH : NORTH;
    }
    bool playerCaptured() const { return capture; }
    int getCapturedSeed() const { return capturedSeed; }
    int getPlayerStore() const { return playerStore; }
    int getOpponentStore() const { return opponentStore; }
    bool playerHasAnotherTurn() const { return repeat; }
    // Displays the pretty printed version of the board.
    ostream& displayBoard(ostream& out) const { return board.prettyPrint(out); }
};
````

7. However, I added the following two additional `const` member functions `bool Game::isEmpty(Player pl, int num) const` and `int Game::getNumHouses() const` as I needed to get information about the game in the `MachineAgent`; I delegated them to the `Board` class who is expert on those information.

````
    // delegate the task to the corresponding isEmpty() function in Board
    bool isEmpty(Player pl, int num) const { return board.isEmpty(pl, num);}
    // delegate the task to the corresponding getNumHouses() function in Board
    int getNumHouses() const { return board.getNumHouses(); }
````

8. The human agent passed quit game information by returning 0 for the move, which triggers a quit signal in the referee class.

````
int HumanAgent::chooseMove(Player pl) {
    string str;
    int move = 0;
    for (;;) {
        cout << "Please enter a pit number for " << pl << " (q to quit):";
        cin >> str; // user interaction
        cout << "\n";
        if (cin.fail()) { // No data item read
            cout << "Error Reading Input: " << str << endl;
            continue;
        } else if (str == "q") {
            cout << "Quitting game at user's request\n";
            move = 0;
            break;
        }
        stringstream iss(str);
        iss >> move;
        if (iss.fail()) // No data item read
        {
            cout << "Invalid number: " << str << "\n";
            continue;
        } else if (!iss.eof()) // Reading stopped before reaching EOF
        {
            cout << "Trailing characters after number: " << str << "\n";
            continue;
        } else
            break;
    }
    return move;
}
````


Report (PS5)
======
* My main class design is based on the following two requirements:
    1. Game Class: A Game manages the overall play of the game. It creates and
       has custody of the Board object that is used to play the game. It has
       Knowledge of the rules of Kalah game.
    2. Referee Class: The interface between the user and the abstract game. It
       creates, manages, and has custody of a Game object. It displays the board,
       announces whose turn it is, and prompts the user to enter a move.

  To this end, Referee controls the game by using only the Game class and only
  has one constructor and one function:
  ````
  class Referee {
    private:
      Game game; // Referee creates and has custody of the Game object

    public:
      Referee(const int numHouses, const int numSeeds)
          : game(numHouses, numSeeds){};

      ~Referee() = default;

      void playGame(); // The top-level function
  };
  ````
* `Referee::playGame()` function initiates and controls all user interactions.
* The Game Class keeps the state of the game using following private data members:
  ````
  class Game {
    private:
      const Board board;     // Game creates and has custody of the Board object

      Player player = SOUTH; // active player who makes the next move (game state)
      int capturedSeed = 0;  // seeds captured if happens in the turn (game state)
      bool capture = false;  // signals a capture happens (game state)
      bool repeat = false;   // signal the player has another turn (game state)
      int playerStore = 0;   // tracks active player's store seeds (game state)
      int opponentStore = 0; // tracks opponent's store seeds (game state)
      void nextTurn();       // switch players to change the game turn (private)
      void resetGameState(); // resets the game state
    ...
  }
  ````
* It has several functions in order to govern the rule of the game. Those functions are
specified as requirements in the problem set.
  ````
    public:
    ...
        bool isValidMove(int move) const;
        void doMove(int move);
        bool isOver() const;
        void endGame();
    ...
* However, I preferred inserting the `capture()` and `sow()`` operations in
`void doMove(int move)`. Because I had to pass Pit pointers among mothods and
I thought this might clutter the code.
  ````
* In order to provide Referee with read access to the game state, I defined several
functions
  ````
    public:
      explicit Game(int numHouses, int numSeeds) : board(numHouses, numSeeds){};
      ~Game() = default;

      // getter functions related to game state
      Player activePlayer() const { return player; }
      Player opponent() const { return (player == NORTH) ? SOUTH : NORTH; }
      bool playerCaptured() const { return capture; }
      int getCapturedSeed() const { return capturedSeed; }
      int getPlayerStore() const { return playerStore; }
      int getOpponentStore() const { return opponentStore; }
      bool playerHasAnotherTurn() const { return repeat; }
      // Displays the pretty printed version of the board.
      ostream& displayBoard(ostream& out) const { return board.prettyPrint(out); }
  `````
* As an addition, I added an enum to structurally track the types of pits, to check
whether it is House or Store when needed:
  ````
   enum Type { HOUSE, STORE };
  ````
 I may have used inheritance but I didn't need neither slicing nor polymorphism in
 the functions.

* I added the following getters to the Pit class in order to access from the game
 ````
     int getContent() const { return content; }
     Player getOwner() const { return owner; }
     Type getType() const { return type; }
 ````
* The dependency/call direction among classes is as follows:
      * Kalah -> Referee -> Game -> Board, Pit

* For each class, I carefully followed the specification (the data members, 
functions, visibilities (private/public) are implemented and consumed accordingly.

* I used `reference` calls, `initializers` in `constructors` and `const` 
modifiers liberally. 

* I run valgrind on different programs to check memory leaks to increase the
coverage.

* For each class I created a print function and implemented `<<` operator.

* `makefile` generates `kalah` executable without any warnings and errors.