#include <sstream>
#include <iomanip>
#include <string>
#include <cassert>
#include <Queue.h>
#include "engine.h"
#include "units.h"
#include "vector.h"
#include "Stack.h"
#include "algorithms.h"
using namespace std;

bool debugMode = true;

int fireBall(Field&, Vector<Unit*>&, char, int, int);
int tornado(Field& field, Vector<Unit*>& units, int row, int col, int index);
int goblinPatrol(Field& field, Vector<Unit*>& units, int index);
int goblinTrace(Field& field, Vector<Unit*>& units, int index);

// Load map and units
Field* loadMap(istream& is, Vector<Unit*>& units)
{
    //Fill in your code here
    string line;
    int M, N, NT, NM, NG;
    getline(is, line);
    istringstream iss(line);
    iss >> M >> N >> NT >> NM >> NG;
//    cin >> M >> N >> NT >> NM >> NG;
    Field* Rmap = new Field(M, N); // allocate a piece of heap memory.
    for (int i = 0; i < NT; i++) {
        getline(is, line);
        istringstream iss(line);
        int TM, TN;
        char TT;
        iss >> TM >> TN >> TT;
//        cin >> TM >> TN >> TT;
        if (TM >= M || TN >= N) {
            return NULL;
        }
        if (TT == 'W') {
            Rmap->setTerrain(TM, TN, WATER);
        }
        else {
            Rmap->setTerrain(TM, TN, MOUNTAIN);
        }
    }

    for (int i = 0; i < NM; i++) {
        getline(is, line);
        istringstream iss(line);
        int MM, MN;
        iss >> MM >> MN;
//        cin >> MM >> MN;
        if (MM >= M || MN >= N) {
            return NULL;
        }
        Unit* MG = new Unit(true, MM, MN);
        units.add(MG);
        Rmap->setUnit(MM, MN, MG);
    }

    for (int i = 0; i < NG; i++) {
        getline(is, line);
        istringstream iss(line);
        int GM, GN;
        string GC;
        iss >> GM >> GN >> GC;
//        cin >> GM >> GN >> GC;
        if (GM >= M || GN >= N) {
            return NULL;
        }
        Unit* GB = new Unit(false, GM, GN, GC == "TG");
        if (GC == "TG") {
            int GP;
            iss >> GP;
            GB->setPoint(GP);
        }
        else {
            char GD;
            int GP;
            iss >> GD >> GP;
            GB->setDir(GD);
            GB->setPoint(GP);
        }

        units.add(GB);
        Rmap->setUnit(GM, GN, GB);
    }

    return Rmap;
}


