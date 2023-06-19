// cd KONGCE
// javac Servidor/DonCEyKongJrServer.java
// java Servidor.DonCEyKongJrServer

package Servidor;

// Importa las clases y paquetes necesarios
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.Scanner;


// Clase principal que representa el servidor del juego
public class DonCEyKongJrServer {

    public static final int NUMERO_LIANAS = 6;
    public static final int ALTURA_MAXIMA_LIANA = 8;
    
    public static List<Cocodrilo> cocodrilos;
    public static List<Fruta> frutas;

    private List<Integer> lianasConFruta = new ArrayList<>();
    private List<Integer> lianasConCocodrilo = new ArrayList<>();


    
    public DonCEyKongJrServer() {
        cocodrilos = new ArrayList<>();
        frutas = new ArrayList<>();
    }

    // Método para agregar un cocodrilo al juego
    public void agregarCocodrilo(int liana, int altura, boolean esRojo) {
        Cocodrilo cocodrilo = new Cocodrilo(liana, altura, esRojo);
        cocodrilos.add(cocodrilo);
    }

    // Método para agregar una fruta al juego
    public void agregarFruta(int liana, int altura, int puntos) {
        // Verificar si ya existe una fruta en la misma posición
        for (Fruta frutaExistente : frutas) {
            if (frutaExistente.liana == liana && frutaExistente.altura == altura) {
                System.out.println("Ya existe una fruta en esa posición. No se puede agregar otra fruta en el mismo lugar.");
                return; // Salir del método sin agregar la fruta
            }
        }

        Fruta fruta = new Fruta(liana, altura, puntos);
        frutas.add(fruta);
    }

    // Método para eliminar una fruta del juego
    public void eliminarFruta(int liana) {
        for (int i = 0; i < frutas.size(); i++) {
            Fruta fruta = frutas.get(i);
            if (fruta.liana == liana) {
                frutas.remove(i);
                break;
            }
        }
    }

    // Método para que el administrador elimine una fruta
    public void eliminarFrutaDesdeConsola() {
        Scanner scanner = new Scanner(System.in);

        System.out.println("-------------------");
        System.out.println();
        System.out.println("Ingrese la liana de la cual desea eliminar una fruta:");
        int liana = scanner.nextInt();

        eliminarFruta(liana);

        int x = 0;
        if (x == 1) {
            scanner.close();
        }
    }

    // Método para verificar si un cocodrilo alcanza a Donkey Kong Jr
    public boolean cocodriloAlcanzaJr() {
        // Lógica para verificar la colisión entre los cocodrilos y Donkey Kong Jr
        // Retorna true si hay colisión, false en caso contrario
        return false;
    }

    // Método para verificar si Donkey Kong Jr cae al abismo
    public boolean jrCaeAlAbismo() {
        // Lógica para verificar si Donkey Kong Jr cae al abismo
        // Retorna true si cae al abismo, false en caso contrario
        return false;
    }

    // Método para generar una fruta aleatoria
    public void generarFrutaAleatoria() {
        Random random = new Random();
        int liana = random.nextInt(NUMERO_LIANAS) + 1;
        int altura = random.nextInt(ALTURA_MAXIMA_LIANA) + 1;
        int puntos = random.nextInt(10) + 1;

        // Verificar si ya hay una fruta en la liana generada
        boolean frutaExistente = false;
        for (Fruta fruta : frutas) {
            if (fruta.liana == liana) {
                frutaExistente = true;
                break;
            }
        }

        // Repetir el proceso hasta encontrar una liana sin fruta
        while (frutaExistente) {
            liana = random.nextInt(NUMERO_LIANAS) + 1;
            frutaExistente = false;
            for (Fruta fruta : frutas) {
                if (fruta.liana == liana) {
                    frutaExistente = true;
                    break;
                }
            }
        }

        agregarFruta(liana, altura, puntos);
        lianasConFruta.add(liana);
    }

    // Método para generar los cocodrilos rojos al inicio de la partida
    public void generarCocodrilosRojosIniciales() {
        Random random = new Random();
        for (int liana = 1; liana <= NUMERO_LIANAS; liana++) {
            if (random.nextDouble() <= 0.5) {
                int altura = random.nextInt(DonCEyKongJrServer.ALTURA_MAXIMA_LIANA) + 1;
                agregarCocodrilo(liana, altura, true);
                lianasConCocodrilo.add(liana);
            }
        }
    }

