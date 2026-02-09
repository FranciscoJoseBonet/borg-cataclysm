---
# Borg Cataclysm 🚀

Trabajo Final – Programación Orientada a Objetos** **FICH – Universidad Nacional del Litoral (UNL)
---

## 📘 Descripción General

**Borg Cataclysm** es un videojuego de acción 2D estilo bullet-hell desarrollado en **C++** utilizando la biblioteca **SFML 3.0**. Este proyecto representa el trabajo integrador final para la materia **Programación Orientada a Objetos**.

El jugador toma el mando de una nave de la Federación en una misión desesperada para contener la invasión de los **Borg**. A través de una estética inspirada en el sistema **LCARS** de Star Trek, el juego desafía al usuario a sobrevivir oleadas de enemigos con patrones de movimiento dinámicos y enfrentamientos contra un jefe masivo (Borg Cube).

---

## 🧬 Implementación de Programación Orientada a Objetos

El núcleo del proyecto ha sido diseñado siguiendo los pilares de la **POO** para garantizar escalabilidad y modularidad:

- **Herencia y Polimorfismo:** Implementación de una jerarquía de clases robusta para entidades. El uso de métodos virtuales (`update`, `draw`, `movePattern`) permite que el motor trate a cualquier objeto (Naves, Enemigos, Proyectiles) de forma genérica.
- **Encapsulamiento:** Gestión estricta de estados internos y visibilidad de miembros, utilizando modificadores `protected` para facilitar la extensión en subclases y `private` para la seguridad de datos.
- **Patrones de Diseño:**
- **Singleton:** Aplicado en `UITheme` para centralizar recursos visuales (fuentes y colores LCARS) y evitar redundancia en memoria, un gestor de estilos casero.
- **Strategy / Callbacks:** Uso de `std::function` y lambdas para desacoplar el sistema de armas del motor de nivel, permitiendo una comunicación eficiente sin dependencias circulares.

- **Composición:** Las naves tienen armas (clase `Weapon`), permitiendo el desacoplamiento de componentes de juego.

---

## 🕹️ Características Técnicas Principales

- **Motor de Renderizado Optimizado:** Uso de `sf::VertexArray` para el fondo de estrellas dinámico, permitiendo procesar cientos de partículas con un costo mínimo de CPU/GPU.
- **Sistema de Animación:** Clase `Explosion` basada en spritesheets con gestión automática de ciclo de vida.
- **Interfaz LCARS:** HUD avanzado que monitorea en tiempo real la integridad estructural (HP), escudos y estados de power-ups.
- **Dificultad Progresiva:** Sistema de oleadas donde la velocidad, daño y frecuencia de disparo de los Borg escalan según el progreso del jugador.
- **Persistencia de Datos:** Manejo de archivos para el guardado y lectura del **Top 10 de mejores puntajes**.

---

## 📂 Estructura del Proyecto

```
src/
 ├── core/              # Bucle principal y gestión de la sesión
 ├── data/              # Gestion de DTO y DAO para compilacion de las naves de forma dinamica
 ├── effects/           # Clase Explosion basada en spritesheets con gestión automática de ciclo de vida
 ├── environment/       # Fondo de estrellas dinámico usando sf::VertexArray
 ├── entities/          # Jerarquía de objetos del juego
 │    ├── items/        # Power-ups (Health, Shield, Weapon Upgrade)
 │    ├── ships/        # Jugador y lógica de naves
 │    │    └── enemies/ # "IA": Scouts, Explorers y Borg Cubes (Bosses)
 │    └── projectiles/  # Tipos de munición (Laser, Misiles, Curvos)
 ├── managers/          # Manejo de recursos (Texturas/Sonido) y niveles
 ├── scenes/            # Máquina de estados: Menú, Selección, Juego, Scores (se gestiona desde core)
 ├── ui/                # HUD y UITheme (Sistema LCARS)
 ├── weapons/           # Clases de armas y sistema de disparo
 └── main.cpp           # Punto de entrada

```

## 🏗️ Instalación y Compilación

### Requisitos

- **Compilador:** C++17 o superior.
- **Biblioteca:** [SFML 3.0.0](https://www.google.com/search?q=https://www.sfml-dev.org/download/3.0.0/) (Requerido).
- **Herramienta de Construcción:** [CMake](https://cmake.org/).

### Pasos para Compilar

**Si se va a compilar el proyecto configurar correctamente el cmakelists con los requisitos para el uso local de cmake**

```bash
# 1. Clonar el repositorio
git clone https://github.com/FranciscoJoseBonet/borg-cataclysm.git
cd borg-cataclysm

# 2. Configurar build con CMake
mkdir build && cd build
cmake ..

# 3. Compilar
cmake --build .

# 4. Ejecutar
./BorgCataclysm

```

_Nota: El proceso de compilación copia automáticamente la carpeta `assets` al directorio de ejecución mediante scripts de CMake._

---

## 📄 Consignas Académicas

Requisitos para la defensa del TP Final:

- ✅ **Modularidad:** Separación clara entre lógica de negocio y presentación.
- ✅ **Archivos:** Implementación de persistencia para el sistema de High Scores.
- ✅ **Complejidad:** Implementación de colisiones, gestión de memoria dinámica y patrones de diseño.
- ✅ **SFML:** Uso integral de la biblioteca para gráficos y eventos.

---

## Aclaración importante sobre assets

**Fuentes:** Se han utilizado fuentes gratuitas disponibles en línea para lograr la estética LCARS. Estas fuentes se incluyen en la carpeta `assets/fonts/` y se han verificado que son de uso libre.

**Imagenes:** Se han creado sprites personalizados para las naves, enemigos y efectos visuales utilizando Nano Banana de Gemini. Estas imágenes se encuentran en `assets/images/`.

---

## 👨‍🚀 Autor

**Francisco José Bonet** Estudiante de Ingeniería - FICH (UNL)

**Materia:** Programación Orientada a Objetos

---
