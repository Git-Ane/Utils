# Spécifier le compilateur
CC=g++

# Spécifier les options de compilation
CFLAGS=-std=c++17 -Wall -Iinclude

# Nom de l'exécutable
TARGET=build/GitAneServer

# Chemins des fichiers source
SRC=server.cpp include/http_tcpServer_linux.cpp include/url_tree.cpp include/http_subsystems/file_transfer.cpp include/http_subsystems/response_builder.cpp include/http_subsystems/data_manipulations.cpp include/http_subsystems/login_and_register.cpp

# Chemins des fichiers objet (dans le dossier build)
OBJ=$(SRC:%.cpp=build/%.o)

# Création du répertoire build si nécessaire
$(shell mkdir -p build)
$(shell mkdir -p build/include)
$(shell mkdir -p build/include/http_subsystems)
$(shell mkdir -p build/include/http_subsystems)
$(shell g++ client.cpp -std=c++17 -c -o client.o -lssl -lcrypto -lz -lstdc++ -lcurl)
$(shell g++ test_client.cpp -std=c++17 -c -o test_client.o -lssl -lcrypto -lz -lstdc++ -lcurl)
$(shell g++ test_client.o client.o ./build/include/http_subsystems/data_manipulations.o -std=c++17 -lssl -lcrypto -lz -lstdc++ -lcurl -o test_client)
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) -lssl -lcrypto -lz -lstdc++ -lcurl

build/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -lssl -lcrypto -lz -lstdc++ -lcurl

clean:
	rm -rf build
