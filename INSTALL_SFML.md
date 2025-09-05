# Installation SFML pour RoadSim

## État actuel
✅ **Composants principaux compilés avec succès** (core, editor, io, runtime sans dépendances SFML)

❌ **Composants graphiques en attente** (Window, Renderer, Application, main.cpp) - nécessitent SFML

## Options d'installation SFML

### Option 1: vcpkg (Recommandée)
```powershell
# Installer vcpkg si pas déjà fait
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Installer SFML
.\vcpkg install sfml:x64-windows

# Intégrer avec Visual Studio
.\vcpkg integrate install
```

### Option 2: Téléchargement manuel
1. Télécharger SFML 2.6.x depuis https://www.sfml-dev.org/download.php
2. Choisir "Visual C++ 15 (2017) - 64-bit" ou version compatible
3. Extraire dans `C:\SFML\` ou dossier de votre choix
4. Modifier `compile_project.bat` pour pointer vers le bon chemin

### Option 3: CMake avec vcpkg
```powershell
# Après installation vcpkg
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[chemin_vcpkg]\scripts\buildsystems\vcpkg.cmake
cmake --build build --config Release
```

## Test après installation

Après installation SFML, tester avec:
```powershell
.\compile_project.bat
```

Ou pour CMake:
```powershell
cmake -B build -S .
cmake --build build
```

## Fichiers concernés par SFML
- `app/main.cpp` - Point d'entrée avec fenêtre SFML
- `app/render/Window.h/.cpp` - Gestion fenêtre
- `app/render/Renderer.h/.cpp` - Rendu graphique
- `app/runtime/Application.h/.cpp` - Boucle principale

## Prochaines étapes
1. Installer SFML (option 1 recommandée)
2. Compiler le projet complet
3. Lancer l'exécutable pour voir la fenêtre vide
4. Commencer le développement des fonctionnalités