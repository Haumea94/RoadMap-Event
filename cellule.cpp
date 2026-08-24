#include "Cellule.h"

Cellule::Cellule(TypeCellule t) : type(t), occupee(false) {}

TypeCellule Cellule::getType() const { return type; }
void Cellule::setType(TypeCellule t) { type = t; }
bool Cellule::estOccupee() const { return occupee; }
void Cellule::setOccupee(bool o) { occupee = o; }
bool Cellule::estTraversable() const {
    return (type == ROUTE_H || type == ROUTE_V || type == CARREFOUR) && !occupee;
}