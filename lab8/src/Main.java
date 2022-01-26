// Monitor que implementa a logica do padrao leitores/escritores
class LE {
    private int leit, escr;

    // Construtor
    LE() {
        this.leit = 0; //leitores lendo (0 ou mais)
        this.escr = 0; //escritor escrevendo (0 ou 1)
    }

    // Entrada para leitores
    public synchronized void EntraLeitor(int id) {
        try {
            while (this.escr > 0) {
                //if (this.escr > 0) {
                System.out.println("le.leitorBloqueado(" + id + ")");
                wait();  //bloqueia pela condicao logica da aplicacao
            }
            this.leit++;  //registra que ha mais um leitor lendo
            System.out.println("le.leitorLendo(" + id + ")");
        } catch (InterruptedException e) {
        }
    }

    // Saida para leitores
    public synchronized void SaiLeitor(int id) {
        this.leit--; //registra que um leitor saiu
        if (this.leit == 0)
            this.notify(); //libera escritor (caso exista escritor bloqueado)
        System.out.println("le.leitorSaindo(" + id + ")");
    }

    // Entrada para escritores
    public synchronized void EntraEscritor(int id) {
        try {
            while ((this.leit > 0) || (this.escr > 0)) {
                //if ((this.leit > 0) || (this.escr > 0)) {
                System.out.println("le.escritorBloqueado(" + id + ")");
                wait();  //bloqueia pela condicao logica da aplicacao
            }
            this.escr++; //registra que ha um escritor escrevendo
            System.out.println("le.escritorEscrevendo(" + id + ")");
        } catch (InterruptedException e) {
        }
    }

    // Saida para escritores
    public synchronized void SaiEscritor(int id) {
        this.escr--; //registra que o escritor saiu
        notifyAll(); //libera leitores e escritores (caso existam leitores ou escritores bloqueados)
        System.out.println("le.escritorSaindo(" + id + ")");
    }
}


// Escritor
class LeitorEscritor extends Thread {
    int id; //identificador da thread
    LE monitor; //objeto monitor para coordenar a lógica de execução das threads
    int delay;
    BancoDeDados bancoDeDados;

    // Construtor
    LeitorEscritor(int id, int delay, LE m, BancoDeDados bancoDeDados) {
        this.id = id;
        this.monitor = m;
        this.delay = delay;
        this.bancoDeDados = bancoDeDados;
    }

    // Método executado pela thread
    public void run() {
        double j = 77777777777777.7;
        while (true) {
            this.monitor.EntraLeitor(this.id);
            System.out.println("A variável no banco de dados é de valor " + bancoDeDados.getBancoDeDados());
            for (int i = 0; i < 100000000; i++) {
                j = j / 2;
            } //...loop bobo para simbolizar o tempo de leitura
            this.monitor.SaiLeitor(this.id);
            try {
                sleep(this.delay);
            } catch (InterruptedException e) {
                return;
            }
            this.monitor.EntraEscritor(this.id);
            bancoDeDados.incrementaBancoDeDados(1);
            this.monitor.SaiEscritor(this.id);
            try {
                sleep(this.delay);
            } catch (InterruptedException e) {
                return;
            }
        }
    }
}

// Leitor
class Leitor extends Thread {
    int id; //identificador da thread
    int delay; //atraso bobo
    LE monitor; //objeto monitor para coordenar a lógica de execução das threads
    BancoDeDados bancoDeDados;

    // Construtor
    Leitor(int id, int delayTime, LE m, BancoDeDados bancoDeDados) {
        this.id = id;
        this.delay = delayTime;
        this.monitor = m;
        this.bancoDeDados = bancoDeDados;
    }

    // Método executado pela thread
    public void run() {
        while (true) {
            this.monitor.EntraLeitor(this.id);
            System.out.println("A variável no banco de dados é um valor " + (this.bancoDeDados.getBancoDeDados()%2 == 0 ? "par" : "ímpar"));
            this.monitor.SaiLeitor(this.id);
            try {
                sleep(this.delay);
            } catch (InterruptedException e) {
                return;
            }
        }
    }
}

//--------------------------------------------------------
// Escritor
class Escritor extends Thread {
    int id; //identificador da thread
    int delay; //atraso bobo...
    LE monitor; //objeto monitor para coordenar a lógica de execução das threads
    BancoDeDados bancoDeDados;

    // Construtor
    Escritor(int id, int delayTime, LE m, BancoDeDados bancoDeDados) {
        this.id = id;
        this.delay = delayTime;
        this.monitor = m;
        this.bancoDeDados = bancoDeDados;
    }

    // Método executado pela thread
    public void run() {
        while (true) {
            this.monitor.EntraEscritor(this.id);
            this.bancoDeDados.setBancoDeDados(id);
            this.monitor.SaiEscritor(this.id);
            try {
                sleep(this.delay);
            } catch (InterruptedException e) {
                return;
            }
        }
    }
}

// Classe da variávle compartilhada entre as Threads
class BancoDeDados {

    private int bancoDeDados;

    public BancoDeDados() {
        this.bancoDeDados = 0;
    }

    public synchronized void incrementaBancoDeDados(int valor) {
        this.bancoDeDados += valor;
    }

    public synchronized int getBancoDeDados() {
        return this.bancoDeDados;
    }

    public synchronized void setBancoDeDados(int valor) {
        this.bancoDeDados = valor;
    }

}

//--------------------------------------------------------
// Classe principal
class Main {
    static final int L = 4;
    static final int E = 3;
    static final int LE = 5;

    public static void main(String[] args) {
        BancoDeDados bancoDeDados = new BancoDeDados();
        LE monitor = new LE();            // Monitor (objeto compartilhado entre leitores e escritores)
        Leitor[] l = new Leitor[L];       // Threads leitores
        Escritor[] e = new Escritor[E];   // Threads escritores
        LeitorEscritor[] le = new LeitorEscritor[LE];   // Threads leitores e escritores

        //inicia o log de saida
        System.out.println("import verificaLE");
        System.out.println("le = verificaLE.LE()");

        for (int i = 0; i < L; i++) {
            l[i] = new Leitor(i + 1, (i + 1) * 500, monitor, bancoDeDados);
            l[i].start();
        }
        for (int i= 0;i<E ;i++){
            e[i] = new Escritor(i + 1, (i + 1) * 500, monitor, bancoDeDados);
            e[i].start();
        }
        for (int i= 0;i<LE ;i++){
            le[i] = new LeitorEscritor(i + 1, (i + 1) * 500, monitor, bancoDeDados);
            le[i].start();
        }
    }
}