# Borg Cataclysm 🚀

Trabajo Final – Programación Orientada a Objetos  
FICH – Universidad Nacional del Litoral

---

## 📘 Descripción General

**Borg Cataclysm** es un videojuego 2D estilo arcade, desarrollado en **C++ con SFML**, como **Trabajo Práctico Final de la materia Programación Orientada a Objetos (POO)** de la **FICH – UNL**.

El juego está ambientado en el universo de **Star Trek: Deep Space Nine (DS9)**. El jugador debe defender la estación **DS9** enfrentando oleadas crecientes de enemigos cardassianos, Jem'Hadar, fundadores y Maquis.

Este proyecto cumple con las premisas establecidas en las **Consignas del Trabajo Final de POO**:

- Diseño orientado a objetos (clases, herencia, polimorfismo).
- Separación entre lógica, presentación y entidades del modelo.
- Manejo obligatorio de archivos (sistema de puntuaciones).
- Desarrollo completo en C++ con biblioteca gráfica (SFML).

---

## 🕹️ Características Principales

### ✔ Shooter 2D arcade retro

- Acción rápida, estética pixel art, controles simples.
- Movimiento libre en 2D dentro del 20% inferior de la pantalla.

### ✔ Sistema de vidas

- El jugador inicia con **3 vidas por run**.
- Al morir pierde potenciadores y reaparece con 2.5s de invulnerabilidad.
- Las vidas pueden aparecer como power‑ups (muy raras).

### ✔ Oleadas infinitas

- Niveles 1 y 2 son predefinidos.
- A partir del nivel 3, se generan **oleadas procedurales infinitas**.
- La dificultad escala automáticamente (velocidad, daño, cantidad).
- El jugador progresa mientras defiende DS9.

### ✔ Armas y proyectiles

- Cada nave puede equipar **2 armas**, con cooldown independiente.
- La nave también escala automáticamente (velocidad, daño, vida).
- Potenciadores temporales: doble disparo, velocidad, escudo, daño, etc.

### ✔ Sistema de puntuación y archivos

- Al finalizar la run se calcula la puntuación.
- Se guarda en un archivo local la tabla **Top 10**.
- Si el jugador entra en el Top 10, se le pide un nombre y se guarda registro.

---

## 📂 Estructura del Proyecto (Actual)

Acorde a la estructura actual del repositorio: (posteriormente se agregarán las escenas GameOver y Score como asi tambien las clases faltantes)

```
assets/             # Recursos del juego (imágenes, sonidos, fuentes)
  ├── fonts/
  ├── img/
  └── sound/

src/
  ├── core/
  │     │
  │     ├── entities/
  │     │     ├── Entity.h
  │     │     ├── SpaceShip.h
  │     │     └── SpaceShip.cpp
  │     │
  │     ├── projectiles/
  │     │     ├── LaserProjectile.h
  │     │     └── MissileProjectile.h
  │     │
  │     ├── scenes/
  │     │     ├── Scene.h / Scene.cpp
  │     │     ├── MenuScene.h / MenuScene.cpp
  │     │     ├── GameScene.h / GameScene.cpp
  │     │     ├── GameOverScene.* (a agregar)
  │     │     └── ScoreScene.* (a agregar)
  │     │
  │     └── weapons/
  │           ├── Weapon.h
  │           └── Weapon.cpp
  │
  └── main.cpp

README.md
```

(Carpetas **build**, **bin**, **tools** y **.vscode** se excluyen del esquema oficial ya que son de uso local y no forman parte del proyecto fuente.)

---

## 🧬 Arquitectura OO

El proyecto implementa un diseño orientado a objetos completo:

### 🧱 Entidades

- **Entity** (abstracta)
  - Hereda de `sf::Drawable` y `sf::Transformable`.
  - Proporciona posición, vida, tipo y estado “alive”.
- **SpaceShip** (jugador)
- **Projectile**
- **Weapon**
- **Enemy** (a integrar con subclases según facciones)

### 🎭 Sistema de Escenas

Basado en polimorfismo y composición:

- `Scene` (abstracta)
- `MenuScene`
- `ShipSelectScene`
- `GameScene`
- `GameOverScene`
- `ScoreScene`

### 📈 Sistema de Oleadas

A integrar según GDD:

- `Wave`
- `WaveGenerator`

---

## 🏗️ Instalación y Compilación

### Requisitos

- C++17 o superior
- SFML 3.0.0
- CMake (opcional)

### Compilación (Linux/Mac/Windows) - Generacion del ejecutable

```bash
git clone https://github.com/FranciscoJoseBonet/borg-cataclysm.git
cd borg-cataclysm
mkdir build && cd build
cmake ..
cmake --build .
./borg-cataclysm
```

---

## 📄 Consignas Académicas Cumplidas (TP Final – POO FICH-UNL)

Este proyecto cumple con los siguientes puntos establecidos en las **Consignas del Trabajo Final**:

- ✔ Desarrollo en **C++** con **orientación a objetos** real.
- ✔ Separación entre modelo, lógica y presentación.
- ✔ Uso de biblioteca gráfica (SFML).
- ✔ Uso obligatorio de **archivos** (puntuaciones persistentes).
- ✔ Proyecto suficientemente complejo para defensa oral individual.
- ✔ Permite mostrar herencia, polimorfismo, encapsulamiento y modularidad.

---

## ⭐ Objetivo del Jugador

Sobrevivir la mayor cantidad de niveles posibles, mejorar su nave mediante potenciadores, derrotar enemigos de diversas facciones y alcanzar un puntaje histórico mientras defiende **Deep Space Nine**.

---

## 👨‍🚀 Autor

**Francisco José Bonet**  
FICH – Universidad Nacional del Litoral  
Materia: Programación Orientada a Objetos

---

## 📄 Licencia

Proyecto académico
