# 🚦 Simulation de circulation routière – Carrefour à feux intelligents

Simulation interactive en **C++17** d’un carrefour à quatre directions (Nord, Sud, Est, Ouest).  
Le projet illustre la gestion du trafic grâce à des **feux de circulation alternés**, l’apparition aléatoire de véhicules et la **prévention des collisions** dans une grille en console.

![License MIT](https://img.shields.io/badge/License-MIT-blue.svg)
![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)

---

## ✨ Fonctionnalités principales

- **Grille routière 2D** : routes horizontales, verticales et un carrefour central.
- **Feux de circulation dynamiques** : alternance automatique Nord/Sud ↔ Est/Ouest toutes les 3 secondes (configurable).
- **Génération aléatoire de véhicules** : apparition sur les 4 bords de la carte avec des symboles directionnels (`^`, `v`, `<`, `>`).
- **Gestion des collisions** : un véhicule ne peut pas avancer si la case devant lui est occupée.
- **Priorité au carrefour** : seuls les véhicules ayant le feu vert peuvent traverser.
- **Affichage temps réel** : mise à jour toutes les 500 ms avec état des feux, nombre de véhicules et temps de simulation.
- **Compatibilité Windows / Linux / macOS** : effacement d’écran adaptatif (`cls` / `clear`).

---

## 🧠 Architecture technique

Le projet est développé en **C++17** et repose sur les concepts suivants :

- **Programmation orientée objet** : classes `Cellule`, `Vehicule`, `FeuCirculation`, `SimulationCirculation`.
- **Gestion mémoire** : utilisation de `shared_ptr` pour les véhicules.
- **Multithreading léger** : `std::this_thread::sleep_for` pour le rafraîchissement.
- **Conteneurs STL** : `vector`, `map` pour la grille et les états des feux.
- **Code modulaire** : bien que livré en un seul fichier pour plus de simplicité, il peut être facilement découpé en `.h` et `.cpp`.

---

## 🖥️ Aperçu de l’affichage