// Main loop for playing the game
void play(Field& field, istream& is, ostream& os, Vector<Unit*>& units)
{
    int numTurns = 1;
    while (is)
    {
        os << "Turn " << numTurns << " begins:" << endl;
        // Print the new map
        os << field << endl;
//        cout << units << endl;
        // winner
        if (units.isEmpty()) {
            os << "A Draw!" << endl;
            return;
        }
        else if (!units[0]->getSide()) {
            os << "You Lose!" << endl;
            return;
        }
        else if (units[units.size() - 1]->getSide()) {
            os << "You Win!" << endl;
            return;
        }

        // unit moves
        // Fill in your code here
        int i = 0;
        while (i < units.size() && units[i] != nullptr) {
            // magician moves
            if (units[i]->getSide()) {
                os << "Please move the unit at (" << units[i]->getRow() << "," << units[i]->getCol() << ")" << endl;
//                char M[3];
//                is >> M[0] >> M[1] >> M[2];
                string line;
                getline(is, line);
                istringstream iss(line);
                int dead, j = 0, flag = 1;
                while (j < 3 && flag == 1 && line != "") {
                    char M = '\0';
                    iss >> M;
                    switch(M) {
                    case 'A':
                        field.moveUnit(units[i]->getRow(), units[i]->getCol(), units[i]->getRow(), units[i]->getCol() - 1);
                        j++;
                        break;
                    case 'S':
                        field.moveUnit(units[i]->getRow(), units[i]->getCol(), units[i]->getRow() + 1, units[i]->getCol());
                        j++;
                        break;
                    case 'D':
                        field.moveUnit(units[i]->getRow(), units[i]->getCol(), units[i]->getRow(), units[i]->getCol() + 1);
                        j++;
                        break;
                    case 'W':
                        field.moveUnit(units[i]->getRow(), units[i]->getCol(), units[i]->getRow() - 1, units[i]->getCol());
                        j++;
                        break;
                    // magician use magic
                    case 'X':
                        char MB;
                        iss >> MB;
                        dead = fireBall(field, units, MB, units[i]->getRow(), units[i]->getCol());
                        // dead <= i guarantees that the dead must be a magician
                        if (dead != -1 && dead <= i) {
                            i--;
                        }
                        flag = 0;
                        break;
                    case 'Y':
                        dead = tornado(field, units, units[i]->getRow(), units[i]->getCol(), i);
                        i -= dead;
                        flag = 0;
                        break;
                    case '\0':
                        flag = 0;
                        break;
                    }

                    if (j == 3) {
                        iss >> M;
                        switch(M) {
                        case 'X':
                            char MB;
                            iss >> MB;
                            dead = fireBall(field, units, MB, units[i]->getRow(), units[i]->getCol());
                            // dead <= i guarantees that the dead must be a magician
                            if (dead != -1 && dead <= i) {
                                i--;
                            }
                            flag = 0;
                            break;
                        case 'Y':
                            dead = tornado(field, units, units[i]->getRow(), units[i]->getCol(), i);
                            i -= dead;
                            flag = 0;
                            break;
                        }
                    }
                }
                os << field << endl;
            }
            else {
                if (!units[i]->getAbl()) {
                    int dead = goblinPatrol(field, units, i);
                    i-=dead;
                }
                else {
                    int dead = goblinTrace(field, units, i);
                    i-=dead;
                }
            }
        i++;
        }

        // Preventing loop in empty program
//        string line;
//        if (!getline(is,line)) break;

        numTurns++;
    }
}


/* fireBall: return the index of the dead; if there is no one dead, return -1 */
int fireBall(Field& field, Vector<Unit*>& units, char dir, int row, int col) {
//    cout << units << endl;
    switch(dir) {
    case 'A':
        for (int j = col - 1; j >= 0; j--) {
            if (field.getTerrain(row, j) == MOUNTAIN) {
                break;
            }
            else if (field.getUnit(row, j) != nullptr) {
                for (int i = 0; i < units.size(); i++) {
                    if (units[i]->getRow() == row && units[i]->getCol() == j) {
                        units.remove(i);
                        field.removeUnit(row, j);
                        return i;
                    }
                }
            }
        }
        break;
    case 'D':
        for (int j = col + 1; j < field.getWidth(); j++) {
            if (field.getTerrain(row, j) == MOUNTAIN) {
                break;
            }
            else if (field.getUnit(row, j) != nullptr) {
                for (int i = 0; i < units.size(); i++) {
                    if (units[i]->getRow() == row && units[i]->getCol() == j) {
                        units.remove(i);
                        field.removeUnit(row, j);
                        return i;
                    }
                }
            }
        }
        break;
    case 'S':
        for (int j = row + 1; j <= field.getHeight(); j++) {
            if (field.getTerrain(j, col) == MOUNTAIN) {
                break;
            }
            else if (field.getUnit(j, col) != nullptr) {
                for (int i = 0; i < units.size(); i++) {
                    if (units[i]->getRow() == j && units[i]->getCol() == col) {
                        units.remove(i);
                        field.removeUnit(j, col);
                        return i;
                    }
                }
            }
        }
        break;
    case 'W':
        for (int j = row - 1; j >= 0; j--) {
            if (field.getTerrain(j, col) == MOUNTAIN) {
                break;
            }
            else if (field.getUnit(j, col) != nullptr) {
                for (int i = 0; i < units.size(); i++) {
                    if (units[i]->getRow() == j && units[i]->getCol() == col) {
                        units.remove(i);
                        field.removeUnit(j, col);
                        return i;
                    }
                }
            }
        }
        break;
    }
    return -1;
}

