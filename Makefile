# Spécifiez le compilateur
CC=g++

# Spécifiez les options de compilation
CFLAGS=-std=c++17 -Wall -Iinclude

# Nom de votre exécutable
TARGET=build/GitAneConsole

# Chemins des fichiers source
SRC=Console/console.cpp Console/commands.cpp Plugins/plugin_loader.cpp Plugins/joke/joke.cpp Plugins/merge/merge.cpp Plugins/debug/debug.cpp Plugins/client/client.cpp Plugins/client/gitting.cpp Git/classes.cpp SHA-warma/shawarma.cpp Net/client.cpp

# Chemins des fichiers objet (dans le dossier build)
OBJ=$(SRC:%.cpp=build/%.o)

# Création du répertoire build si nécessaire
$(shell mkdir -p build)
$(shell mkdir -p build/Console)
$(shell mkdir -p build/include)
$(shell mkdir -p build/Plugins)
$(shell mkdir -p build/Plugins/joke)
$(shell mkdir -p build/Plugins/debug)
$(shell mkdir -p build/Plugins/merge)
$(shell mkdir -p build/Plugins/client/merge)
$(shell mkdir -p build/Git)
$(shell mkdir -p build/Net)
$(shell mkdir -p build/SHA-warma)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) -lssl -lcrypto -lz -lstdc++ -lcurl

build/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -lssl -lcrytpo -lz -lstdc++ -lcurl

clean:
	rm -rf build
