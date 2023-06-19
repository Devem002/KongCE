// gcc cliente.c -o client -lws2_32
// ./client

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddress;
    char serverIP[] = "127.0.0.1";  // Dirección IP del servidor
    int serverPort = 12345;  // Puerto del servidor

    // Inicializar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Error al inicializar Winsock.\n");
        return 1;
    }

    // Crear el socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        printf("Error al crear el socket.\n");
        WSACleanup();
        return 1;
    }

    // Configurar la dirección del servidor
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(serverIP);
    serverAddress.sin_port = htons(serverPort);

    // Conectar al servidor
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        printf("Error al conectar al servidor.\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Aquí va la logica para enviar y recibir datos a través del socket

    // Cerrar la conexión
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
