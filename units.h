#ifndef UNITS_H_INCLUDED
#define UNITS_H_INCLUDED

#include <memory>
#include <string>
#include "field.h"


//enum GoblinType {Patrol, Tracking};


/* Class for units */
class Unit {
public:

    // Constructor
    Unit(bool sd, int r, int c);
    Unit(bool sd, int r, int c, bool a);

    // Check which side the unit belongs to
    bool getSide() const;


    // Get and set row(col) of the unit
    int getRow() const;
    void setRow(int row);

    int getCol() const;
    void setCol(int col);

    // Get and set dir of the unit if the goblin is patrolling
    char getDir() const;
    void setDir(char dir);

    // Get and set the ability of the unit: true if tracing and false if patrolling
    bool getAbl() const;
    void setAbl(bool abl);

    // Get and set the action point of a goblin
    int getPoint() const;
    void setPoint(int pt);

    // Destructor
    ~Unit();

private:
    // Position in the field
    int row;
    int col;
    bool side;
    bool abl;
    int pt;
    char dir;

    // add other properties for future tasks

};

std::string getUnitSymbol(const Unit& u);
#endif // UNITS_H_INCLUDED
