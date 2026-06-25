/*
    MYSTICAL TEMPLE ADVENTURE  (SIMPLE VERSION)
    --------------------------------------------
    A text based adventure game.
    Written in very basic C++ so it is easy to read and understand:
      - no classes
      - no vectors / sets / maps
      - just variables, if-else, loops and functions

    HOW TO COMPILE:
        g++ -std=c++17 -O2 -o mystical_temple_adventure mystical_temple_adventure.cpp
    HOW TO RUN:
        ./mystical_temple_adventure        (Linux / Mac)
        mystical_temple_adventure.exe      (Windows)
*/

#include <iostream>
#include <string>
#include <cstdlib>   // for rand()
#include <ctime>     // for time()
using namespace std;

// ------------------------------------------------------------
// PLAYER VARIABLES (just plain global variables, very simple)
// ------------------------------------------------------------

string playerName = "Adventurer";
int playerHealth = 100;
int playerMaxHealth = 100;
int playerAttack = 12;
int playerGold = 0;

// Instead of a "vector inventory", we just use simple true/false
// flags for each item. This is easier to read for beginners.
bool hasBronzeKey = false;
bool hasSilverKey = false;
bool hasHealingPotion = false;
bool hasGoldenAmulet = false;
bool hasIdol = false;

bool guardianDefeated = false;
bool libraryDone = false;

bool gameOver = false;   // becomes true if player dies
bool gameWon  = false;   // becomes true if player gets the idol

// ------------------------------------------------------------
// SIMPLE HELPER FUNCTIONS
// ------------------------------------------------------------

void printLine() {
    cout << "------------------------------------------------------------\n";
}

// shows health, gold and items the player is carrying
void showInventory() {
    printLine();
    cout << "Health: " << playerHealth << "/" << playerMaxHealth;
    cout << "   Gold: " << playerGold << "\n";

    cout << "Items: ";
    bool nothing = true;

    if (hasBronzeKey)     { cout << "[Bronze Key] "; nothing = false; }
    if (hasSilverKey)     { cout << "[Silver Key] "; nothing = false; }
    if (hasHealingPotion) { cout << "[Healing Potion] "; nothing = false; }
    if (hasGoldenAmulet)  { cout << "[Golden Amulet] "; nothing = false; }
    if (hasIdol)          { cout << "[Sacred Idol] "; nothing = false; }

    if (nothing) cout << "(empty)";
    cout << "\n";
    printLine();
}

// reads one line of text from the player and makes it lowercase
string getCommand() {
    cout << "> ";
    string input;
    getline(cin, input);

    // turn it into lowercase letter by letter (simple loop, no fancy STL)
    for (int i = 0; i < (int)input.size(); i++) {
        if (input[i] >= 'A' && input[i] <= 'Z') {
            input[i] = input[i] + ('a' - 'A');
        }
    }
    return input;
}

// returns true if "text" contains the word "word" inside it
bool contains(string text, string word) {
    return text.find(word) != string::npos;
}

// ------------------------------------------------------------
// COMBAT FUNCTION
// One simple function handles every fight in the game.
// Returns true if the player wins, false if the player flees or dies.
// ------------------------------------------------------------

bool fightEnemy(string enemyName, int enemyHealth, int enemyAttack) {
    cout << "\nA " << enemyName << " appears!\n";

    while (enemyHealth > 0 && playerHealth > 0) {
        cout << "\n" << enemyName << " HP: " << enemyHealth;
        cout << "   Your HP: " << playerHealth << "\n";
        cout << "Type: fight / item / flee\n";

        string cmd = getCommand();

        if (contains(cmd, "fight")) {
            int damage = playerAttack + (rand() % 5);   // small random bonus
            enemyHealth = enemyHealth - damage;
            cout << "You hit the " << enemyName << " for " << damage << " damage!\n";

            if (enemyHealth > 0) {
                int enemyDamage = enemyAttack + (rand() % 4);
                playerHealth = playerHealth - enemyDamage;
                cout << "The " << enemyName << " hits you for " << enemyDamage << " damage!\n";
            }
        }
        else if (contains(cmd, "item")) {
            if (hasHealingPotion) {
                hasHealingPotion = false;
                playerHealth = playerHealth + 30;
                if (playerHealth > playerMaxHealth) playerHealth = playerMaxHealth;
                cout << "You drink the Healing Potion and gain 30 HP!\n";
            } else {
                cout << "You have no Healing Potion.\n";
            }
        }
        else if (contains(cmd, "flee")) {
            int chance = rand() % 100;
            if (chance < 50) {
                cout << "You escaped from the " << enemyName << "!\n";
                return false;
            } else {
                cout << "You could not escape!\n";
                playerHealth = playerHealth - enemyAttack;
                cout << "The " << enemyName << " hits you for " << enemyAttack << " damage!\n";
            }
        }
        else {
            cout << "Please type: fight, item, or flee\n";
        }

        if (playerHealth <= 0) {
            cout << "\nYou were defeated by the " << enemyName << "...\n";
            gameOver = true;
            return false;
        }
    }

    cout << "\nYou defeated the " << enemyName << "!\n";
    return true;
}

