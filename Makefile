CXX = g++
CXXFLAGS = -g -lGL -lGLU -lglut -lsqlite3
# SQLFLAGS = -lsqlite3
journey: main.cpp Planet.o
	$(CXX) -o journey main.cpp Planet.o $(CXXFLAGS)

Planet.o: Planet.h