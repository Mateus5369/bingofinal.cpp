#include <iostream>
#include <vector>
#include <deque> //Permite usar a estrutura deque (double-ended queue) 
#include <algorithm> //find() random_shuffle()
#include <cstdlib>
#include <ctime>
#include <iomanip> //Permite manipular formatação de saída setw()

#ifdef _WIN32
    #include <windows.h>
    #define SLEEP(x) Sleep(x * 1000)
#else
    #include <unistd.h>
    #define SLEEP(x) sleep(x)
#endif

using namespace std;

// Códigos de cores ANSI
const string RESET = "\033[0m";
const string VERDE = "\033[1;32m";
const string AMARELO = "\033[1;33m";
const string AZUL = "\033[1;34m";
const string VERMELHO = "\033[1;31m";
const string CIANO = "\033[1;36m";
const string BRANCO = "\033[1;37m";

// Função para limpar o ecrã
void limparEcra() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Classe que representa a cartela do Bingo
class Cartela {
private:
    vector<int> numeros; // números de 1 a 100

public:
    Cartela() {
        for (int i = 1; i <= 100; i++) {
            numeros.push_back(i);
        }
    }

    // Verifica se um número foi sorteado
    bool foiSorteado(int numero, const vector<int>& sorteados) {
        return find(sorteados.begin(), sorteados.end(), numero) != sorteados.end();
    }

    // Obtém letra da coluna
    string obterLetraColuna(int numero) {
        if (numero >= 1 && numero <= 20) return "B";
        if (numero >= 21 && numero <= 40) return "I";
        if (numero >= 41 && numero <= 60) return "N";
        if (numero >= 61 && numero <= 80) return "G";
        if (numero >= 81 && numero <= 100) return "O";
        return "";
    }

    // Exibe a cartela completa com números destacados
    void exibir(const vector<int>& sorteados) {
        cout << "\n" << CIANO << "========== CARTELA DE BINGO ==========\n" << RESET;
        cout << "  B        I        N        G        O\n";
        cout << "--------------------------------------\n";

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

// Classe que controla o jogo de Bingo
class Bingo {
private:
    Cartela cartela;
    vector<int> todosSorteados;
    deque<int> ultimas5;
    int bolaAnterior;

public:
    Bingo() {
        bolaAnterior = -1;
        // Inicializa vetor de números de 1 a 100
        for (int i = 1; i <= 100; i++) {
            todosSorteados.push_back(i);
        }
        srand(time(NULL));
        random_shuffle(todosSorteados.begin(), todosSorteados.end());
        ultimas5.clear();
    }

    void exibirTitulo() {
        cout << AZUL;
        cout << "     ██╗ ██████╗  ██████╗  ██████╗     ██████╗  ██████╗     ██████╗ ██╗███╗   ██╗ ██████╗  ██████╗ \n";
        cout << "     ██║██╔═══██╗██╔════╝ ██╔═══██╗    ██╔══██╗██╔═══██╗    ██╔══██╗██║████╗  ██║██╔════╝ ██╔═══██╗\n";
        cout << "     ██║██║   ██║██║  ███╗██║   ██║    ██║  ██║██║   ██║    ██████╔╝██║██╔██╗ ██║██║  ███╗██║   ██║\n";
        cout << "██   ██║██║   ██║██║   ██║██║   ██║    ██║  ██║██║   ██║    ██╔══██╗██║██║╚██╗██║██║   ██║██║   ██║\n";
        cout << "╚█████╔╝╚██████╔╝╚██████╔╝╚██████╔╝    ██████╔╝╚██████╔╝    ██████╔╝██║██║ ╚████║╚██████╔╝╚██████╔╝\n";
        cout << RESET << "\n";
    }

    string obterLetraColuna(int numero) {
        return cartela.obterLetraColuna(numero);
    }

    void exibirInformacoes(int bolaAtual, const vector<int>& numerosSorteados) {
        limparEcra();
        exibirTitulo();

        cout << "\n" << VERMELHO << "===================================\n";
        cout << "   NUMERO SORTEADO: " << obterLetraColuna(bolaAtual) << "-";
        cout << setw(3) << bolaAtual << "        \n";
        cout << "===================================\n" << RESET;

        if (bolaAnterior != -1) {
            cout << AMARELO << "Ultimo numero sorteado: " 
                 << obterLetraColuna(bolaAnterior) << "-" << bolaAnterior << RESET << "\n";
        }

        if (!ultimas5.empty()) {
            cout << AZUL << "Ultimos 5 numeros sorteados: " << RESET;
            for (size_t i = 0; i < ultimas5.size(); i++) {
                cout << obterLetraColuna(ultimas5[i]) << "-" << ultimas5[i];
                if (i < ultimas5.size() - 1) cout << ", ";
            }
            cout << "\n";
        }

        cartela.exibir(numerosSorteados);
        cout << CIANO << "Total de numeros sorteados: " << numerosSorteados.size() << "/100" << RESET << "\n";
    }

    void jogarAutomatico() {
        cout << CIANO << "Modo Automatico ativado! Numeros a cada 2 segundos.\n" << RESET;
        SLEEP(2);

        vector<int> numerosSorteados;

        for (size_t i = 0; i < todosSorteados.size(); i++) {
            int numero = todosSorteados[i];

            ultimas5.push_back(numero);
            if (ultimas5.size() > 5) ultimas5.pop_front();

            numerosSorteados.push_back(numero); // adiciona à lista de sorteados

            exibirInformacoes(numero, numerosSorteados);
            bolaAnterior = numero;
            SLEEP(2);
        }

        cout << VERMELHO << "Todos os numeros foram sorteados! Jogo terminado.\n" << RESET;
    }

    void jogarManual() {
        cout << CIANO << "Modo Manual ativado! Prima Enter para sortear cada numero.\n" << RESET;
        SLEEP(1);

        vector<int> numerosSorteados;

        for (size_t i = 0; i < todosSorteados.size(); i++) {
            cout << "\nPrima Enter para sortear o proximo numero...";
            cin.get();

            int numero = todosSorteados[i];

            ultimas5.push_back(numero);
            if (ultimas5.size() > 5) ultimas5.pop_front();

            numerosSorteados.push_back(numero); // adiciona à lista de sorteados

            exibirInformacoes(numero, numerosSorteados);
            bolaAnterior = numero;
        }

        cout << VERMELHO << "Todos os numeros foram sorteados! Jogo terminado.\n" << RESET;
    }
};

int main() {
    Bingo bingo;
    bingo.exibirTitulo();

    cout << "\nEscolha o modo de jogo:\n";
    cout << "1 - Modo Automatico (sorteio a cada 2 segundos)\n";
    cout << "2 - Modo Manual (Enter para cada sorteio)\n";
    cout << "Opcao: ";

    int opcao;
    cin >> opcao;
    cin.ignore(); // Limpar buffer

    if (opcao == 1) {
        bingo.jogarAutomatico();
    } else if (opcao == 2) {
        bingo.jogarManual();
    } else {
        cout << VERMELHO << "Opcao invalida!\n" << RESET;
    }

    return 0;
}
