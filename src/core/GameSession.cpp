#include "GameSession.h"

// Definicion de memoria para las variables estaticas
// Si no hacemos esto en el .cpp, el linker daria error

int GameSession::selectedShipIndex = 0; // Default: Primera nave de la lista del carrousell
int GameSession::currentScore = 0;