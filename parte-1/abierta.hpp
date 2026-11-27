#ifndef ABIERTA_HPP
#define ABIERTA_HPP

#include <vector>
#include <cstddef>

// en cada entrada guardamos la posicion del nodo y sus prioridades
struct EntradaAbierta {
    std::size_t indiceNodo;
    double f;
    double h;
    std::size_t orden;
};

class Abierta {

private:

    // almacenamos los nodos ordenados por prioridad
    std::vector<EntradaAbierta> elementos;

    // guardamos el tamaño maximo que ha alcanzado la abierta
    std::size_t maximoAlcanzado;

    // usamos un contador para resolver los empates entre entradas
    std::size_t siguienteOrden;

    // comprueba que entrada tiene mayor prioridad
    bool tieneMayorPrioridad(
        const EntradaAbierta& a,
        const EntradaAbierta& b
    ) const;

    // recolocamos la entrada para mantener el orden de prioridad
    void subir(std::size_t indice);

    // reorganizamos las entradas despues de extraer el minimo
    void bajar(std::size_t indice);

public:
    Abierta();

    // insertamos un nodo indicando su indice y sus prioridades
    void insertar(std::size_t indiceNodo, double f, double h);

    // extraemos la entrada con menor f, desempatando por h y orden
    EntradaAbierta extraerMinimo();

    // consultamos esa entrada sin eliminarla
    const EntradaAbierta& consultarMinimo() const;

    // comprobamos si quedan entradas pendientes
    bool estaVacia() const;

    // devolvemos el numero de entradas almacenadas
    std::size_t tamano() const;

    // devolvemos el tamaño maximo que ha alcanzado la abierta
    std::size_t obtenerMaximoAlcanzado() const;

    // eliminamos todas las entradas almacenadas
    void limpiar();
};

#endif