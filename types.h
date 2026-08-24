#ifndef TYPES_H
#define TYPES_H

enum Direction { NORD, SUD, EST, OUEST, AUCUNE };
enum TypeCellule { VIDE, ROUTE_H, ROUTE_V, CARREFOUR, OBSTACLE };
enum EtatFeu { ROUGE, VERT };

struct Position {
    int x, y;
    Position(int _x = 0, int _y = 0) : x(_x), y(_y) {}
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

#endif