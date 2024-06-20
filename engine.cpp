#include <sstream>
#include <iomanip>
#include <cassert>
#include <Queue.h>
#include "engine.h"
#include "units.h"
#include "vector.h"
#include "Stack.h"
#include "algorithms.h"
using namespace std;

bool debugMode = true;

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
        Unit* GB = new Unit(false, GM, GN);
        units.add(GB);
        Rmap->setUnit(GM, GN, GB);
    }

    return Rmap;
}


// Main loop for playing the game
// void play(Field& field, istream& is, ostream& os, Vector<Unit*>& units)
// {
//     int numTurns = 1;
//     while (is)
//     {
//         os << "Turn " << numTurns << " begins:" << endl;
//         // Print the new map
//         os << field << endl;
// //        cout << units << endl;
//         // winner
//         if (units.isEmpty()) {
//             os << "A Draw!" << endl;
//             return;
//         }
//         else if (!units[0]->getSide()) {
//             os << "You Lose!" << endl;
//             return;
//         }
//         else if (units[units.size() - 1]->getSide()) {
//             os << "You Win!" << endl;
//             return;
//         }

//         // unit moves
//         // Fill in your code here
//         int i = 0;
//         while (i < units.size() && units[i] != nullptr) {
//             // magician moves
//             if (units[i]->getSide()) {
//                 os << "Please move the unit at (" << units[i]->getRow() << "," << units[i]->getCol() << ")" << endl;
// //                char M[3];
// //                is >> M[0] >> M[1] >> M[2];
//                 string line;
//                 getline(is, line);
//                 istringstream iss(line);
//                 char M;
//                 int dead, j = 0, flag = 1;
//                 while (j < 3 && flag == 1 && line != "") {
//                     iss >> M;
//                     switch(M) {
//                     case 'A':
//                         field.moveUnit(units[i]->getRow(), units[i]->getCol(), units[i]->getRow(), units[i]->getCol() - 1);
//                         j++;
//                         break;
//                     case 'S':
//                         field.moveUnit(units[i]->getRow(), units[i]->getCol(), units[i]->getRow() + 1, units[i]->getCol());
//                         j++;
//                         break;
//                     case 'D':
//                         field.moveUnit(units[i]->getRow(), units[i]->getCol(), units[i]->getRow(), units[i]->getCol() + 1);
//                         j++;
//                         break;
//                     case 'W':
//                         field.moveUnit(units[i]->getRow(), units[i]->getCol(), units[i]->getRow() - 1, units[i]->getCol());
//                         j++;
//                         break;
//                     // magician use magic
//                     case 'X':
//                         char MB;
//                         iss >> MB;
//                         dead = fireBall(field, units, MB, units[i]->getRow(), units[i]->getCol());
//                         // dead <= i guarantees that the dead must be a magician
//                         if (dead != -1 && dead <= i) {
//                             i--;
//                         }
//                         flag = 0;
//                         break;
//                     case 'Y':
//                         dead = tornado(field, units, units[i]->getRow(), units[i]->getCol(), i);
//                         i -= dead;
//                         flag = 0;
//                         break;
//                     }

//                     if (j == 3) {
//                         iss >> M;
//                         switch(M) {
//                         case 'X':
//                             char MB;
//                             iss >> MB;
//                             dead = fireBall(field, units, MB, units[i]->getRow(), units[i]->getCol());
//                             // dead <= i guarantees that the dead must be a magician
//                             if (dead != -1 && dead <= i) {
//                                 i--;
//                             }
//                             flag = 0;
//                             break;
//                         case 'Y':
//                             dead = tornado(field, units, units[i]->getRow(), units[i]->getCol(), i);
//                             i -= dead;
//                             flag = 0;
//                             break;
//                         }
//                     }
//                 }
//                 os << field << endl;
//             }
//             else {
//                 break;
//             }
//         i++;
//         }

//         // Preventing loop in empty program
// //        string line;
// //        if (!getline(is,line)) break;

//         numTurns++;
//     }
// }


