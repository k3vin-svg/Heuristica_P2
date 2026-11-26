#include "abierta.hpp"
#include <stdexcept>
#include <utility>

// inicializamos la lista abierta y sus contadores
Abierta::Abierta()
    : maximoAlcanzado(0), siguienteOrden(0) {
}

// comprueba que entrada tiene mayor prioridad, primero comparamos f, despues h y por último el orden de insercion
bool Abierta::tieneMayorPrioridad(
    const EntradaAbierta& a,
    const EntradaAbierta& b
) const {
    if (a.f < b.f) {
        return true;
    }
    if (a.f > b.f) {
        return false;
    }
    if (a.h < b.h) {
        return true;
    }
    if (a.h > b.h) {
        return false;
    }
    return a.orden < b.orden;
}

// subimos la entrada hasta encontrar su posicion correcta
void Abierta::subir(std::size_t indice) {

    while (indice > 0) {

        std::size_t padre = (indice - 1) / 2;
        // si la entrada tiene mayor prioridad que su padre los intercambiamos
        if (tieneMayorPrioridad(elementos[indice], elementos[padre])) {
            std::swap(elementos[indice], elementos[padre]);
            indice = padre;

        } else {
            break;
        }
    }
}


// bajamos la entrada hasta encontrar su posicion correcta
void Abierta::bajar(std::size_t indice) {

    std::size_t n = elementos.size();
    while (true) {

        std::size_t izquierdo = 2 * indice + 1;
        std::size_t derecho = 2 * indice + 2;
        std::size_t mejor = indice;

        // comprobamos si el hijo izquierdo tiene mayor prioridad
        if (izquierdo < n &&
            tieneMayorPrioridad(elementos[izquierdo], elementos[mejor])) {
            mejor = izquierdo;
        }

        // comprobamos si el hijo derecho tiene mayor prioridad
        if (derecho < n &&
            tieneMayorPrioridad(elementos[derecho], elementos[mejor])) {
            mejor = derecho;
        }

        // si la entrada actual ya tiene mayor prioridad terminamos
        if (mejor == indice) {
            break;
        }

        // intercambiamos la entrada con el hijo que tiene mayor prioridad
        std::swap(elementos[indice], elementos[mejor]);
        indice = mejor;
    }
}


// insertamos un nodo con su indice y sus prioridades
void Abierta::insertar(
    std::size_t indiceNodo,
    double f,
    double h
) {

    // creamos una entrada con los datos necesarios para ordenar el monticulo
    EntradaAbierta nuevaEntrada = {
        indiceNodo,
        f,
        h,
        siguienteOrden
    };

    siguienteOrden++;

    // añadimos la entrada al final del vector
    elementos.push_back(nuevaEntrada);

    // colocamos la nueva entrada en su posicion correcta
    subir(elementos.size() - 1);

    // actualizamos el tamaño maximo alcanzado
    if (elementos.size() > maximoAlcanzado) {
        maximoAlcanzado = elementos.size();
    }
}

// extraemos la entrada que tiene menor prioridad
EntradaAbierta Abierta::extraerMinimo() {

    // comprobamos que haya entradas disponibles
    if (estaVacia()) {
        throw std::out_of_range(
            "Error: no se puede extraer de una lista abierta vacia"
        );
    }

    // intercambiamos la raiz con la ultima entrada
    std::swap(elementos[0], elementos.back());

    // guardamos la entrada minima
    EntradaAbierta minimo = elementos.back();

    // eliminamos la ultima posicion
    elementos.pop_back();

    // si quedan entradas reorganizamos el monticulo
    if (!elementos.empty()) {
        bajar(0);
    }

    return minimo;
}

// devuelve la entrada con menor prioridad sin eliminarla
const EntradaAbierta& Abierta::consultarMinimo() const {
    if (estaVacia()) {
        throw std::out_of_range(
            "Error: la lista abierta esta vacia"
        );
    }

    return elementos[0];
}

// comprobamos si quedan entradas pendientes
bool Abierta::estaVacia() const {
    return elementos.empty();
}

// devolvemos el numero de entradas almacenadas
std::size_t Abierta::tamano() const {
    return elementos.size();
}

// devolvemos el mayor tamaño que ha alcanzado la abierta
std::size_t Abierta::obtenerMaximoAlcanzado() const {
    return maximoAlcanzado;
}

// eliminamos todas las entradas almacenadas y mantenemos el tamaño maximo para consultar las estadisticas
void Abierta::limpiar() {

    elementos.clear();
    // reiniciamos el orden porque la abierta vuelve a estar vacia
    siguienteOrden = 0;
}