/* tornado return the dead  */
int tornado(Field& field, Vector<Unit*>& units, int row, int col, int index) {
    int dead = 0; // document the number of dead magician indexes before the magician
    // left
    if (field.inBounds(row, col - 1) && field.getUnit(row, col - 1) != nullptr) {
        int minimal;
        for (int i = col - 1; i >= 0; i--) {
            if (field.getUnit(row, i) == nullptr) {
                minimal = i;
                break;
            }
        }
        if (field.inBounds(row, minimal)) { // check if minimal is in the map
            if (field.getTerrain(row, minimal) == PLAIN) {
                for (int i = minimal + 1; i < col; i++) {
                    field.moveUnit(row, i, row, i - 1);
                    // the pointer in the units needs no alteration
                }
            }
            else if (field.getTerrain(row, minimal) == WATER) {
                // remove the drowned unit from field and units
                for (int i = 0; i < units.size(); i++) {
                    if (units[i]->getRow() == row && units[i]->getCol() == minimal + 1) {
                        if (units[i]->getSide() && index - dead < i) {
                            dead++;
                        }
                        // check units
//                        cout << units[i] << " removed successfully!" << endl;
                        units.remove(i);
                    }
                }
                field.removeUnit(row, minimal + 1);

                for (int i = minimal + 2; i < col; i++) {
                    field.moveUnit(row, i, row, i - 1);
                    // the pointer in the units needs no alteration
                }
            }
        }
    }

    // right
    if (field.inBounds(row, col + 1) && field.getUnit(row, col + 1) != nullptr) {
        int maximal;
        for (int i = col + 1; i < field.getWidth(); i++) {
            if (field.getUnit(row, i) == nullptr) {
                maximal = i;
                break;
            }
        }
//        cout << "maximal: " << maximal << endl; // delete
        if (field.inBounds(row, maximal)) {// check if minimal is in the map
//            cout << "inBounds successfully" << endl; // delete
            if (field.getTerrain(row, maximal) == PLAIN) {
                for (int i = maximal - 1; i > col; i--) {
                    field.moveUnit(row, i, row, i + 1);
//                    cout << "move right successfully" << endl; // delete
                    // the pointer in the units needs no alteration
                }
            }
            else if (field.getTerrain(row, maximal) == WATER) {
                // remove the drowned unit from field and units
                for (int i = 0; i < units.size(); i++) {
                    if (units[i]->getRow() == row && units[i]->getCol() == maximal - 1) {
                        if (units[i]->getSide() && index - dead < i) {
                            dead++;
                        }
                        // check units
//                        cout << units[i] << " removed successfully!" << endl;
                        units.remove(i);
                    }
                }
                field.removeUnit(row, maximal - 1);

                for (int i = maximal - 2; i > col; i--) {
                    field.moveUnit(row, i, row, i + 1);
                    // the pointer in the units needs no alteration
                }
            }
        }
    }

    // up
    if (field.inBounds(row - 1, col) && field.getUnit(row - 1, col) != nullptr) {
        int minimal;
        for (int i = row - 1; i >= 0; i--) {
            if (field.getUnit(i, col) == nullptr) {
                minimal = i;
                break;
            }
        }
        if (field.inBounds(minimal, col)) { // check if minimal is in the map
            if (field.getTerrain(minimal, col) == PLAIN) {
                for (int i = minimal + 1; i < row; i++) {
                    field.moveUnit(i, col, i - 1, col);
                    // the pointer in the units needs no alteration
                }
            }
            else if (field.getTerrain(minimal, col) == WATER) {
                // remove the drowned unit from field and units
                for (int i = 0; i < units.size(); i++) {
                    if (units[i]->getRow() == minimal + 1 && units[i]->getCol() == col) {
                        if (units[i]->getSide() && index - dead < i) {
                            dead++;
                        }
                        // check units
//                        cout << units[i] << " removed successfully!" << endl;
                        units.remove(i);
                    }
                }
                field.removeUnit(minimal + 1, col);

                for (int i = minimal + 2; i < row; i++) {
                    field.moveUnit(i, col, i - 1, col);
                    // the pointer in the units needs no alteration
                }
            }
        }
    }

    // down
    if (field.inBounds(row + 1, col) && field.getUnit(row + 1, col) != nullptr) {
        int maximal;
        for (int i = row + 1; i < field.getHeight(); i++) {
            if (field.getUnit(i, col) == nullptr) {
                maximal = i;
                break;
            }
        }
        if (field.inBounds(maximal, col)) { // check if minimal is in the map
            if (field.getTerrain(maximal, col) == PLAIN) {
                for (int i = maximal - 1; i > row; i--) {
                    field.moveUnit(i, col, i + 1, col);
                    // the pointer in the units needs no alteration
                }
            }
            else if (field.getTerrain(maximal, col) == WATER) {
                // remove the drowned unit from field and units
                for (int i = 0; i < units.size(); i++) {
                    if (units[i]->getRow() == maximal - 1 && units[i]->getCol() == col) {
                        if (units[i]->getSide() && index - dead < i) {
                            dead++;
                        }
                        // check units
//                        cout << units[i] << " removed successfully!" << endl;
                        units.remove(i);
                    }
                }
                field.removeUnit(maximal - 1, col);

                for (int i = maximal - 2; i > row; i--) {
                    field.moveUnit(i, col, i + 1, col);
                    // the pointer in the units needs no alteration
                }
            }
        }
    }

    // left up
    if (field.inBounds(row - 1, col - 1) && field.getUnit(row - 1, col - 1) != nullptr) {
        int maximal;
        for (int i = 1; row - i >= 0 && col - i >= 0; i++) {
            if (field.getUnit(row - i, col - i) == nullptr) {
                maximal = i;
                break;
            }
        }
        if (field.inBounds(row - maximal, col - maximal)) { // check if minimal is in the map
            if (field.getTerrain(row - maximal, col - maximal) == PLAIN) {
                for (int i = maximal - 1; i > 0; i--) {
                    field.moveUnit(row - i, col - i, row - i - 1, col - i - 1);
                    // the pointer in the units needs no alteration
                }
            }
            else if (field.getTerrain(row - maximal, col - maximal) == WATER) {
                // remove the drowned unit from field and units
                for (int i = 0; i < units.size(); i++) {
                    if (units[i]->getRow() == row - maximal + 1 && units[i]->getCol() == col - maximal + 1) {
                        if (units[i]->getSide() && index - dead < i) {
                            dead++;
                        }
                        //check units
//                        cout << units[i] << " removed successfully!" << endl;
                        units.remove(i);
                    }
                }
                field.removeUnit(row - maximal + 1, col - maximal + 1);

                for (int i = maximal - 2; i > 0; i--) {
                    field.moveUnit(row - i, col - i, row - i - 1, col - i - 1);
                    // the pointer in the units needs no alteration
                }
            }
        }
    }

    // left down
    if (field.inBounds(row + 1, col - 1) && field.getUnit(row + 1, col - 1) != nullptr) {
        int maximal;
        for (int i = 1; row + i < field.getHeight() && col - i >= 0; i++) {
            if (field.getUnit(row + i, col - i) == nullptr) {
                maximal = i;
                break;
            }
        }
        if (field.inBounds(row + maximal, col - maximal)) { // check if minimal is in the map
            if (field.getTerrain(row + maximal, col - maximal) == PLAIN) {
                for (int i = maximal - 1; i > 0; i--) {
                    field.moveUnit(row + i, col - i, row + i + 1, col - i - 1);
                    // the pointer in the units needs no alteration
                }
            }
            else if (field.getTerrain(row + maximal, col - maximal) == WATER) {
                // remove the drowned unit from field and units
                for (int i = 0; i < units.size(); i++) {
                    if (units[i]->getRow() == row + maximal - 1 && units[i]->getCol() == col - maximal + 1) {
                        if (units[i]->getSide() && index - dead < i) {
                            dead++;
                        }
                        // check units
//                        cout << units[i] << " removed successfully!" << endl;
                        units.remove(i);
                    }
                }
                field.removeUnit(row + maximal - 1, col - maximal + 1);

                for (int i = maximal - 2; i > 0; i--) {
                    field.moveUnit(row + i, col - i, row + i + 1, col - i - 1);
                    // the pointer in the units needs no alteration
                }
            }
        }
    }

    // right up
    if (field.inBounds(row + 1, col + 1) && field.getUnit(row + 1, col + 1) != nullptr) {
        int maximal;
        for (int i = 1; row + i < field.getHeight() && col + i < field.getWidth(); i++) {
            if (field.getUnit(row + i, col + i) == nullptr) {
                maximal = i;
                break;
            }
        }
        if (field.inBounds(row + maximal, col + maximal)) { // check if minimal is in the map
            if (field.getTerrain(row + maximal, col + maximal) == PLAIN) {
                for (int i = maximal - 1; i > 0; i--) {
                    field.moveUnit(row + i, col + i, row + i + 1, col + i + 1);
                    // the pointer in the units needs no alteration
                }
            }
            else if (field.getTerrain(row + maximal, col + maximal) == WATER) {
                // remove the drowned unit from field and units
                for (int i = 0; i < units.size(); i++) {
                    if (units[i]->getRow() == row + maximal - 1 && units[i]->getCol() == col + maximal - 1) {
                        if (units[i]->getSide() && index - dead < i) {
                            dead++;
                        }
                        // check units
//                        cout << units[i] << " removed successfully!" << endl;
                        units.remove(i);
                    }
                }
                field.removeUnit(row + maximal - 1, col + maximal - 1);

                for (int i = maximal - 2; i > 0; i--) {
                    field.moveUnit(row + i, col + i, row + i + 1, col + i + 1);
                    // the pointer in the units needs no alteration
                }
            }
        }
    }

    // right down
    if (field.inBounds(row - 1, col + 1) && field.getUnit(row - 1, col + 1) != nullptr) {
        int maximal;
        for (int i = 1; row - i >= 0 && col + i < field.getWidth(); i++) {
            if (field.getUnit(row - i, col + i) == nullptr) {
                maximal = i;
                break;
            }
        }
        if (field.inBounds(row - maximal, col + maximal)) { // check if minimal is in the map
            if (field.getTerrain(row - maximal, col + maximal) == PLAIN) {
                for (int i = maximal - 1; i > 0; i--) {
                    field.moveUnit(row - i, col + i, row - i - 1, col + i + 1);
                    // the pointer in the units needs no alteration
                }
            }
            else if (field.getTerrain(row - maximal, col + maximal) == WATER) {
                // remove the drowned unit from field and units
                for (int i = 0; i < units.size(); i++) {
                    if (units[i]->getRow() == row - maximal + 1 && units[i]->getCol() == col + maximal - 1) {
                        if (units[i]->getSide() && index - dead < i) {
                            dead++;
                        }
                        // check units
//                        cout << units[i] << " removed successfully!" << endl;
                        units.remove(i);
                    }
                }
                field.removeUnit(row - maximal + 1, col + maximal - 1);

                for (int i = maximal - 2; i > 0; i--) {
                    field.moveUnit(row - i, col + i, row - i - 1, col + i + 1);
                    // the pointer in the units needs no alteration
                }
            }
        }
    }

//    cout << units << endl; // check the units
    return dead;
}

// return true if goblin kills a magician successfully else false
int goblinPatrol(Field& field, Vector<Unit*>& units, int index) {
    int row = units[index]->getRow(), col = units[index]->getCol(), dir = units[index]->getDir(), pt = units[index]->getPoint();
//    cout << "goblin (" << row << "," << col << ") patrols, with its direction: " << dir << " and ability: " << pt << endl; //delete
    int dead = 0;
    switch(dir) {
    case 'W':
        for (int i = row; i >= 0 && i >= row - pt; i--) {
            // UP
            if (field.inBounds(i - 1, col) && field.getUnit(i - 1, col) != nullptr && field.getUnit(i - 1, col)->getSide()) {
                for (int j = 0; j < units.size(); j++) {
                    if (units[j]->getRow() == i - 1 && units[j]->getCol() == col) {
                        dead++;
                        units.remove(j);
                        field.removeUnit(i - 1, col);
                    }
                }
            }
            // DOWN
            if (field.inBounds(i + 1, col) && field.getUnit(i + 1, col) != nullptr && field.getUnit(i + 1, col)->getSide()) {
                for (int j = 0; j < units.size(); j++) {
                    if (units[j]->getRow() == i + 1 && units[j]->getCol() == col) {
                        dead++;
                        units.remove(j);
                        field.removeUnit(i + 1, col);
                    }
                }
            }
            // LEFT
            if (field.inBounds(i, col - 1) && field.getUnit(i, col - 1) != nullptr && field.getUnit(i, col - 1)->getSide()) {
                for (int j = 0; j < units.size(); j++) {
                    if (units[j]->getRow() == i && units[j]->getCol() == col - 1) {
                        dead++;
                        units.remove(j);
                        field.removeUnit(i, col - 1);
                    }
                }
            }
            // RIGHT
            if (field.inBounds(i, col + 1) && field.getUnit(i, col + 1) != nullptr && field.getUnit(i, col + 1)->getSide()) {
                for (int j = 0; j < units.size(); j++) {
                    if (units[j]->getRow() == i && units[j]->getCol() == col + 1) {
                        dead++;
                        units.remove(j);
                        field.removeUnit(i, col + 1);
                    }
                }
            }
            // check whether there are dead magicians;
            if (dead > 0) {
//                cout << dead << " magicians dead" << endl; //delete
                break;
            }

            // check whether the points are used up
            if (i == row - pt) {
                break;
            }

            // barrier exists
            if (field.inBounds(i - 1, col) && field.getUnit(i - 1, col) != nullptr && !field.getUnit(i - 1, col)->getSide() || \
                field.inBounds(i - 1, col) && field.getTerrain(i - 1, col) != PLAIN || \
                !field.inBounds(i - 1, col)) {
                break;
            }
            field.moveUnit(i, col, i - 1, col);
        }
        units[index - dead]->setDir('S');
        break;
    case 'S':
        for (int i = row; i < field.getHeight() && i <= row + pt; i++) {
            // UP
            if (field.inBounds(i - 1, col) && field.getUnit(i - 1, col) != nullptr && field.getUnit(i - 1, col)->getSide()) {
                for (int j = 0; j < units.size(); j++) {
                    if (units[j]->getRow() == i - 1 && units[j]->getCol() == col) {
                        dead++;
                        units.remove(j);
                        field.removeUnit(i - 1, col);
                    }
                }
            }
            // DOWN
            if (field.inBounds(i + 1, col) && field.getUnit(i + 1, col) != nullptr && field.getUnit(i + 1, col)->getSide()) {
                for (int j = 0; j < units.size(); j++) {
                    if (units[j]->getRow() == i + 1 && units[j]->getCol() == col) {
                        dead++;
                        units.remove(j);
                        field.removeUnit(i + 1, col);
                    }
                }
            }
            // LEFT
            if (field.inBounds(i, col - 1) && field.getUnit(i, col - 1) != nullptr && field.getUnit(i, col - 1)->getSide()) {
                for (int j = 0; j < units.size(); j++) {
                    if (units[j]->getRow() == i && units[j]->getCol() == col - 1) {
                        dead++;
                        units.remove(j);
                        field.removeUnit(i, col - 1);
                    }
                }
            }
            // RIGHT
            if (field.inBounds(i, col + 1) && field.getUnit(i, col + 1) != nullptr && field.getUnit(i, col + 1)->getSide()) {
                for (int j = 0; j < units.size(); j++) {
                    if (units[j]->getRow() == i && units[j]->getCol() == col + 1) {
                        dead++;
                        units.remove(j);
                        field.removeUnit(i, col + 1);
                    }
                }
            }
            // check whether there are dead magicians;
            if (dead > 0) {
//                cout << dead << " magicians dead" << endl; //delete
                break;
            }

            // check whether the points are used up
            if (i == row + pt) {
                break;
            }

            // barrier exists
            if (field.inBounds(i + 1, col) && field.getUnit(i + 1, col) != nullptr && !field.getUnit(i + 1, col)->getSide() || \
                field.inBounds(i + 1, col) && field.getTerrain(i + 1, col) != PLAIN || \
                !field.inBounds(i + 1, col)) {
                break;
            }

            field.moveUnit(i, col, i + 1, col);
        }
        units[index - dead]->setDir('W');
        break;
    case 'A':
        for (int i = col; i >= 0 && i >= col - pt; i--) {
            // UP
            if (field.inBounds(row - 1, i) && field.getUnit(row - 1, i) != nullptr && field.getUnit(row - 1, i)->getSide()) {
                for (int j = 0; j < units.size(); j++) {
                    if (units[j]->getRow() == row - 1 && units[j]->getCol() == i) {
                        dead++;
                        units.remove(j);
                        field.removeUnit(row - 1, i);
                    }
                }
            }
            // DOWN
            if (field.inBounds(row + 1, i) && field.getUnit(row + 1, i) != nullptr && field.getUnit(row + 1, i)->getSide()) {
                for (int j = 0; j < units.size(); j++) {
                    if (units[j]->getRow() == row + 1 && units[j]->getCol() == i) {
                        dead++;
                        units.remove(j);
                        field.removeUnit(row + 1, i);
                    }
                }
            }
            // LEFT
            if (field.inBounds(row, i - 1) && field.getUnit(row, i - 1) != nullptr && field.getUnit(row, i - 1)->getSide()) {
                for (int j = 0; j < units.size(); j++) {
                    if (units[j]->getRow() == row && units[j]->getCol() == i - 1) {
                        dead++;
                        units.remove(j);
                        field.removeUnit(row, i - 1);
                    }
                }
            }
            // RIGHT
            if (field.inBounds(row, i + 1) && field.getUnit(row, i + 1) != nullptr && field.getUnit(row, i + 1)->getSide()) {
                for (int j = 0; j < units.size(); j++) {
                    if (units[j]->getRow() == row && units[j]->getCol() == i + 1) {
                        dead++;
                        units.remove(j);
                        field.removeUnit(row, i + 1);
                    }
                }
            }
            // check whether there are dead magicians;
            if (dead > 0) {
//                cout << dead << " magicians dead" << endl; //delete
                break;
            }

            // check whether the points are used up
            if (i == col - pt) {
                break;
            }

            // barrier exists
            if (field.inBounds(row, i - 1) && field.getUnit(row, i - 1) != nullptr && !field.getUnit(row, i - 1)->getSide() || \
                field.inBounds(row, i - 1) && field.getTerrain(row, i - 1) != PLAIN || \
                !field.inBounds(row, i - 1)) {
                break;
            }

            field.moveUnit(row, i, row, i - 1);
        }
        units[index - dead]->setDir('D');
        break;
    case 'D':
        for (int i = col; i <= field.getWidth() && i <= col + pt; i++) {
            // UP
            if (field.inBounds(row - 1, i) && field.getUnit(row - 1, i) != nullptr && field.getUnit(row - 1, i)->getSide()) {
                for (int j = 0; j < units.size(); j++) {
                    if (units[j]->getRow() == row - 1 && units[j]->getCol() == i) {
                        dead++;
                        units.remove(j);
                        field.removeUnit(row - 1, i);
                    }
                }
            }
            // DOWN
            if (field.inBounds(row + 1, i) && field.getUnit(row + 1, i) != nullptr && field.getUnit(row + 1, i)->getSide()) {
                for (int j = 0; j < units.size(); j++) {
                    if (units[j]->getRow() == row + 1 && units[j]->getCol() == i) {
                        dead++;
                        units.remove(j);
                        field.removeUnit(row + 1, i);
                    }
                }
            }
            // LEFT
            if (field.inBounds(row, i - 1) && field.getUnit(row, i - 1) != nullptr && field.getUnit(row, i - 1)->getSide()) {
                for (int j = 0; j < units.size(); j++) {
                    if (units[j]->getRow() == row && units[j]->getCol() == i - 1) {
                        dead++;
                        units.remove(j);
                        field.removeUnit(row, i - 1);
                    }
                }
            }
            // RIGHT
            if (field.inBounds(row, i + 1) && field.getUnit(row, i + 1) != nullptr && field.getUnit(row, i + 1)->getSide()) {
                for (int j = 0; j < units.size(); j++) {
                    if (units[j]->getRow() == row && units[j]->getCol() == i + 1) {
                        dead++;
                        units.remove(j);
                        field.removeUnit(row, i + 1);
                    }
                }
            }
            // check whether there are dead magicians;
            if (dead > 0) {
//                cout << dead << " magicians dead" << endl; //delete
                break;
            }

            // check whether the points are used up
            if (i == col + pt) {
                break;
            }

            // barrier exists
            if (field.inBounds(row, i + 1) && field.getUnit(row, i + 1) != nullptr && !field.getUnit(row, i + 1)->getSide() || \
                field.inBounds(row, i + 1) && field.getTerrain(row, i + 1) != PLAIN || \
                !field.inBounds(row, i + 1)) {
                break;
            }

            field.moveUnit(row, i, row, i + 1);
        }
        units[index - dead]->setDir('A');
        break;
    }
    return dead;
}

