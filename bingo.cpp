#include <iostream>
#include <vector>
#include <deque> //Permite usar a estrutura deque (double-ended queue) 
#include <algorithm> //find() random_shuffle()
#include <cstdlib>
#include <ctime>
#include <iomanip> //Permite manipular formatação de saída setw()

// Compatibilidade multiplataforma para sleep
#ifdef _WIN32
    #include <windows.h>
    #define SLEEP(x) Sleep(x * 1000)
#else
    #include <unistd.h>
    #define SLEEP(x) sleep(x)
#endif

using namespace std;

// Códigos ANSI para cores no terminal
const string RESET = "\033[0m";
const string VERDE = "\033[1;32m";
const string AMARELO = "\033[1;33m";
const string AZUL = "\033[1;34m";
const string VERMELHO = "\033[1;31m";
const string CIANO = "\033[1;36m";
const string BRANCO = "\033[1;37m";

// Limpa terminal (Windows: cls / Linux: clear)
void limparEcra() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Representa a cartela completa (1-100)
class Cartela {
private:
    vector<int> numeros; // Todos os números de 1 a 100

public:
    // Construtor: preenche vetor 1-100
    Cartela() {
        for (int i = 1; i <= 100; i++) {
            numeros.push_back(i);
        }
    }

    // Verifica se número está na lista de sorteados
    bool foiSorteado(int numero, const vector<int>& sorteados) {
        return find(sorteados.begin(), sorteados.end(), numero) != sorteados.end();
    }

    // Retorna letra BINGO conforme intervalo do número
    string obterLetraColuna(int numero) {
        if (numero >= 1 && numero <= 20) return "B";
        if (numero >= 21 && numero <= 40) return "I";
        if (numero >= 41 && numero <= 60) return "N";
        if (numero >= 61 && numero <= 80) return "G";
        if (numero >= 81 && numero <= 100) return "O";
        return "";
    }

    // Mostra cartela 5x20: sorteados em vermelho, não sorteados em verde
    void exibir(const vector<int>& sorteados) {
        cout << "\n" << CIANO << "========== CARTELA DE BINGO ==========\n" << RESET;
        cout << "  B        I        N        G        O\n";
        cout << "--------------------------------------\n";

        // 20 linhas, 5 colunas (B:1-20, I:21-40, N:41-60, G:61-80, O:81-100)
        for (int linha = 0; linha < 20; linha++) {
            int colunas[5] = {linha + 1, linha + 21, linha + 41, linha + 61, linha + 81};
            for (int c = 0; c < 5; c++) {
                if (foiSorteado(colunas[c], sorteados)) {
                    cout << VERMELHO << setw(3) << colunas[c] << RESET << "      ";
                } else {
                    cout << VERDE << setw(3) << colunas[c] << RESET << "      ";
                }
            }
            cout << "\n";
        }
        cout << "--------------------------------------\n";
    }
};

// Controla lógica do jogo
class Bingo {
private:
    Cartela cartela;
    vector<int> todosSorteados; // Ordem embaralhada dos sorteios
    deque<int> ultimas5; // Histórico dos últimos 5 números
    int bolaAnterior; // Guarda número anterior ao atual

public:
    // Inicializa e embaralha ordem de sorteio
    Bingo() {
        bolaAnterior = -1;
        // Preenche 1-100
        for (int i = 1; i <= 100; i++) {
            todosSorteados.push_back(i);
        }
        srand(time(NULL)); // Semente aleatória
        random_shuffle(todosSorteados.begin(), todosSorteados.end()); // Embaralha
        ultimas5.clear();
    }

    // ASCII art do título
    void exibirTitulo() {
        cout << AZUL;
        cout << "     ██╗ ██████╗  ██████╗  ██████╗     ██████╗  ██████╗     ██████╗ ██╗███╗   ██╗ ██████╗  ██████╗ \n";
        cout << "     ██║██╔═══██╗██╔════╝ ██╔═══██╗    ██╔══██╗██╔═══██╗    ██╔══██╗██║████╗  ██║██╔════╝ ██╔═══██╗\n";
        cout << "     ██║██║   ██║██║  ███╗██║   ██║    ██║  ██║██║   ██║    ██████╔╝██║██╔██╗ ██║██║  ███╗██║   ██║\n";
        cout << "██   ██║██║   ██║██║   ██║██║   ██║    ██║  ██║██║   ██║    ██╔══██╗██║██║╚██╗██║██║   ██║██║   ██║\n";
        cout << "╚█████╔╝╚██████╔╝╚██████╔╝╚██████╔╝    ██████╔╝╚██████╔╝    ██████╔╝██║██║ ╚████║╚██████╔╝╚██████╔╝\n";
        cout << RESET << "\n";
    }

