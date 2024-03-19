# Spécifiez le compilateur
CC=g++

# Spécifiez les options de compilation
CFLAGS=-std=c++17 -Wall -Iinclude

# Nom de votre exécutable
TARGET=build/GitAneConsole

# Chemins des fichiers source
SRC=Console/console.cpp Console/commands.cpp Console/Plugins/plugin_loader.cpp Console/Plugins/joke/joke.cpp Git/classes.cpp

# Chemins des fichiers objet (dans le dossier build)
OBJ=$(SRC:%.cpp=build/%.o)

# Création du répertoire build si nécessaire
$(shell mkdir -p build)
$(shell mkdir -p build/Console)
$(shell mkdir -p build/include)
$(shell mkdir -p build/Console/Plugins)
$(shell mkdir -p build/Console/Plugins/joke)
$(shell mkdir -p build/Git)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

build/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build
