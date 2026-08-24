#include "SimulationCirculation.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>

using namespace std;

SimulationCirculation::SimulationCirculation(int l, int h) 
    : largeur(l), hauteur(h), tempsSimulation(0) {
    initialiserGrille();
    posCarrefour = Position(largeur / 2, hauteur / 2);
}

void SimulationCirculation::initialiserGrille() {
    grille.resize(hauteur, vector<Cellule>(largeur));
    int milieu_h = hauteur / 2;
    int milieu_v = largeur / 2;

    for (int x = 0; x < largeur; x++) {
        grille[milieu_h][x].setType(ROUTE_H);
    }
    for (int y = 0; y < hauteur; y++) {
        grille[y][milieu_v].setType(ROUTE_V);
    }
    grille[milieu_h][milieu_v].setType(CARREFOUR);
}

bool SimulationCirculation::positionValide(const Position& pos) const {
    return pos.x >= 0 && pos.x < largeur && pos.y >= 0 && pos.y < hauteur;
}

bool SimulationCirculation::peutAvancer(const Position& pos) const {
    if (!positionValide(pos)) return false;
    return grille[pos.y][pos.x].estTraversable();
}

bool SimulationCirculation::peutTraverserCarrefour(shared_ptr<Vehicule> v) const {
    Position next = v->prochainPosition();
    if (!positionValide(next)) return true;
    if (grille[next.y][next.x].getType() != CARREFOUR) return true;
    return feu.getEtat(v->getDirection()) == VERT;
}

bool SimulationCirculation::collisionPossible(const Position& pos, int vehiculeId) const {
    for (const auto& v : vehicules) {
        if (v->getId() != vehiculeId && v->getPosition() == pos) {
            return true;
        }
    }
    return false;
}

void SimulationCirculation::ajouterVehicule(Position pos, Direction dir, char symbole) {
    if (positionValide(pos) && !grille[pos.y][pos.x].estOccupee()) {
        int id = vehicules.size();
        auto v = make_shared<Vehicule>(id, pos, dir, symbole);
        vehicules.push_back(v);
        grille[pos.y][pos.x].setOccupee(true);
    }
}

void SimulationCirculation::deplacerVehicule(shared_ptr<Vehicule> v) {
    Position actuelle = v->getPosition();
    Position next = v->prochainPosition();

    if (!positionValide(next)) {
        grille[actuelle.y][actuelle.x].setOccupee(false);
        return;
    }

    if (!peutAvancer(next)) return;
    if (!peutTraverserCarrefour(v)) return;
    if (collisionPossible(next, v->getId())) return;

    grille[actuelle.y][actuelle.x].setOccupee(false);
    v->setPosition(next);
    grille[next.y][next.x].setOccupee(true);
}

void SimulationCirculation::actualiser() {
    tempsSimulation++;
    feu.actualiser();

    for (auto it = vehicules.begin(); it != vehicules.end(); ) {
        auto v = *it;
        Position avant = v->getPosition();
        deplacerVehicule(v);

        if (!positionValide(v->getPosition())) {
            grille[avant.y][avant.x].setOccupee(false);
            it = vehicules.erase(it);
        } else {
            ++it;
        }
    }
}

void SimulationCirculation::effacerEcran() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void SimulationCirculation::afficher() const {
    effacerEcran();

    cout << "\n╔";
    for (int x = 0; x < largeur; x++) cout << "══";
    cout << "╗\n";

    for (int y = 0; y < hauteur; y++) {
        cout << "║";
        for (int x = 0; x < largeur; x++) {
            bool vehiculeTrouve = false;
            for (const auto& v : vehicules) {
                if (v->getPosition().x == x && v->getPosition().y == y) {
                    cout << " " << v->getSymbole();
                    vehiculeTrouve = true;
                    break;
                }
            }

            if (!vehiculeTrouve) {
                TypeCellule type = grille[y][x].getType();
                switch(type) {
                    case ROUTE_H: cout << "──"; break;
                    case ROUTE_V: cout << " │"; break;
                    case CARREFOUR:
                        if (feu.getEtat(NORD) == VERT)
                            cout << " ┼";
                        else
                            cout << " ╬";
                        break;
                    case OBSTACLE: cout << " █"; break;
                    default: cout << "  "; break;
                }
            }
        }
        cout << "║\n";
    }

    cout << "╚";
    for (int x = 0; x < largeur; x++) cout << "══";
    cout << "╝\n";

    cout << "\nTemps: " << tempsSimulation << " | Véhicules: " << vehicules.size();
    cout << " | Feu change dans: " << feu.getTempsRestant() << " tours\n";
    cout << "État feu - Nord/Sud: " << (feu.getEtat(NORD) == VERT ? "VERT" : "ROUGE");
    cout << " | Est/Ouest: " << (feu.getEtat(EST) == VERT ? "VERT" : "ROUGE") << "\n";
}

void SimulationCirculation::genererVehicules() {
    if (rand() % 100 < 30) {
        int bord = rand() % 4;
        Position pos;
        Direction dir;
        char symbole;

        switch(bord) {
            case 0: // Haut
                pos = Position(largeur / 2, 0);
                dir = SUD;
                symbole = 'v';
                break;
            case 1: // Bas
                pos = Position(largeur / 2, hauteur - 1);
                dir = NORD;
                symbole = '^';
                break;
            case 2: // Gauche
                pos = Position(0, hauteur / 2);
                dir = EST;
                symbole = '>';
                break;
            case 3: // Droite
                pos = Position(largeur - 1, hauteur / 2);
                dir = OUEST;
                symbole = '<';
                break;
        }
        ajouterVehicule(pos, dir, symbole);
    }
}

void SimulationCirculation::executer(int iterations) {
    for (int i = 0; i < iterations; i++) {
        genererVehicules();
        actualiser();
        afficher();
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}