// ------------------------------------------------------------
// ROOM FUNCTIONS
// Each room is just one function with a loop and if-else commands.
// ------------------------------------------------------------

void finalChamber() {
    printLine();
    cout << "=== THE FINAL CHAMBER ===\n";
    cout << "The Sacred Idol glows on a stone altar.\n";
    cout << "A Temple Wraith rises from the shadows to guard it!\n";

    int wraithHealth = 70;
    if (hasGoldenAmulet) {
        cout << "Your Golden Amulet weakens the Wraith!\n";
        wraithHealth = wraithHealth - 20;
    }

    bool won = fightEnemy("Temple Wraith", wraithHealth, 16);
    if (gameOver) return;

    if (won) {
        cout << "\nThe Wraith dissolves into dust.\n";
        cout << "You pick up the Sacred Idol of Lumara!\n";
        hasIdol = true;
        gameWon = true;
    } else {
        cout << "You step back, the idol still out of reach.\n";
    }
}

void innerSanctum() {
    printLine();
    cout << "=== THE INNER SANCTUM ===\n";
    cout << "A glowing gate blocks the way.\n";
    cout << "It says: 'Speak the word of the sun god to pass.'\n";
    cout << "(Hint: the answer is the word RA)\n";

    while (true) {
        cout << "\nType: speak <word>  /  inventory\n";
        string cmd = getCommand();

        if (contains(cmd, "speak")) {
            if (contains(cmd, "ra")) {
                cout << "The gate dissolves in golden light!\n";
                finalChamber();
                return;
            } else {
                cout << "Wrong word. The gate stays shut.\n";
            }
        }
        else if (contains(cmd, "inventory")) {
            showInventory();
        }
        else {
            cout << "Type 'speak ra' to open the gate.\n";
        }
    }
}

void treasureVault() {
    printLine();
    cout << "=== THE TREASURE VAULT ===\n";
    cout << "Gold coins glitter everywhere. A Golden Amulet sits on a stand.\n";

    while (true) {
        cout << "\nType: take gold / take amulet / inventory / continue\n";
        string cmd = getCommand();

        if (contains(cmd, "gold")) {
            int found = 50 + (rand() % 50);
            playerGold = playerGold + found;
            cout << "You collect " << found << " gold coins!\n";
        }
        else if (contains(cmd, "amulet")) {
            if (!hasGoldenAmulet) {
                hasGoldenAmulet = true;
                cout << "You take the Golden Amulet!\n";
            } else {
                cout << "You already have the amulet.\n";
            }
        }
        else if (contains(cmd, "inventory")) {
            showInventory();
        }
        else if (contains(cmd, "continue")) {
            cout << "A staircase leads deeper into the temple...\n";
            innerSanctum();
            return;
        }
        else {
            cout << "Type: take gold, take amulet, or continue\n";
        }
    }
}

void serpentPit() {
    printLine();
    cout << "=== THE SERPENT PIT ===\n";
    cout << "A giant snake blocks the rope bridge!\n";

    bool won = fightEnemy("Venomous Serpent", 40, 14);
    if (gameOver) return;

    if (won) {
        cout << "The serpent flees. You cross the bridge safely.\n";
        treasureVault();
    } else {
        cout << "You step back from the pit.\n";
    }
}

void puzzleRoom() {
    printLine();
    cout << "=== THE CHAMBER OF LOCKS ===\n";
    cout << "A stone door has two keyholes: Bronze and Silver.\n";

    while (true) {
        cout << "\nType: insert keys / inventory / wait\n";
        string cmd = getCommand();

        if (contains(cmd, "insert")) {
            if (hasBronzeKey && hasSilverKey) {
                cout << "Both keys click into place. The door opens!\n";
                hasBronzeKey = false;
                hasSilverKey = false;
                serpentPit();
                return;
            } else {
                cout << "You are missing a key.\n";
                if (!hasBronzeKey) cout << "  - Need the Bronze Key (Guardian Chamber)\n";
                if (!hasSilverKey) cout << "  - Need the Silver Key (Library)\n";
            }
        }
        else if (contains(cmd, "inventory")) {
            showInventory();
        }
        else if (contains(cmd, "wait")) {
            cout << "Nothing happens.\n";
        }
        else {
            cout << "Type: insert keys, inventory, or wait\n";
        }

        if (gameOver) return;
    }
}

