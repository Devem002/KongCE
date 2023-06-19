package Servidor;


    // Clase interna que representa un hilo para el movimiento de un cocodrilo
    public class MovimientoCocodriloThread extends Thread {
        private Elemento elemento;

        public MovimientoCocodriloThread(Elemento elemento) {
            this.elemento = elemento;
        }

        @Override
        public void run() {


            if (elemento instanceof Cocodrilo) {
                Cocodrilo cocodrilo = (Cocodrilo) elemento;

                cocodrilo.moviendose = true; // Marcar el cocodrilo como moviéndose


                while (true) {
                    if (cocodrilo.esRojo) {

                        if (cocodrilo.altura == DonCEyKongJrServer.ALTURA_MAXIMA_LIANA) {
                            // Si el cocodrilo está en la altura máxima, baja paso a paso hasta la altura mínima
                            while (cocodrilo.altura > 1) {
                                cocodrilo.altura--;

                                // Imprimir detalles del movimiento
                                //System.out.println("Cocodrilo Rojo en liana: " + cocodrilo.liana + ", altura: " + cocodrilo.altura);/////////////

                                // Realizar alguna acción correspondiente al movimiento, como actualizar la interfaz gráfica
                                try {
                                    Thread.sleep(2000); // velocidad de los cocodrilos 
                                } catch (InterruptedException e) {
                                    e.printStackTrace();
                                }
                            }
                        } else {
                            // Si el cocodrilo no está en la altura máxima, sube paso a paso hasta la altura máxima
                            while (cocodrilo.altura < DonCEyKongJrServer.ALTURA_MAXIMA_LIANA) {
                                cocodrilo.altura++;

                                // Imprimir detalles del movimiento
                                //System.out.println("Cocodrilo Rojo en liana: " + cocodrilo.liana + ", altura: " + cocodrilo.altura);///////////////////////

                                // Realizar alguna acción correspondiente al movimiento, como actualizar la interfaz gráfica
                                try {
                                    Thread.sleep(2000); // velocidad de los cocodrilos 
                                } catch (InterruptedException e) {
                                    e.printStackTrace();
                                }
                            }
                        }
                        
                    } else {

                        // Lógica de movimiento para cocodrilos azules
                        while (cocodrilo.altura < DonCEyKongJrServer.ALTURA_MAXIMA_LIANA) {
                            cocodrilo.altura++;

                            // Imprimir detalles del movimiento
                            //System.out.println("Cocodrilo Azul en liana: " + cocodrilo.liana + ", altura: " + cocodrilo.altura);/////////////

                            // Realizar alguna acción correspondiente al movimiento, como actualizar la interfaz gráfica
                            try {
                                Thread.sleep(2000); // velocidad de los cocodrilos 
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }
                        }

                        // El cocodrilo azul ha alcanzado la altura máxima y cae, siendo eliminado
                        DonCEyKongJrServer.cocodrilos.remove(cocodrilo);
                        break; // Detener el hilo al eliminar el cocodrilo
                    }

                    try {
                        Thread.sleep(2000); // velocidad de los cocodrilos 
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                cocodrilo.moviendose = false; // Marcar el cocodrilo como no moviéndose al finalizar el hilo
            }
        }
    }