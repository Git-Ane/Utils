# Spécifiez le compilateur
CC=g++

# Spécifiez les options de compilation
CFLAGS=-std=c++14 -Wall -Iinclude

# Nom de votre exécutable
TARGET=build/GitAneConsole

# Chemins des fichiers source
SRC=console.cpp commands.cpp Plugins/plugin_loader.cpp Plugins/joke/joke.cpp

# Chemins des fichiers objet (dans le dossier build)
OBJ=$(SRC:%.cpp=build/%.o)

# Création du répertoire build si nécessaire
$(shell mkdir -p build)
$(shell mkdir -p build/include)
$(shell mkdir -p build/Plugins)
$(shell mkdir -p build/Plugins/joke)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

build/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build
