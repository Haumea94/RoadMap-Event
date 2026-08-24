#ifndef SIMULATIONCIRCULATION_H
#define SIMULATIONCIRCULATION_H

#include "types.h"
#include "Cellule.h"
#include "Vehicule.h"
#include "FeuCirculation.h"
#include <vector>
#include <memory>

class SimulationCirculation {
private:
    int largeur, hauteur;
    std::vector<std::vector<Cellule>> grille;
    std::vector<std::shared_ptr<Vehicule>> vehicules;
    Position posCarrefour;
    FeuCirculation feu;
    int tempsSimulation;

    void initialiserGrille();
    void effacerEcran() const;

public:
    SimulationCirculation(int l, int h);
    bool positionValide(const Position& pos) const;
    bool peutAvancer(const Position& pos) const;
    bool peutTraverserCarrefour(std::shared_ptr<Vehicule> v) const;
    bool collisionPossible(const Position& pos, int vehiculeId) const;
    void ajouterVehicule(Position pos, Direction dir, char symbole = 'V');
    void deplacerVehicule(std::shared_ptr<Vehicule> v);
    void actualiser();
    void afficher() const;
    void genererVehicules();
    void executer(int iterations = 100);
};

#endif