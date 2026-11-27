#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "grafo.hpp"
#include "resultado.hpp"

// busca la ruta de menor distancia entre los dos aeropuertos
Resultado dijkstra(const Grafo& grafo, int origen, int destino);

#endif