    // Método para agregar que el administrador agregue un cocodrilo
    public void agregarCocodriloDesdeConsola(){
        Scanner scanner = new Scanner(System.in);

        System.out.println("-------------------");
        System.out.println();
        System.out.println("Ingrese la liana donde desea agregar el cocodrilo:");
        int liana = scanner.nextInt();

        System.out.println("Ingrese la altura donde desea agregar el cocodrilo:");
        int altura = scanner.nextInt();

        System.out.println("Ingrese el color del cocodrilo (r: rojo / a: azul):");
        String colorInput = scanner.next();

        boolean esRojo;
        if (colorInput.equalsIgnoreCase("r")) {
            esRojo = true;
        } else if (colorInput.equalsIgnoreCase("a")) {
            esRojo = false;
        } else {
            System.out.println("Opción inválida. Se asumirá color azul por defecto.");
            esRojo = false;
        }

        agregarCocodrilo(liana, altura, esRojo);
        iniciarMovimientoCocodrilos();

        int x = 0;
        if (x == 1){
            scanner.close();
        }
    }

    // Método para iniciar el movimiento de los cocodrilos
    public void iniciarMovimientoCocodrilos() {
        
        for (Cocodrilo cocodrilo : cocodrilos) {
            // Verificar si el cocodrilo ya se está moviendo
            if (cocodrilo.moviendose) {
                continue; // Pasar al siguiente cocodrilo
            }
            
            Thread movimientoCocodriloThread = new MovimientoCocodriloThread(cocodrilo);
            movimientoCocodriloThread.start();
        }
    }

    // Método que pregunta al usuario que accion desea realizar
    public void pregunta(){

        System.out.println("-------------------");
        System.out.println();
        System.out.println("ingrese una accion a realizar");
        System.out.println("1. agregar un cocodrilo");
        System.out.println("2. eliminar fruta ");
        System.out.println("0. finalizar");
    }

    // Método que retorna las frutas y cocodrilos actuales
    public void datosActuales(){

        System.out.println("-------------------");
        System.out.println();
        System.out.println("Frutas actuales:");
        System.out.println();
        for (Fruta fruta : frutas) {
            System.out.println("Fruta en liana: " + fruta.liana + ", altura: " + fruta.altura + ", puntos: " + fruta.puntos);
        }
        System.out.println();
        System.out.println("Cocodrilos actuales:");
        System.out.println();
        for (Cocodrilo cocodrilo : cocodrilos) {
            System.out.println("Cocodrilo en liana: " + cocodrilo.liana + ", altura: " + cocodrilo.altura + ", color: " + (cocodrilo.esRojo ? "rojo" : "azul"));
        }
    }

    // Método principal para ejecutar el servidor y probar la lógica del juego
    public static void main(String[] args) {

        DonCEyKongJrServer servidor = new DonCEyKongJrServer();

        

        // Agregar ejemplos de cocodrilos y frutas
        servidor.generarCocodrilosRojosIniciales();
        for (int i = 1; i <= NUMERO_LIANAS; i++) {
            servidor.generarFrutaAleatoria();
        }

        servidor.datosActuales();

        // Iniciar movimiento de los cocodrilos
        servidor.iniciarMovimientoCocodrilos();

        // Iniciar generación de cocodrilos azules
        Thread generadorAzules = new Thread(new GeneradorCocodrilosAzules(servidor));
        generadorAzules.start();

        // Solicitar al administrador agregar un cocodrilo o eliminar fruta
        Scanner scanner = new Scanner(System.in);
        servidor.pregunta();

        int accion;

        do {
            accion = scanner.nextInt();

            switch (accion) {
                case 1:
                    servidor.agregarCocodriloDesdeConsola();
                    servidor.datosActuales();
                    servidor.pregunta();
                    break;
                case 2:
                    servidor.eliminarFrutaDesdeConsola();
                    servidor.datosActuales();
                    servidor.pregunta();
                    break;
                case 0:
                    break;
                default:
                    System.out.println("Entrada inválida. Por favor, ingrese una acción válida.");
                    servidor.pregunta();
                    break;
            }
        } while (accion != 0);
        scanner.close();
    }
}
