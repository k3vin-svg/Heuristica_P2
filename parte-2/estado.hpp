#ifndef ESTADO_HPP
#define ESTADO_HPP

#include <string>

// representa el aeropuerto en el que nos encontramos
class Estado {
private:
    std::string iata;

public:
    // guarda el codigo del aeropuerto al crear el estado
    explicit Estado(const std::string& codigo);

    // devuelve el código del aeropuerto
    const std::string& obtenerIata() const;
};

// representa un camino que llega hasta un aeropuerto
struct NodoBusqueda {
    int estado;
    double g;
    double h;
    double f;

    // guarda el indice del nodo anterior o -1 si es el inicial
    int padre;

    // guarda los datos del camino y calcula f
    NodoBusqueda(
        int estado,
        double g,
        double h,
        int padre = -1
    );
};

#endif