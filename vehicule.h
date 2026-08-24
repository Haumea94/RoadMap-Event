#ifndef VEHICULE_H
#define VEHICULE_H

#include "types.h"

class Vehicule {
private:
    int id;
    Position position;
    Direction direction;
    char symbole;
    int vitesse;
public:
    Vehicule(int _id, Position pos, Direction dir, char sym = 'V');
    int getId() const;
    Position getPosition() const;
    void setPosition(const Position& pos);
    Direction getDirection() const;
    void setDirection(Direction dir);
    char getSymbole() const;
    Position prochainPosition() const;
};

#endif