    // Wrapper para obter letra do número
    string obterLetraColuna(int numero) {
        return cartela.obterLetraColuna(numero);
    }

    // Atualiza tela com bola atual e histórico
    void exibirInformacoes(int bolaAtual, const vector<int>& numerosSorteados) {
        limparEcra();
        exibirTitulo();

        // Mostra número atual em destaque
        cout << "\n" << VERMELHO << "===================================\n";
        cout << "   NUMERO SORTEADO: " << obterLetraColuna(bolaAtual) << "-";
        cout << setw(3) << bolaAtual << "        \n";
        cout << "===================================\n" << RESET;

        // Mostra bola anterior
        if (bolaAnterior != -1) {
            cout << AMARELO << "Ultimo numero sorteado: " 
                 << obterLetraColuna(bolaAnterior) << "-" << bolaAnterior << RESET << "\n";
        }

        // Mostra últimos 5 sorteados
        if (!ultimas5.empty()) {
            cout << AZUL << "Ultimos 5 numeros sorteados: " << RESET;
            for (size_t i = 0; i < ultimas5.size(); i++) {
                cout << obterLetraColuna(ultimas5[i]) << "-" << ultimas5[i];
                if (i < ultimas5.size() - 1) cout << ", ";
            }
            cout << "\n";
        }

        // Exibe cartela colorida
        cartela.exibir(numerosSorteados);
        cout << CIANO << "Total de numeros sorteados: " << numerosSorteados.size() << "/100" << RESET << "\n";
    }

    // Modo automático: sorteia a cada 2 segundos
    void jogarAutomatico() {
        cout << CIANO << "Modo Automatico ativado! Numeros a cada 2 segundos.\n" << RESET;
        SLEEP(2);

        vector<int> numerosSorteados; // Acumula sorteados para exibição

        for (size_t i = 0; i < todosSorteados.size(); i++) {
            int numero = todosSorteados[i];

            // Atualiza deque dos últimos 5
            ultimas5.push_back(numero);
            if (ultimas5.size() > 5) ultimas5.pop_front();

            numerosSorteados.push_back(numero); // Adiciona aos sorteados

            exibirInformacoes(numero, numerosSorteados);
            bolaAnterior = numero; // Guarda para próxima iteração
            SLEEP(2); // Pausa 2 segundos
        }

        cout << VERMELHO << "Todos os numeros foram sorteados! Jogo terminado.\n" << RESET;
    }

    // Modo manual: sorteia ao pressionar Enter
    void jogarManual() {
        cout << CIANO << "Modo Manual ativado! Prima Enter para sortear cada numero.\n" << RESET;
        SLEEP(1);

        vector<int> numerosSorteados;

        for (size_t i = 0; i < todosSorteados.size(); i++) {
            cout << "\nPrima Enter para sortear o proximo numero...";
            cin.get(); // Aguarda Enter

            int numero = todosSorteados[i];

            // Atualiza histórico dos últimos 5
            ultimas5.push_back(numero);
            if (ultimas5.size() > 5) ultimas5.pop_front();

            numerosSorteados.push_back(numero);

            exibirInformacoes(numero, numerosSorteados);
            bolaAnterior = numero;
        }

        cout << VERMELHO << "Todos os numeros foram sorteados! Jogo terminado.\n" << RESET;
    }
};

// Ponto de entrada: escolhe modo e inicia jogo
int main() {
    Bingo bingo;
    bingo.exibirTitulo();

    cout << "\nEscolha o modo de jogo:\n";
    cout << "1 - Modo Automatico (sorteio a cada 2 segundos)\n";
    cout << "2 - Modo Manual (Enter para cada sorteio)\n";
    cout << "Opcao: ";

    int opcao;
    cin >> opcao;
    cin.ignore(); // Limpa buffer do Enter

    if (opcao == 1) {
        bingo.jogarAutomatico();
    } else if (opcao == 2) {
        bingo.jogarManual();
    } else {
        cout << VERMELHO << "Opcao invalida!\n" << RESET;
    }

    return 0;
}
