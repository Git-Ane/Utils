# Spécifiez le compilateur
CC=g++

# Spécifiez les options de compilation
CFLAGS=-std=c++17 -Wall -Iinclude

# Nom de votre exécutable
TARGET=build/GitAneConsole

# Chemins des fichiers source
SRC=Console/console.cpp Console/commands.cpp Console/Plugins/plugin_loader.cpp Console/Plugins/joke/joke.cpp Console/Plugins/debug/debug.cpp Git/classes.cpp SHA-warma/shawarma.cpp

# Chemins des fichiers objet (dans le dossier build)
OBJ=$(SRC:%.cpp=build/%.o)

# Création du répertoire build si nécessaire
$(shell mkdir -p build)
$(shell mkdir -p build/Console)
$(shell mkdir -p build/include)
$(shell mkdir -p build/Console/Plugins)
$(shell mkdir -p build/Console/Plugins/joke)
$(shell mkdir -p build/Console/Plugins/debug)
$(shell mkdir -p build/Git)
$(shell mkdir -p build/SHA-warma)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) -lssl -lcrypto -lz

build/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -lssl -lcrytpo -lz

clean:
	rm -rf build
