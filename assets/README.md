# Assets - Ressources Visuelles RoadSim

Ce dossier contient tous les assets visuels et multimédias de l'application RoadSim.

## Structure des dossiers

### 📁 `images/`
- **Usage :** Images générales, photos, backgrounds, captures d'écran
- **Formats supportés :** PNG, JPG, JPEG, BMP
- **Exemples :** logos, arrière-plans, images de documentation

### 📁 `videos/`
- **Usage :** Vidéos de démonstration, tutoriels, animations
- **Formats supportés :** MP4, AVI, MOV
- **Exemples :** vidéos de présentation, tutoriels d'utilisation

### 📁 `icons/`
- **Usage :** Icônes de l'interface utilisateur, boutons, indicateurs
- **Formats supportés :** PNG, SVG, ICO
- **Exemples :** icônes de boutons, indicateurs d'état, curseurs

### 📁 `textures/`
- **Usage :** Textures pour les objets 3D, surfaces, matériaux
- **Formats supportés :** PNG, JPG, TGA
- **Exemples :** textures de route, bâtiments, véhicules

### 📁 `sprites/`
- **Usage :** Sprites 2D, animations, éléments de jeu
- **Formats supportés :** PNG, GIF
- **Exemples :** sprites de véhicules, animations de trafic

### 📁 `ui/`
- **Usage :** Éléments d'interface utilisateur, panneaux, menus
- **Formats supportés :** PNG, SVG
- **Exemples :** panneaux de contrôle, menus, overlays

### 📁 `fonts/`
- **Usage :** Polices de caractères personnalisées
- **Formats supportés :** TTF, OTF, WOFF
- **Exemples :** polices pour l'UI, textes de simulation

## Conventions de nommage

- Utilisez des noms descriptifs en anglais
- Séparez les mots par des underscores : `traffic_light_red.png`
- Incluez la résolution pour les images : `logo_1920x1080.png`
- Utilisez des préfixes pour grouper : `ui_button_start.png`, `icon_play.svg`

## Intégration dans le code

Pour utiliser ces assets dans l'application :

```cpp
// Exemple de chargement d'une texture
sf::Texture texture;
if (!texture.loadFromFile("assets/textures/road_asphalt.png")) {
    // Gestion d'erreur
}

// Exemple de chargement d'une police
sf::Font font;
if (!font.loadFromFile("assets/fonts/arial.ttf")) {
    // Gestion d'erreur
}
```

## Notes importantes

- Vérifiez les droits d'utilisation des assets externes
- Optimisez la taille des fichiers pour les performances
- Utilisez des formats avec transparence (PNG) quand nécessaire
- Gardez des copies de sauvegarde des assets sources