# Spécifiez le compilateur
CC=g++

# Spécifiez les options de compilation
CFLAGS=-std=c++14 -Wall -Iinclude

# Nom de votre exécutable
TARGET=build/GitAneConsole

# Chemins des fichiers source
SRC=console.cpp

# Chemins des fichiers objet (dans le dossier build)
OBJ=$(SRC:%.cpp=build/%.o)

# Création du répertoire build si nécessaire
$(shell mkdir -p build)
$(shell mkdir -p build/include)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

build/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build