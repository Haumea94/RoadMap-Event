// ============================================================
// SIMULATION DE CIRCULATION ROUTIÈRE AVEC FEUX INTELLIGENTS
// ============================================================

#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <map>

using namespace std;

// -----------------------------------------------------------------
// Énumérations et structures
// -----------------------------------------------------------------

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

// -----------------------------------------------------------------
// Classe Cellule
// -----------------------------------------------------------------

class Cellule {
private:
    TypeCellule type;
    bool occupee;
public:
    Cellule(TypeCellule t = VIDE) : type(t), occupee(false) {}

    TypeCellule getType() const { return type; }
    void setType(TypeCellule t) { type = t; }
    bool estOccupee() const { return occupee; }
    void setOccupee(bool o) { occupee = o; }

    bool estTraversable() const {
        return (type == ROUTE_H || type == ROUTE_V || type == CARREFOUR) && !occupee;
    }
};

// -----------------------------------------------------------------
// Classe Vehicule
// -----------------------------------------------------------------

class Vehicule {
private:
    int id;
    Position position;
    Direction direction;
    char symbole;
    int vitesse;
public:
    Vehicule(int _id, Position pos, Direction dir, char sym = 'V')
        : id(_id), position(pos), direction(dir), symbole(sym), vitesse(1) {}

    int getId() const { return id; }
    Position getPosition() const { return position; }
    void setPosition(const Position& pos) { position = pos; }
    Direction getDirection() const { return direction; }
    void setDirection(Direction dir) { direction = dir; }
    char getSymbole() const { return symbole; }

    Position prochainPosition() const {
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
};

// -----------------------------------------------------------------
// Classe FeuCirculation
// -----------------------------------------------------------------

class FeuCirculation {
private:
    map<Direction, EtatFeu> etats;
    int tempsRestant;
    int dureeVert;
    int dureeRouge;
public:
    FeuCirculation(int duree = 3) : tempsRestant(duree), dureeVert(duree), dureeRouge(duree) {
        etats[NORD] = VERT;
        etats[SUD]  = VERT;
        etats[EST]  = ROUGE;
        etats[OUEST]= ROUGE;
    }

    EtatFeu getEtat(Direction dir) const {
        auto it = etats.find(dir);
        return (it != etats.end()) ? it->second : ROUGE;
    }

    void actualiser() {
        tempsRestant--;
        if (tempsRestant <= 0) {
            changerFeux();
            tempsRestant = dureeVert;
        }
    }

    void changerFeux() {
        for (auto& paire : etats) {
            paire.second = (paire.second == VERT) ? ROUGE : VERT;
        }
    }

    int getTempsRestant() const { return tempsRestant; }
};

// -----------------------------------------------------------------
// Classe SimulationCirculation
// -----------------------------------------------------------------

class SimulationCirculation {
private:
    int largeur, hauteur;
    vector<vector<Cellule>> grille;
    vector<shared_ptr<Vehicule>> vehicules;
    Position posCarrefour;
    FeuCirculation feu;
    int tempsSimulation;

public:
    SimulationCirculation(int l, int h) : largeur(l), hauteur(h), tempsSimulation(0) {
        initialiserGrille();
        posCarrefour = Position(largeur / 2, hauteur / 2);
    }

    void initialiserGrille() {
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

    bool positionValide(const Position& pos) const {
        return pos.x >= 0 && pos.x < largeur && pos.y >= 0 && pos.y < hauteur;
    }

    bool peutAvancer(const Position& pos) const {
        if (!positionValide(pos)) return false;
        return grille[pos.y][pos.x].estTraversable();
    }

    bool peutTraverserCarrefour(shared_ptr<Vehicule> v) const {
        Position next = v->prochainPosition();
        if (!positionValide(next)) return true;
        if (grille[next.y][next.x].getType() != CARREFOUR) return true;
        return feu.getEtat(v->getDirection()) == VERT;
    }

    bool collisionPossible(const Position& pos, int vehiculeId) const {
        for (const auto& v : vehicules) {
            if (v->getId() != vehiculeId && v->getPosition() == pos) {
                return true;
            }
        }
        return false;
    }

    void ajouterVehicule(Position pos, Direction dir, char symbole = 'V') {
        if (positionValide(pos) && !grille[pos.y][pos.x].estOccupee()) {
            int id = vehicules.size();
            auto v = make_shared<Vehicule>(id, pos, dir, symbole);
            vehicules.push_back(v);
            grille[pos.y][pos.x].setOccupee(true);
        }
    }

    void deplacerVehicule(shared_ptr<Vehicule> v) {
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

    void actualiser() {
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

    // Effacement d'écran portable
    void effacerEcran() const {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    void afficher() const {
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
                                cout << " ┼";   // Vert Nord‑Sud
                            else
                                cout << " ╬";   // Rouge Nord‑Sud (vert Est‑Ouest)
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

    void genererVehicules() {
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

    void executer(int iterations = 100) {
        for (int i = 0; i < iterations; i++) {
            genererVehicules();
            actualiser();
            afficher();
            this_thread::sleep_for(chrono::milliseconds(500));
        }
    }
};

// -----------------------------------------------------------------
// Fonction principale
// -----------------------------------------------------------------

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    cout << "=== SIMULATION DE CIRCULATION ROUTIÈRE ===\n";
    cout << "Carrefour avec feux de circulation intelligents\n\n";

    SimulationCirculation sim(20, 15);

    // Ajout de véhicules initiaux pour démarrer
    sim.ajouterVehicule(Position(10, 2), SUD, 'v');
    sim.ajouterVehicule(Position(10, 12), NORD, '^');
    sim.ajouterVehicule(Position(2, 7), EST, '>');
    sim.ajouterVehicule(Position(18, 7), OUEST, '<');

    sim.executer(200);

    cout << "\n=== SIMULATION TERMINÉE ===\n";
    return 0;
}