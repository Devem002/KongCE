package Servidor;

// Clase que representa un cocodrilo
public class Cocodrilo extends Elemento {
    public boolean esRojo;
    public boolean moviendose;

    public Cocodrilo(int liana, int altura, boolean esRojo) {
        super(liana, altura);
        this.esRojo = esRojo;
        this.moviendose = false;
    }
}