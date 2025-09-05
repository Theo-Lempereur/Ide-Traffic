# Configuration de l'environnement de développement

## Prérequis

Pour compiler ce projet, vous devez installer l'un des environnements suivants :

### Option 1 : Visual Studio (Recommandé)

1. Téléchargez et installez [Visual Studio Community 2022](https://visualstudio.microsoft.com/fr/vs/community/)
2. Lors de l'installation, sélectionnez :
   - **Développement Desktop en C++**
   - **CMake tools for C++**
   - **MSVC v143 - VS 2022 C++ x64/x86 build tools**

### Option 2 : CMake + Visual Studio Build Tools

1. Téléchargez et installez [CMake](https://cmake.org/download/)
2. Téléchargez et installez [Visual Studio Build Tools 2022](https://visualstudio.microsoft.com/fr/downloads/#build-tools-for-visual-studio-2022)
3. Ajoutez CMake au PATH système

### Option 3 : MinGW-w64

1. Téléchargez et installez [MSYS2](https://www.msys2.org/)
2. Ouvrez le terminal MSYS2 et exécutez :
   ```bash
   pacman -S mingw-w64-x86_64-gcc
   pacman -S mingw-w64-x86_64-cmake
   pacman -S mingw-w64-x86_64-make
   ```
3. Ajoutez `C:\msys64\mingw64\bin` au PATH système

## Dépendances

### SFML (Simple and Fast Multimedia Library)

Le projet utilise SFML pour le rendu graphique. Vous pouvez :

#### Option A : Installation via vcpkg (Recommandé)
```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg install sfml:x64-windows
```

#### Option B : Téléchargement manuel
1. Téléchargez [SFML 2.6.x](https://www.sfml-dev.org/download.php) pour Visual Studio
2. Extrayez dans `C:\SFML`
3. Ajoutez `C:\SFML\bin` au PATH système

## Compilation

### Avec CMake (Recommandé)
```bash
# Configuration
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug

# Compilation
cmake --build build --config Debug

# Exécution
.\build\Debug\RoadSim.exe
```

### Avec Visual Studio
1. Ouvrez le dossier du projet dans Visual Studio
2. Visual Studio détectera automatiquement CMakeLists.txt
3. Sélectionnez la configuration Debug/Release
4. Appuyez sur F5 pour compiler et exécuter

### Compilation manuelle (si CMake n'est pas disponible)
```bash
# Avec MSVC (après avoir exécuté vcvars64.bat)
cl /EHsc /std:c++20 /I. /Iapp main.cpp app\**\*.cpp /Fe:RoadSim.exe

# Avec MinGW
g++ -std=c++20 -I. -Iapp main.cpp app/**/*.cpp -o RoadSim.exe
```

## Structure du projet

```
Ide-Traffic/
├── app/
│   ├── core/          # Classes de simulation (Simulator, Scheduler, RNG)
│   ├── editor/        # Éditeurs (MapEditor, EntityEditor)
│   ├── render/        # Rendu graphique (Renderer, Window)
│   ├── io/           # Entrées/Sorties (JsonLoader, ConfigLoader)
│   └── runtime/      # Gestion d'application (Application, ThreadManager)
├── tests/            # Tests unitaires
├── main.cpp          # Point d'entrée
├── CMakeLists.txt    # Configuration CMake
└── build.bat         # Script de test de compilation
```

## Dépannage

### Erreur "cmake n'est pas reconnu"
- Vérifiez que CMake est installé et ajouté au PATH
- Redémarrez votre terminal après l'installation

### Erreur de compilation SFML
- Vérifiez que SFML est correctement installé
- Assurez-vous que les DLL SFML sont dans le PATH ou copiées près de l'exécutable

### Erreurs de compilation C++20
- Vérifiez que votre compilateur supporte C++20
- Pour MSVC : Visual Studio 2019 16.11+ ou 2022
- Pour GCC : version 10+
- Pour Clang : version 12+

## État actuel du projet

✅ **Architecture créée** - Toutes les classes principales sont définies
✅ **Headers complets** - Interfaces publiques définies avec PIMPL
✅ **Implémentations de base** - Méthodes essentielles implémentées
⚠️ **Compilation** - Nécessite l'installation d'un environnement de développement
🔄 **À venir** - Logique métier détaillée, intégration SFML, tests

Le projet est prêt pour la compilation une fois l'environnement configuré !