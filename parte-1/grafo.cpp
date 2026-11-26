#include "grafo.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <stdexcept>

namespace {
    constexpr double PI = 3.14159265358979323846;
    constexpr double RADIO_TIERRA = 6371.0088;

    // leemos una fila respetando las comas y los saltos dentro de comillas
    bool leerFila(
        std::istream& archivo,
        std::vector<std::string>& campos
    ) {
        campos.clear();

        std::string campo;
        bool comillas = false;
        bool hayDatos = false;
        char caracter;

        while (archivo.get(caracter)) {
            hayDatos = true;

            if (caracter == '"') {
                // dos comillas seguidas representan una comilla en el texto
                if (comillas && archivo.peek() == '"') {
                    campo += '"';
                    archivo.get();
                } else {
                    comillas = !comillas;
                }
            } else if (caracter == ',' && !comillas) {
                campos.push_back(campo);
                campo.clear();
            } else if (
                (caracter == '\n' || caracter == '\r') && !comillas
            ) {
                // algunos archivos terminan las filas con dos caracteres
                if (caracter == '\r' && archivo.peek() == '\n') {
                    archivo.get();
                }

                campos.push_back(campo);
                return true;
            } else {
                campo += caracter;
            }
        }

        if (comillas) {
            throw std::runtime_error("Hay una fila con comillas sin cerrar");
        }

        // también recogemos la última fila si no termina con un salto
        if (hayDatos) {
            campos.push_back(campo);
            return true;
        }

        return false;
    }

    // pasamos los grados del archivo a radianes para usar seno y coseno
    double radianes(double grados) {
        return grados * PI / 180.0;
    }
}

// cargamos primero los aeropuertos y después las conexiones entre ellos
void Grafo::cargar(
    const std::string& archivoVertices,
    const std::string& archivoEdges
) {
    std::ifstream vertices(archivoVertices);
    std::ifstream edges(archivoEdges);

    if (!vertices.is_open()) {
        throw std::runtime_error(
            "No se puede abrir el archivo: " + archivoVertices
        );
    }

    if (!edges.is_open()) {
        throw std::runtime_error(
            "No se puede abrir el archivo: " + archivoEdges
        );
    }

    // borramos los datos anteriores por si volvemos a cargar el grafo
    estados.clear();
    coordenadas.clear();
    conexiones.clear();
    indices.clear();

    std::vector<std::string> campos;

    // saltamos la cabecera de vertices.csv
    if (!leerFila(vertices, campos)) {
        throw std::runtime_error("El archivo de aeropuertos esta vacio");
    }

    while (leerFila(vertices, campos)) {
        // ignoramos las líneas que no tienen contenido
        if (campos.size() == 1 && campos[0].empty()) {
            continue;
        }

        if (campos.size() < 3 || campos[0].empty()) {
            throw std::runtime_error("Hay una fila de aeropuerto incompleta");
        }

        const std::string& iata = campos[0];

        if (indices.find(iata) != indices.end()) {
            throw std::runtime_error("El aeropuerto esta repetido: " + iata);
        }

        // en el csv aparece primero la longitud y después la latitud
        double longitud = std::stod(campos[1]);
        double latitud = std::stod(campos[2]);

        if (
            !std::isfinite(latitud) || !std::isfinite(longitud) ||
            latitud < -90.0 || latitud > 90.0 ||
            longitud < -180.0 || longitud > 180.0
        ) {
            throw std::runtime_error(
                "Las coordenadas no son validas: " + iata
            );
        }

        int indice = static_cast<int>(estados.size());

        indices.emplace(iata, indice);
        estados.emplace_back(iata);
        coordenadas.push_back({latitud, longitud});
        conexiones.emplace_back();
    }

    // saltamos la cabecera de edges.csv
    if (!leerFila(edges, campos)) {
        throw std::runtime_error("El archivo de conexiones esta vacio");
    }

    while (leerFila(edges, campos)) {
        if (campos.size() == 1 && campos[0].empty()) {
            continue;
        }

        if (campos.size() < 2) {
            throw std::runtime_error("Hay una conexion incompleta");
        }

        int origen = buscarAeropuerto(campos[0]);
        int destino = buscarAeropuerto(campos[1]);

        // volver al mismo aeropuerto no mejora la distancia recorrida
        if (origen == destino) {
            continue;
        }

        auto& destinos = conexiones[origen];
        bool repetida = false;

        // varios vuelos pueden representar la misma conexión en esta parte
        for (const Conexion& conexion : destinos) {
            if (conexion.destino == destino) {
                repetida = true;
                break;
            }
        }

        if (!repetida) {
            double distancia = calcularDistancia(origen, destino);
            destinos.push_back({destino, distancia});
        }
    }
}

// buscamos el aeropuerto sin recorrer todo el vector de estados
int Grafo::buscarAeropuerto(const std::string& iata) const {
    auto encontrado = indices.find(iata);

    if (encontrado == indices.end()) {
        throw std::runtime_error("No existe el aeropuerto: " + iata);
    }

    return encontrado->second;
}

// devolvemos el aeropuerto que ocupa esa posición
const Estado& Grafo::obtenerEstado(int indice) const {
    return estados.at(indice);
}

// devolvemos las conexiones sin copiar el vector
const std::vector<Conexion>& Grafo::obtenerConexiones(int origen) const {
    return conexiones.at(origen);
}

// aplicamos haversine usando las coordenadas de los dos aeropuertos
double Grafo::calcularDistancia(int origen, int destino) const {
    const Coordenadas& inicio = coordenadas.at(origen);
    const Coordenadas& fin = coordenadas.at(destino);

    double latitudInicio = radianes(inicio.latitud);
    double latitudFin = radianes(fin.latitud);

    double diferenciaLatitud = latitudFin - latitudInicio;
    double diferenciaLongitud = radianes(fin.longitud - inicio.longitud);

    double senoLatitud = std::sin(diferenciaLatitud / 2.0);
    double senoLongitud = std::sin(diferenciaLongitud / 2.0);
    double valor = senoLatitud * senoLatitud + std::cos(latitudInicio) * std::cos(latitudFin) * senoLongitud * senoLongitud;

    // evitamos que el redondeo deje el valor fuera del intervalo permitido
    valor = std::max(0.0, std::min(1.0, valor));

    return 2.0 * RADIO_TIERRA * std::atan2(
        std::sqrt(valor),
        std::sqrt(1.0 - valor)
    );
}

// contamos los aeropuertos que hemos leído del archivo
std::size_t Grafo::tamano() const {
    return estados.size();
}