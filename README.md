# 🚀 Space Invaders - Star Trek Edition

### 🎮 Descripción

Este es un juego 2D inspirado en _Space Invaders_, pero con una temática basada en _Star Trek_. Los jugadores pueden seleccionar su nave según facciones icónicas de la saga, cada una con habilidades pasivas únicas. Los enemigos tienen patrones de ataque definidos por algoritmos simples.

El juego está desarrollado en **C++** utilizando la librería **SFML** para los gráficos y animaciones en _pixel art_. Los datos se guardan en archivos binarios.

---

## 🛠️ Tecnologías utilizadas

- **Lenguaje:** C++
- **Librería gráfica:** SFML
- **Formato de guardado:** Archivos binarios
- **Estilo visual:** Pixel art con animaciones básicas

---

## 📂 Estructura del Proyecto

```
/SpaceInvaders-StarTrek/
│── /bin/                  # Ejecutables y archivos binarios
│── /assets/               # Sprites, sonidos y fuentes
│── /src/                  # Código fuente
│   │── /core/             # Lógica del juego (sin SFML)
│   │   │── Game.cpp/h     # Controla el flujo del juego
│   │   │── Player.cpp/h   # Lógica del jugador (nave + atributos)
│   │   │── Enemy.cpp/h    # IA básica de enemigos
│   │   │── Projectile.cpp/h # Disparos y proyectiles
│   │   │── Utils.cpp/h    # Funciones auxiliares (archivos binarios, etc.)
│   │── /graphics/         # Parte visual (con SFML)
│   │   │── Renderer.cpp/h # Renderiza todos los elementos en pantalla
│   │   │── Animations.cpp/h # Manejo de animaciones
│   │   │── UI.cpp/h       # Interfaz de usuario (menús, HUD)
│   │── main.cpp           # Punto de entrada del juego
│── /include/              # Archivos .h para evitar sobrecarga en src/
│── /build/                # Carpeta para compilación
│── CMakeLists.txt         # Configuración de CMake
│── README.md              # Explicación del proyecto
```

---

## 🎮 Modo de juego

1. Elegí tu nave según la facción de Star Trek (Federación, Klingon, Romulanos, etc.)
2. Cada nave tiene una habilidad pasiva diferente
3. Enfrentate a oleadas de enemigos que tienen patrones de ataque variados
4. ¡Sobreviví y conseguí la mayor puntuación posible!

---

## 🖥️ Contribuciones

Si querés colaborar en este proyecto, podés hacer un _fork_ y enviar un _pull request_ con mejoras o correcciones.

---

¡Cualquier sugerencia es bienvenida! 🚀