int goblinTrace(Field& field, Vector<Unit*>& units, int index) {
    int dead = 0, srow = units[index]->getRow(), scol = units[index]->getCol(), pt = units[index]->getPoint();
    for (int i = 1; i <= pt; i++) {
        // j is the movement on the row;
        for (int p = -i; p <= i; p++) {
            for (int k = -i; k <= i; k++){
                if (abs(k) + abs(p) == i && field.inBounds(srow + p, scol + k) && field.getUnit(srow + p, scol + k) != nullptr && field.getUnit(srow + p, scol + k)->getSide()) {
                    pair<int, Vector<point>> shortestPath = searchShortestPath(field, srow, scol, srow + p, scol + k);
                    if (shortestPath.first != -1) {
                        for (point x: shortestPath.second) {
                            if (field.getUnit(x.first, x.second) == nullptr) {
                                field.moveUnit(units[index]->getRow(), units[index]->getCol(), x.first, x.second);
                            }
                        }
                        // attack
                        int row = units[index]->getRow(), col = units[index]->getCol();
                        // UP
                        if (field.inBounds(row - 1, col) && field.getUnit(row - 1, col) != nullptr && field.getUnit(row - 1, col)->getSide()) {
                            for (int j = 0; j < units.size(); j++) {
                                if (units[j]->getRow() == row - 1 && units[j]->getCol() == col) {
                                    dead++;
                                    units.remove(j);
                                    field.removeUnit(row - 1, col);
                                }
                            }
                        }
                        // DOWN
                        if (field.inBounds(row + 1, col) && field.getUnit(row + 1, col) != nullptr && field.getUnit(row + 1, col)->getSide()) {
                            for (int j = 0; j < units.size(); j++) {
                                if (units[j]->getRow() == row + 1 && units[j]->getCol() == col) {
                                    dead++;
                                    units.remove(j);
                                    field.removeUnit(row + 1, col);
                                }
                            }
                        }
                        // LEFT
                        if (field.inBounds(row, col - 1) && field.getUnit(row, col - 1) != nullptr && field.getUnit(row, col - 1)->getSide()) {
                            for (int j = 0; j < units.size(); j++) {
                                if (units[j]->getRow() == row && units[j]->getCol() == col - 1) {
                                    dead++;
                                    units.remove(j);
                                    field.removeUnit(row, col - 1);
                                }
                            }
                        }
                        // RIGHT
                        if (field.inBounds(row, col + 1) && field.getUnit(row, col + 1) != nullptr && field.getUnit(row, col + 1)->getSide()) {
                            for (int j = 0; j < units.size(); j++) {
                                if (units[j]->getRow() == row && units[j]->getCol() == col + 1) {
                                    dead++;
                                    units.remove(j);
                                    field.removeUnit(row, col + 1);
                                }
                            }
                        }
                        break;
                    }
                }
                if (dead > 0) {
                    break;
                }
            }
            if (dead > 0) {
                break;
            }
        }
        if (dead > 0) {
            break;
        }
    }
    return dead;
}

