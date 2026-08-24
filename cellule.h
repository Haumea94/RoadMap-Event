#ifndef CELLULE_H
#define CELLULE_H

#include "types.h"

class Cellule {
private:
    TypeCellule type;
    bool occupee;
public:
    Cellule(TypeCellule t = VIDE);
    TypeCellule getType() const;
    void setType(TypeCellule t);
    bool estOccupee() const;
    void setOccupee(bool o);
    bool estTraversable() const;
};

#endif