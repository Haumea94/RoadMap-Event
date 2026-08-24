#ifndef FEUCIRCULATION_H
#define FEUCIRCULATION_H

#include "types.h"
#include <map>

class FeuCirculation {
private:
    std::map<Direction, EtatFeu> etats;
    int tempsRestant;
    int dureeVert;
    int dureeRouge;
public:
    FeuCirculation(int duree = 3);
    EtatFeu getEtat(Direction dir) const;
    void actualiser();
    void changerFeux();
    int getTempsRestant() const;
};

#endif