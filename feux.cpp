#include "FeuCirculation.h"

FeuCirculation::FeuCirculation(int duree) : tempsRestant(duree), dureeVert(duree), dureeRouge(duree) {
    etats[NORD] = VERT;
    etats[SUD]  = VERT;
    etats[EST]  = ROUGE;
    etats[OUEST]= ROUGE;
}

EtatFeu FeuCirculation::getEtat(Direction dir) const {
    auto it = etats.find(dir);
    return (it != etats.end()) ? it->second : ROUGE;
}

void FeuCirculation::actualiser() {
    tempsRestant--;
    if (tempsRestant <= 0) {
        changerFeux();
        tempsRestant = dureeVert;
    }
}

void FeuCirculation::changerFeux() {
    for (auto& paire : etats) {
        paire.second = (paire.second == VERT) ? ROUGE : VERT;
    }
}

int FeuCirculation::getTempsRestant() const { return tempsRestant; }