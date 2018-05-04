CXX = g++
CXXFLAGS = -g -lsqlite3 -lSDL -lGL -lGLU -lglut

journey: main.cpp
	$(CXX) -o journey main.cpp $(CXXFLAGS)