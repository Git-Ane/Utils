# Spécifiez le compilateur
CC=g++

# Spécifiez les options de compilation
CFLAGS=-std=c++17 -Wall -Iinclude

# Nom de votre exécutable
TARGET=build/GitAneServer

# Chemins des fichiers source
SRC=server.cpp include/http_tcpServer_linux.cpp include/url_tree.cpp include/http_subsystems/file_transfer.cpp include/http_subsystems/response_builder.cpp include/http_subsystems/data_manipulations.cpp include/http_subsystems/login_and_register.cpp

# Chemins des fichiers objet (dans le dossier build)
OBJ=$(SRC:%.cpp=build/%.o)

# Création du répertoire build si nécessaire
$(shell mkdir -p build)
$(shell mkdir -p build/include)
$(shell mkdir -p build/include/http_subsystems)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) -lssl -lcrypto -lz -lstdc++

build/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -lssl -lcrypto -lz -lstdc++

clean:
	rm -rf build
