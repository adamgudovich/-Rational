CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic
TARGET := rational
OBJECTS := main.o Rational.o

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

main.o: main.cpp Rational.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

Rational.o: Rational.cpp Rational.h
	$(CXX) $(CXXFLAGS) -c Rational.cpp -o Rational.o

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)
