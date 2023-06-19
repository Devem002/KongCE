package Servidor;

// Clase que representa una fruta
public class Fruta extends Elemento {
    public int puntos;

    public Fruta(int liana, int altura, int puntos) {
        super(liana, altura);
        this.puntos = puntos;
    }
}