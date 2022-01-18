import java.util.Arrays;
import java.util.Random;

class Soma {

    private int soma;

    public Soma() {
        this.soma = 0;
    }

    public synchronized void incrementaSoma(int valor) {
        this.soma += valor;
    }

    public synchronized int getSoma() {
        return this.soma;
    }

}

class Task extends Thread {
    // Identificador da thread
    private int id;
    Soma soma;
    private int[] vetor;

    // Construtor
    public Task(int tid, Soma soma, int[] vetor) {
        this.id = tid;
        this.soma = soma;
        this.vetor = vetor;
    }

    // Metodo main da thread
    public void run() {
        System.out.println("Thread " + this.id + " iniciou!");
        for (int j : vetor) {
            this.soma.incrementaSoma(j);
        }
        System.out.println("Thread " + this.id + " terminou!");
    }
}

//classe da aplicacao
class Main {

    static final int N = 50;
    static final int tamVetor = 100;

    public static void main (String[] args) {

        // Cria o vetor de threads e o vetor de valores para serem somados
        Thread[] threads = new Thread[N];
        int[] vetor = new int[tamVetor];

        // Cria variáveis auxiliares para distribuir carga
        int start = 0, end = 0;

        // Cria uma instancia do recurso compartilhado entre as threads
        Soma soma = new Soma();
        PreencherVetor(vetor);

        // Cria as threads da aplicacao
        for (int i = 0; i < threads.length; i++) {
            start = i * ((tamVetor) / N);
            end = (i * ((tamVetor) / N)) + ((tamVetor) / N);
            threads[i] = new Task(i, soma, Arrays.copyOfRange(vetor, start, end));
        }

        // Inicia as threads
        for (int i = 0; i < threads.length; i++) {
            threads[i].start();
        }

        // Espera pelo termino de todas as threads
        for (int i = 0; i < threads.length; i++) {
            try { threads[i].join(); } catch (InterruptedException e) { return; }
        }

        // Checa a corretude do programa
        if(ChecarCorretude(vetor, soma.getSoma())) {
            System.out.println("O cálculo concorrente está correto.");
        } else {
            System.out.println("O cálculo concorrente está errado.");
        }
        System.out.println("Valor final da soma calculada é = " + soma.getSoma());
    }

    private static void PreencherVetor(int[] vetor) {

        Random r = new Random();
        for(int i = 0; i < vetor.length; i++) {
            vetor[i] = r.nextInt(1000) + 1;
        }
    }

    private static boolean ChecarCorretude(int[] vetor, int somaCalculada) {
        int somaLocal = 0;

        for (int j : vetor) {
            somaLocal += j;
        }

        return somaLocal == somaCalculada;
    }
}
