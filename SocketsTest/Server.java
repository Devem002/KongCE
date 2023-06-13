// javac Server.java
// java Server

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {
    public static void main(String[] args) {
        int port = 12345;  // Puerto en el que el servidor escucha las conexiones entrantes

        try {
            // Crear el socket del servidor
            ServerSocket serverSocket = new ServerSocket(port);

            System.out.println("Servidor iniciado. Esperando conexiones...");

            while (true) {
                // Esperar una conexión entrante
                Socket clientSocket = serverSocket.accept();
                System.out.println("Cliente conectado desde: " + clientSocket.getInetAddress().getHostAddress());

                // Aquí se maneja la comunicación con el cliente
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