void library() {
    printLine();
    cout << "=== THE FORGOTTEN LIBRARY ===\n";
    cout << "Dusty scrolls fill the shelves.\n";

    while (true) {
        cout << "\nType: read scroll / inventory / leave\n";
        string cmd = getCommand();

        if (contains(cmd, "read")) {
            cout << "The scroll holds a riddle about secrets of the past.\n";
            if (!hasSilverKey) {
                hasSilverKey = true;
                cout << "A Silver Key falls out of the scroll!\n";
            }
        }
        else if (contains(cmd, "inventory")) {
            showInventory();
        }
        else if (contains(cmd, "leave")) {
            libraryDone = true;
            cout << "You leave the library.\n";
            return;
        }
        else {
            cout << "Type: read scroll, inventory, or leave\n";
        }
    }
}

void guardianChamber() {
    printLine();
    cout << "=== THE GUARDIAN CHAMBER ===\n";
    cout << "A stone statue comes alive, eyes glowing red!\n";

    bool won = fightEnemy("Stone Guardian", 50, 10);
    if (gameOver) return;

    if (won) {
        guardianDefeated = true;
        if (!hasBronzeKey) {
            hasBronzeKey = true;
            cout << "The statue crumbles, leaving a Bronze Key!\n";
        }
        playerGold = playerGold + 25;
        cout << "You also find 25 gold.\n";
    } else {
        cout << "You step back from the chamber.\n";
    }
}

void entranceHall() {
    bool chestOpened = false;

    printLine();
    cout << "=== THE ENTRANCE HALL ===\n";
    cout << "Two paths lie ahead: LEFT (Guardian Chamber) and RIGHT (Library).\n";
    cout << "There is also a small chest here.\n";

    while (true) {
        cout << "\nType: open chest / go left / go right / inventory\n";
        string cmd = getCommand();

        if (contains(cmd, "open") && contains(cmd, "chest")) {
            if (!chestOpened) {
                chestOpened = true;
                hasHealingPotion = true;
                cout << "You find a Healing Potion inside the chest!\n";
            } else {
                cout << "The chest is already empty.\n";
            }
        }
        else if (contains(cmd, "left")) {
            guardianChamber();
        }
        else if (contains(cmd, "right")) {
            library();
        }
        else if (contains(cmd, "inventory")) {
            showInventory();
        }
        else if (contains(cmd, "quit") || contains(cmd, "exit")) {
            cout << "You leave the temple.\n";
            return;
        }
        else {
            cout << "Type: open chest, go left, go right, or inventory\n";
        }

        if (gameOver) return;

        // once both side-rooms are finished, move to the puzzle room
        if (guardianDefeated && libraryDone) {
            cout << "\nA hidden passage opens deeper into the temple...\n";
            puzzleRoom();
            return;
        }
    }
}

// ------------------------------------------------------------
// INTRO + ENDING
// ------------------------------------------------------------

void showIntro() {
    cout << "===========================================================\n";
    cout << "       M Y S T I C A L   T E M P L E   A D V E N T U R E\n";
    cout << "===========================================================\n\n";

    cout << "Enter your name: ";
    getline(cin, playerName);
    if (playerName == "") playerName = "Adventurer";

    printLine();
    cout << "Welcome, " << playerName << "!\n";
    cout << "Deep in the jungle lies a forgotten temple.\n";
    cout << "Inside rests the Sacred Idol of Lumara.\n";
    cout << "Find it, survive the guardians, and escape!\n";
    printLine();
}

void showEnding() {
    printLine();
    if (gameWon) {
        cout << "You escape the temple with the Sacred Idol!\n";
        cout << "*** VICTORY! Final Gold: " << playerGold << " ***\n";
    }
    else if (gameOver) {
        cout << "Your journey ends here, deep within the temple...\n";
        cout << "*** GAME OVER ***\n";
    }
    else {
        cout << "You leave the temple, the idol still hidden somewhere inside.\n";
    }
    printLine();
    cout << "Thanks for playing!\n";
}

// ------------------------------------------------------------
// MAIN FUNCTION  - this is where the program starts
// ------------------------------------------------------------

int main() {
    srand((unsigned int)time(0));   // makes random numbers different each run

    showIntro();
    entranceHall();
    showEnding();

    return 0;
}
