#ifndef A_ESTRELLA_HPP
#define A_ESTRELLA_HPP

#include "dijkstra.hpp"

// busca la ruta de menor distancia usando una estimación hasta el destino
Resultado astar(const Grafo& grafo, int origen, int destino);

#endif