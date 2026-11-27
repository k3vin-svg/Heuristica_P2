#include "estado.hpp"

// creamos el estado con el código del aeropuerto
Estado::Estado(const std::string& codigo)
    : iata(codigo) {
}

// devolvemos el código sin hacer una copia
const std::string& Estado::obtenerIata() const {
    return iata;
}

// guardamos el aeropuerto, los costes y el nodo del que venimos
NodoBusqueda::NodoBusqueda(
    int estado,
    double g,
    double h,
    int padre
)
    : estado(estado),
      g(g),
      h(h),
      f(g + h),
      padre(padre) {
}