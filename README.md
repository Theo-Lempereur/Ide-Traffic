# RoadSim — Mini-IDE de simulation de trafic routier

> Créez votre carte, configurez vos flux (voitures, bus, vélos, piétons), puis cliquez **Démarrer** pour observer la simulation.  
> **Pas de gameplay** : uniquement édition + visualisation.  
> **Threads gérés automatiquement** (simulation, rendu, tâches parallèles).

**TL;DR**
- Un éditeur de carte (nœuds, routes, voies, feux, limitations, spawn rules).
- Un moteur de simulation déterministe à pas de temps fixe.
- Un bouton **Démarrer/Pause/Arrêt** pour lancer/contrôler le trafic.
- Mesures intégrées (débit, temps d’attente, occupation des voies), exportables en JSON/CSV.

---

## 1) Objectifs & périmètre

**But :** fournir une application autonome permettant de **concevoir** un réseau routier simplifié, **configurer** des scénarios de trafic et **observer** leur déroulement, sans prise en main “jeu”.

**Non-objectifs :**
- Pas de conduite manuelle ni d’input de joueur.
- Pas de simulation micro-détaillée (moteur physique complet). On vise la **lisibilité**, la **reproductibilité** et l’**éducation**.

**Origine pédagogique :** ce projet modernise un travail scolaire autour de la modélisation d’usagers (piétons, cyclistes, véhicules légers, bus) avec feux synchronisés et flux entrants/sortants, représentation graphique et contraintes de synchronisation. :contentReference[oaicite:0]{index=0}

---

## 2) Architecture (vue d’ensemble)
app/
├─ core/ # Moteur de simulation (modèle, scheduler, RNG)
├─ editor/ # Mini-IDE (UI) : cartes, entités, inspections
├─ render/ # Rendu 2D (SFML/ImGui-SFML ou équivalent)
├─ io/ # Lecture/écriture JSON/TOML (cartes, scénarios, résultats)
├─ runtime/ # Gestion auto des threads, boucle principale, timers
└─ tests/ # Tests unitaires & scenarii de non-régression


**Modules clés**
- **Editor** : dessin de routes/voies, création d’intersections, placement de feux, zones de spawn, profils d’usagers.
- **Simulator** : pas de temps fixe, modèles cinématiques simples (accélération/décélération constantes, respect des feux/distances de sécurité).
- **Renderer** : 2D temps réel (positions, états des feux, heatmaps optionnelles).
- **Thread Manager** : boucle de simulation dédiée + thread de rendu + **pool** pour tâches parallélisées.
- **I/O** : formats stables (`map.json`, `scenario.json`, `sim.config.toml`, `metrics.csv`).

---

## 3) Choix techniques (recommandés)

- **Langage** : C++20
- **Build** : CMake ≥ 3.27
- **Rendu** : SFML 2.6.x (+ **ImGui-SFML** pour l’UI)
- **Sérialisation** : `nlohmann/json` (JSON) et/ou `toml++` (TOML)
- **Tests** : Catch2 / GoogleTest
- **OS** : Windows / Linux / macOS (cross-platform)

> Ces choix sont indicatifs et peuvent être adaptés si votre environnement impose d’autres bibliothèques.

---

## 4) Démarrage rapide

### Prérequis
- CMake, un compilateur C++20, SFML et (optionnel) ImGui-SFML.



