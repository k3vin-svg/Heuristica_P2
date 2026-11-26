#include "cerrada.hpp"
#include <algorithm>

// al empezar ningún aeropuerto esta cerrado
Cerrada::Cerrada(std::size_t aeropuertos)
    : visitados(aeropuertos, false),
      cantidad(0) {
}

// marcamos el aeropuerto sin contarlo otra vez si ya estaba cerrado
void Cerrada::insertar(std::size_t estado) {
    if (!visitados.at(estado)) {
        visitados.at(estado) = true;
        cantidad++;
    }
}

// consultamos la posición que corresponde al aeropuerto
bool Cerrada::contiene(std::size_t estado) const {
    return visitados.at(estado);
}

// devolvemos el número de aeropuertos cerrados
std::size_t Cerrada::tamano() const {
    return cantidad;
}

// si el contador está a 0, la cerrada está vacía
bool Cerrada::estaVacia() const {
    return cantidad == 0;
}

// quitamos las marcas y reiniciamos el contador
void Cerrada::limpiar() {
    std::fill(visitados.begin(), visitados.end(), false);
    cantidad = 0;
}