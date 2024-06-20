#include "units.h"
#include "field.h"
#include <cassert>
using namespace std;

/* Unit */

// Constructor
// magician
Unit::Unit(bool sd, int r, int c)
    : side(sd), row(r), col(c)
{}

// goblin
Unit::Unit(bool sd, int r, int c, bool a)
    : side(sd), row(r), col(c), abl(a)
{}

// Get the side
bool Unit::getSide() const
{
    return side;
}

// Get and set the row[col] of a unit
int Unit::getRow() const
{
    return row;
}

int Unit::getCol() const
{
    return col;
}

void Unit::setRow(int r)
{
    row = r;
}

void Unit::setCol(int c)
{
    col = c;
}

// Get and set dir of the unit if the goblin is patrolling
char Unit::getDir() const
{
    return dir;
}

void Unit::setDir(char d)
{
    dir = d;
}

// Get and set the ability of the unit
bool Unit::getAbl() const
{
    return abl;
}

void Unit::setAbl(bool a)
{
    abl = a;
}

// Get and set the action point of a goblin
int Unit::getPoint() const
{
    return pt;
}

void Unit::setPoint(int p)
{
    pt = p;
}

// Destroy the unit
Unit::~Unit()
{
    // Implement it if you want to delete units not in a field.
}

// Get the symbol of the unit
string getUnitSymbol(const Unit& u)
{
    bool side = u.getSide();
    if (side) {
        return "@@";
    }
    else{
        bool abl = u.getAbl();
        if (abl) {
            return "TG";
        }
        else {
            return "PG";
        }
    }
    assert (false);
}


