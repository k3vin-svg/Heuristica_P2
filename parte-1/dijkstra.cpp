#include "dijkstra.hpp"

#include "abierta.hpp"
#include "cerrada.hpp"

#include <algorithm>
#include <limits>
#include <vector>

// buscamos la ruta dando prioridad a la menor distancia acumulada
Resultado dijkstra(const Grafo& grafo, int origen, int destino) {
    // comprobamos que los índices correspondan a aeropuertos del grafo
    grafo.obtenerEstado(origen);
    grafo.obtenerEstado(destino);

    Resultado resultado;
    Abierta abierta;
    Cerrada cerrada(grafo.tamano());

    // guardamos los nodos para poder reconstruir el camino al terminar
    std::vector<NodoBusqueda> nodos;

    // al principio no conocemos ninguna distancia hasta los aeropuertos
    std::vector<double> mejores(
        grafo.tamano(),
        std::numeric_limits<double>::infinity()
    );

    // llegar al aeropuerto de salida no supone recorrer ninguna distancia
    mejores[origen] = 0.0;
    nodos.emplace_back(origen, 0.0, 0.0);

    abierta.insertar(0, 0.0, 0.0);
    resultado.generados = 1;

    while (!abierta.estaVacia()) {
        EntradaAbierta entrada = abierta.extraerMinimo();

        // copiamos el nodo porque el vector puede crecer al añadir otros
        NodoBusqueda actual = nodos[entrada.indiceNodo];

        // descartamos el camino si ya encontramos otro más corto
        if (actual.g > mejores[actual.estado]) {
            continue;
        }

        // si ya recorrimos sus conexiones, no lo expandimos otra vez
        if (cerrada.contiene(actual.estado)) {
            continue;
        }

        // al extraer el destino, su distancia ya es la mínima
        if (actual.estado == destino) {
            resultado.encontrado = true;
            resultado.distancia = actual.g;

            int indice = static_cast<int>(entrada.indiceNodo);

            // seguimos los padres desde el destino hasta el origen
            while (indice != -1) {
                const NodoBusqueda& nodo = nodos[indice];

                resultado.recorrido.push_back(
                    grafo.obtenerEstado(nodo.estado).obtenerIata()
                );

                indice = nodo.padre;
            }

            // el recorrido se ha guardado al revés, así que le damos la vuelta
            std::reverse(
                resultado.recorrido.begin(),
                resultado.recorrido.end()
            );

            break;
        }

        cerrada.insertar(actual.estado);
        resultado.expandidos++;

        // revisamos los aeropuertos conectados con el actual
        for (
            const Conexion& conexion :
            grafo.obtenerConexiones(actual.estado)
        ) {
            if (cerrada.contiene(conexion.destino)) {
                continue;
            }

            double distancia = actual.g + conexion.distancia;

            // solo guardamos el camino si mejora la distancia conocida
            if (distancia < mejores[conexion.destino]) {
                mejores[conexion.destino] = distancia;

                int padre = static_cast<int>(entrada.indiceNodo);

                // en dijkstra dejamos h a cero, por lo que f coincide con g
                nodos.emplace_back(
                    conexion.destino,
                    distancia,
                    0.0,
                    padre
                );

                std::size_t indice = nodos.size() - 1;
                abierta.insertar(indice, distancia, 0.0);

                resultado.generados++;
            }
        }
    }

    resultado.maximoAbierta = abierta.obtenerMaximoAlcanzado();
    return resultado;
}