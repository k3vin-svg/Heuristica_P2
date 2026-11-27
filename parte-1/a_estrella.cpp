#include "a_estrella.hpp"
#include "abierta.hpp"
#include "cerrada.hpp"

#include <algorithm>
#include <limits>
#include <vector>

// buscamos la ruta teniendo en cuenta lo recorrido y lo que falta
Resultado astar(const Grafo& grafo, int origen, int destino) {
    // comprobamos que ambos índices correspondan a aeropuertos del grafo
    grafo.obtenerEstado(origen);
    grafo.obtenerEstado(destino);

    Resultado resultado;
    Abierta abierta;
    Cerrada cerrada(grafo.tamano());

    // conservamos los nodos para reconstruir el camino siguiendo sus padres
    std::vector<NodoBusqueda> nodos;

    // guardamos la menor distancia encontrada hasta cada aeropuerto
    std::vector<double> mejores(
        grafo.tamano(),
        std::numeric_limits<double>::infinity()
    );
    double estimacion = grafo.calcularDistancia(origen, destino);
    mejores[origen] = 0.0;
    nodos.emplace_back(origen, 0.0, estimacion);

    // en el nodo inicial g vale cero, así que f coincide con h
    abierta.insertar(0, estimacion, estimacion);
    resultado.generados = 1;

    while (!abierta.estaVacia()) {
        EntradaAbierta entrada = abierta.extraerMinimo();

        // copiamos el nodo porque el vector puede crecer durante la búsqueda
        NodoBusqueda actual = nodos[entrada.indiceNodo];

        // descartamos esta entrada si ya encontramos un camino más corto
        if (actual.g > mejores[actual.estado]) {
            continue;
        }

        if (cerrada.contiene(actual.estado)) {
            continue;
        }

        // comprobamos el destino al extraerlo, no al descubrirlo
        if (actual.estado == destino) {
            resultado.encontrado = true;
            resultado.distancia = actual.g;

            int indice = static_cast<int>(entrada.indiceNodo);

            // recorremos los padres hasta llegar al nodo inicial
            while (indice != -1) {
                const NodoBusqueda& nodo = nodos[indice];

                resultado.recorrido.push_back(
                    grafo.obtenerEstado(nodo.estado).obtenerIata()
                );

                indice = nodo.padre;}

            // colocamos los aeropuertos desde el origen hasta el destino
            std::reverse(
                resultado.recorrido.begin(),
                resultado.recorrido.end()
            );

            break;
        }
        cerrada.insertar(actual.estado);
        resultado.expandidos++;

        // examinamos las conexiones que salen del aeropuerto actual
        for (
            const Conexion& conexion :
            grafo.obtenerConexiones(actual.estado)
        ) {
            if (cerrada.contiene(conexion.destino)) {
                continue;
            }

            double distancia = actual.g + conexion.distancia;
            if (distancia < mejores[conexion.destino]) {
                mejores[conexion.destino] = distancia;

                // estimamos lo que queda desde este aeropuerto al destino
                double h = grafo.calcularDistancia(
                    conexion.destino,
                    destino
                );

                int padre = static_cast<int>(entrada.indiceNodo);

                nodos.emplace_back(
                    conexion.destino,
                    distancia,
                    h,
                    padre
                );
                std::size_t indice = nodos.size() - 1;

                // damos a la abierta el coste total estimado y el desempate
                abierta.insertar(indice, nodos[indice].f, h);
                resultado.generados++;
            }
        }
    }

    resultado.maximoAbierta = abierta.obtenerMaximoAlcanzado();

    return resultado;
}