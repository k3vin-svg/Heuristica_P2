#ifndef A_ESTRELLA_HPP
#define A_ESTRELLA_HPP

#include "grafo.hpp"
#include "resultado.hpp"

// busca la ruta de menor distancia usando una estimación hasta el destino
Resultado a_estrella(const Grafo& grafo, int origen, int destino);

#endif