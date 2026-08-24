#include "Vehicule.h"

Vehicule::Vehicule(int _id, Position pos, Direction dir, char sym)
    : id(_id), position(pos), direction(dir), symbole(sym), vitesse(1) {}

int Vehicule::getId() const { return id; }
Position Vehicule::getPosition() const { return position; }
void Vehicule::setPosition(const Position& pos) { position = pos; }
Direction Vehicule::getDirection() const { return direction; }
void Vehicule::setDirection(Direction dir) { direction = dir; }
char Vehicule::getSymbole() const { return symbole; }

Position Vehicule::prochainPosition() const {
    Position next = position;
    switch(direction) {
        case NORD: next.y--; break;
        case SUD:  next.y++; break;
        case EST:  next.x++; break;
        case OUEST: next.x--; break;
        default: break;
    }
    return next;
}