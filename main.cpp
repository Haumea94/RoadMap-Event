#include <iostream>
#include <cstdlib>
#include <ctime>
#include "SimulationCirculation.h"

using namespace std;

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    cout << "=== SIMULATION DE CIRCULATION ROUTIÈRE ===\n";
    cout << "Carrefour avec feux de circulation intelligents\n\n";

    SimulationCirculation sim(20, 15);

    sim.ajouterVehicule(Position(10, 2), SUD, 'v');
    sim.ajouterVehicule(Position(10, 12), NORD, '^');
    sim.ajouterVehicule(Position(2, 7), EST, '>');
    sim.ajouterVehicule(Position(18, 7), OUEST, '<');

    sim.executer(200);

    cout << "\n=== SIMULATION TERMINÉE ===\n";
    return 0;
}