#pragma once

// Clase de contexto
// Sirve para compartir datos entre escenas (ej: Del Menu de Seleccion al Juego)
// sin tener que pasar parametros complejos

class GameSession
{
public:
    // Las variables no pertenecen a una instancia, sino a la clase misma (static)
    static int selectedShipIndex;
    static int currentScore;

    // Borré el constructor para evitar instanciar esta clase por error
    GameSession() = delete;
};