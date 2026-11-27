CXX = g++
CXXFLAGS = -O3 -std=c++17 -Wall

TARGET = principal

# reunimos los objetos que necesitamos para crear el ejecutable
OBJS = main.o estado.o grafo.o dijkstra.o a_estrella.o abierta.o cerrada.o

all: $(TARGET)

# unimos los objetos para generar el programa
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# compilamos el programa principal
main.o: main.cpp grafo.hpp estado.hpp dijkstra.hpp a_estrella.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

# compilamos los estados y los nodos de búsqueda
estado.o: estado.cpp estado.hpp
	$(CXX) $(CXXFLAGS) -c estado.cpp

# compilamos la lectura y las operaciones del grafo
grafo.o: grafo.cpp grafo.hpp estado.hpp
	$(CXX) $(CXXFLAGS) -c grafo.cpp

# compilamos la búsqueda con dijkstra
dijkstra.o: dijkstra.cpp dijkstra.hpp grafo.hpp estado.hpp abierta.hpp cerrada.hpp
	$(CXX) $(CXXFLAGS) -c dijkstra.cpp

# compilamos la búsqueda con a estrella
a_estrella.o: a_estrella.cpp a_estrella.hpp dijkstra.hpp grafo.hpp estado.hpp abierta.hpp cerrada.hpp
	$(CXX) $(CXXFLAGS) -c a_estrella.cpp

# compilamos el montículo de la abierta
abierta.o: abierta.cpp abierta.hpp
	$(CXX) $(CXXFLAGS) -c abierta.cpp

# compilamos las operaciones de la cerrada
cerrada.o: cerrada.cpp cerrada.hpp
	$(CXX) $(CXXFLAGS) -c cerrada.cpp

# borramos los objetos y el ejecutable
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean