#ifndef RESULTADO_HPP
#define RESULTADO_HPP

#include <cstddef>
#include <string>
#include <vector>

// guarda la solución y las medidas de la búsqueda
struct Resultado {
    bool encontrado = false;
    double distancia = 0.0;
    std::vector<std::string> recorrido;

    std::size_t expandidos = 0;
    std::size_t generados = 0;
    std::size_t maximoAbierta = 0;

    // cuenta las conexiones examinadas para calcular la ramificación
    std::size_t conexionesExaminadas = 0;
};

#endif