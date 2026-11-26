#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "grafo.hpp"

#include <cstddef>
#include <string>
#include <vector>

// recoge la ruta encontrada y los datos para comparar los algoritmos
struct Resultado {
    bool encontrado = false;
    double distancia = 0.0;
    std::vector<std::string> recorrido;

    std::size_t expandidos = 0;
    std::size_t generados = 0;
    std::size_t maximoAbierta = 0;
};

// busca la ruta de menor distancia entre los dos aeropuertos
Resultado dijkstra(const Grafo& grafo, int origen, int destino);

#endif