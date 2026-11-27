#include "grafo.hpp"
#include "dijkstra.hpp"
#include "a_estrella.hpp"

#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {
    // quitamos los espacios y los saltos que puedan rodear al código
    std::string quitarEspacios(const std::string& texto) {
        std::size_t inicio = texto.find_first_not_of(" \t\r\n");

        if (inicio == std::string::npos) {
            return "";
        }

        std::size_t fin = texto.find_last_not_of(" \t\r\n");

        return texto.substr(inicio, fin - inicio + 1);
    }

    // preparamos las dos líneas que mostraremos y guardaremos
    std::string prepararSalida(const Resultado& resultado) {
        std::ostringstream salida;

        // usamos el punto decimal aunque el equipo tenga otra configuración
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
}

// leemos el problema, buscamos la ruta y escribimos el resultado
int main(int argc, char* argv[]) {
    // recibimos entrada y salida, y opcionalmente el algoritmo
    if (argc != 3 && argc != 4) {
        std::cerr
            << "Uso: " << argv[0]
            << " entrada salida [a_estrella|dijkstra]\n";

        return 1;
    }

    try {
        std::string algoritmo = "a_estrella";

        if (argc == 4) {
            algoritmo = argv[3];
        }

        if (algoritmo != "a_estrella" && algoritmo != "dijkstra") {
            throw std::runtime_error(
                "el algoritmo debe ser a_estrella o dijkstra"
            );
        }

        std::ifstream entrada(argv[1]);

        if (!entrada.is_open()) {
            throw std::runtime_error(
                std::string("No se puede abrir la entrada: ") + argv[1]
            );
        }

        std::string origen;
        std::string destino;

        // cada aeropuerto debe aparecer en una línea distinta
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

        // permitimos líneas vacías al final, pero no más datos
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

        // buscamos los csv en el directorio desde el que se ejecuta el programa
        Grafo grafo;
        grafo.cargar("vertices.csv", "edges.csv");

        int inicio = grafo.buscarAeropuerto(origen);
        int fin = grafo.buscarAeropuerto(destino);

        Resultado resultado;

        if (algoritmo == "dijkstra") {
            resultado = dijkstra(grafo, inicio, fin);
        } else {
            resultado = astar(grafo, inicio, fin);
        }

        if (!resultado.encontrado) {
            std::cerr
                << "No hay una ruta desde "
                << origen << " hasta " << destino << '\n';

            return 2;
        }

        std::string texto = prepararSalida(resultado);

        // abrimos la salida después de encontrar una solución
        std::ofstream salida(argv[2]);

        if (!salida.is_open()) {
            throw std::runtime_error(
                std::string("No se puede abrir la salida: ") + argv[2]
            );
        }

        salida << texto;
        salida.close();

        if (!salida) {
            throw std::runtime_error(
                "No se ha podido guardar el resultado completo"
            );
        }

        // mostramos exactamente el mismo contenido que hemos guardado
        std::cout << texto;

        return 0;
    } catch (const std::exception& error) {
        // enviamos los errores por separado de la salida de la solución
        std::cerr << "Error: " << error.what() << '\n';

        return 1;
    }
}