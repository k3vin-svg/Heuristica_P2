#ifndef GRAFO_HPP
#define GRAFO_HPP

#include "estado.hpp"
#include <string>
#include <unordered_map>
#include <vector>

// guardamos el aeropuerto al que podemos ir y la distancia del tramo
struct Conexion {
    int destino;
    double distancia;
};

// guardamos las coordenadas necesarias para calcular las distancias
struct Coordenadas {
    double latitud;
    double longitud;
};

class Grafo {
private:
    std::vector<Estado> estados;
    std::vector<Coordenadas> coordenadas;

    // cada posición contiene las conexiones de un aeropuerto
    std::vector<std::vector<Conexion>> conexiones;

    // encontramos la pos de un aeropuerto usando su código
    std::unordered_map<std::string, int> indices;

public:
    // cargamos los aeropuertos y sus conexiones desde los dos archivos
    void cargar(
        const std::string& archivoVertices,
        const std::string& archivoEdges
    );

    // buscamos un código y devolvemos su posición en el grafo
    int buscarAeropuerto(const std::string& iata) const;

    // consultamos el estado de un aeropuerto
    const Estado& obtenerEstado(int indice) const;

    // devolvemos los destinos a los que podemos volar desde ese aeropuerto
    const std::vector<Conexion>& obtenerConexiones(int origen) const;

    // calculamos la distancia haversine entre dos aeropuertos
    double calcularDistancia(int origen, int destino) const;

    // devolvemos el numero de aeropuertos cargados
    std::size_t tamano() const;
};

#endif