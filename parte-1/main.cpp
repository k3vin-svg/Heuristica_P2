#include "grafo.hpp"
#include "dijkstra.hpp"
#include "a_estrella.hpp"

#include <chrono>
#include <cmath>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {
    // quitamos los espacios que puedan rodear al código del aeropuerto
    std::string quitarEspacios(const std::string& texto) {
        std::size_t inicio = texto.find_first_not_of(" \t\r\n");

        if (inicio == std::string::npos) {
            return "";
        }

        std::size_t fin = texto.find_last_not_of(" \t\r\n");

        return texto.substr(inicio, fin - inicio + 1);
    }

    // preparamos la distancia y el recorrido con el formato del enunciado
    std::string prepararSalida(const Resultado& resultado) {
        std::ostringstream salida;
        salida.imbue(std::locale::classic());

        salida << std::fixed << std::setprecision(2);
        salida << resultado.distancia << '\n';

        for (std::size_t i = 0; i < resultado.recorrido.size(); i++) {
            if (i > 0) {
                salida << " - ";
            }

            salida << resultado.recorrido[i];
        }

        salida << '\n';

        return salida.str();
    }

    // mostramos la solución y las medidas del algoritmo que hemos ejecutado
    void mostrarResultado(
        const std::string& nombre,
        const Resultado& resultado,
        double tiempo
    ) {
        std::cout << "Algoritmo: " << nombre << '\n';

        if (resultado.encontrado) {
            std::cout << prepararSalida(resultado);
        } else {
            std::cout << "No hay una ruta entre los aeropuertos\n";
        }

        std::cout << "Nodos expandidos: " << resultado.expandidos << '\n';
        std::cout << "Nodos generados: " << resultado.generados << '\n';
        std::cout << "Maximo de abierta: " << resultado.maximoAbierta << '\n';

        std::cout << std::fixed << std::setprecision(3);
        std::cout << "Tiempo de busqueda: " << tiempo << " ms\n\n";
    }
}

// resolvemos el mismo problema con los dos algoritmos
int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr
            << "Uso: " << argv[0]
            << " fichero-entrada fichero-salida\n";

        return 1;
    }

    try {
        std::ifstream entrada(argv[1]);

        if (!entrada.is_open()) {
            throw std::runtime_error(
                std::string("No se puede abrir la entrada: ") + argv[1]
            );
        }

        std::string origen;
        std::string destino;

        // leemos un aeropuerto en cada línea
        if (
            !std::getline(entrada, origen) ||
            !std::getline(entrada, destino)
        ) {
            throw std::runtime_error(
                "La entrada debe contener una linea para cada aeropuerto"
            );
        }

        origen = quitarEspacios(origen);
        destino = quitarEspacios(destino);

        if (origen.empty() || destino.empty()) {
            throw std::runtime_error(
                "Los codigos de los aeropuertos no pueden estar vacios"
            );
        }

        // permitimos líneas vacías al final, pero no datos adicionales
        std::string linea;

        while (std::getline(entrada, linea)) {
            if (!quitarEspacios(linea).empty()) {
                throw std::runtime_error(
                    "La entrada contiene datos despues del destino"
                );
            }
        }

        if (entrada.bad()) {
            throw std::runtime_error("No se ha podido leer la entrada");
        }

        entrada.close();

        // cargamos el grafo una sola vez para las dos búsquedas
        Grafo grafo;
        grafo.cargar("vertices.csv", "edges.csv");

        int inicio = grafo.buscarAeropuerto(origen);
        int fin = grafo.buscarAeropuerto(destino);

        // medimos solo la búsqueda de dijkstra
        auto comienzoDijkstra = std::chrono::steady_clock::now();

        Resultado resultadoDijkstra = dijkstra(grafo, inicio, fin);

        auto finalDijkstra = std::chrono::steady_clock::now();

        double tiempoDijkstra =
            std::chrono::duration<double, std::milli>(
                finalDijkstra - comienzoDijkstra
            ).count();

        // a estrella empieza una búsqueda nueva con sus propias estructuras
        auto comienzoEstrella = std::chrono::steady_clock::now();

        Resultado resultadoEstrella = astar(grafo, inicio, fin);

        auto finalEstrella = std::chrono::steady_clock::now();

        double tiempoEstrella =
            std::chrono::duration<double, std::milli>(
                finalEstrella - comienzoEstrella
            ).count();

        mostrarResultado("Dijkstra", resultadoDijkstra, tiempoDijkstra);
        mostrarResultado("A*", resultadoEstrella, tiempoEstrella);

        // ambos deben coincidir en si existe una solución
        if (resultadoDijkstra.encontrado != resultadoEstrella.encontrado) {
            throw std::runtime_error(
                "Los algoritmos no coinciden en si existe una ruta"
            );
        }

        if (!resultadoEstrella.encontrado) {
            return 2;
        }

        // dejamos un pequeño margen por las operaciones con decimales
        double diferencia = std::abs(
            resultadoDijkstra.distancia - resultadoEstrella.distancia
        );

        if (diferencia > 0.000001) {
            throw std::runtime_error(
                "Los algoritmos han obtenido distancias diferentes"
            );
        }

        std::cout << "Ambos algoritmos obtienen la misma distancia optima.\n";

        // guardamos una única solución con las dos líneas exigidas
        std::ofstream salida(argv[2]);

        if (!salida.is_open()) {
            throw std::runtime_error(
                std::string("No se puede abrir la salida: ") + argv[2]
            );
        }

        salida << prepararSalida(resultadoEstrella);
        salida.close();

        if (!salida) {
            throw std::runtime_error(
                "No se ha podido guardar el resultado completo"
            );
        }

        return 0;

    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}