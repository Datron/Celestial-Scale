CXX = g++
CXXFLAGS = -g -lGL -lGLU -lglut

journey: main.cpp
	$(CXX) -o journey main.cpp $(CXXFLAGS)