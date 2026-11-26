#ifndef CERRADA_HPP
#define CERRADA_HPP

#include <cstddef>
#include <vector>

class Cerrada {
private:
    // cada posición indica si ya hemos expandido ese aeropuerto
    std::vector<bool> visitados;

    // cuenta cuántos aeropuertos hemos cerrado
    std::size_t cantidad;

public:
    // prepara una posicion para cada aeropuerto del grafo
    explicit Cerrada(std::size_t aeropuertos);

    // marca el aeropuerto como cerrado
    void insertar(std::size_t estado);

    // comprueba si el aeropuerto ya está cerrado
    bool contiene(std::size_t estado) const;

    // devuelve cuántos aeropuertos están cerrados
    std::size_t tamano() const;

    // comprueba si todavía no hemos cerrado ningun aeropuerto
    bool estaVacia() const;

    // deja la cerrada preparada para empezar otra búsqueda
    void limpiar();
};

#endif