package Servidor;
import java.util.Random;

    // Clase interna para generar cocodrilos azules periódicamente
    public class GeneradorCocodrilosAzules implements Runnable {

        private DonCEyKongJrServer servidor;

        public GeneradorCocodrilosAzules(DonCEyKongJrServer servidor) {
            this.servidor = servidor;
        }

        @Override
        public void run() {
            Random random = new Random();
            while (!Thread.currentThread().isInterrupted()) {
                int liana = random.nextInt(DonCEyKongJrServer.NUMERO_LIANAS) + 1;
                servidor.agregarCocodrilo(liana, 1, false);
                servidor.iniciarMovimientoCocodrilos();
                try {
                    Thread.sleep(5000);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }