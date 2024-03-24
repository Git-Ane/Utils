# Spécifiez le compilateur
CC=g++

# Spécifiez les options de compilation
CFLAGS=-std=c++14 -Wall -Iinclude

# Nom de votre exécutable
TARGET=build/GitAneServer

# Chemins des fichiers source
SRC=server.cpp include/http_tcpServer_linux.cpp include/url_tree.cpp

# Chemins des fichiers objet (dans le dossier build)
OBJ=$(SRC:%.cpp=build/%.o)

# Création du répertoire build si nécessaire
$(shell mkdir -p build)
$(shell mkdir -p build/include)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) -lssl -lcrypto -lz

build/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -lssl -lcrypto -lz

clean:
	rm -rf build
