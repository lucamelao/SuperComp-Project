#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <random>
#include <iomanip>
#include <chrono>
#include <fstream>

using namespace std;

struct filme{
    int id;
    int inicio;
    int fim;
    int categoria;
};

// Define a estrutura de uma programação de maratona
struct bestSchedule{
    vector<filme> filmes;
    int tempoSemTela;
};


bool conflito(filme novoFilme, vector<int> limiteCategoria, vector<int> contagemCategoria, vector<filme> filmesEscolhidos) {
    
    // Verifica se a categoria do filme já alcançou o limite
    if (contagemCategoria[novoFilme.categoria - 1] >= limiteCategoria[novoFilme.categoria - 1]) {
        return true;
    }

    // Verifica se há conflito de horários com os filmes já selecionados
    for (auto& filmeEscolhido : filmesEscolhidos) {
        int fimFilmeEscolhido = filmeEscolhido.fim >= filmeEscolhido.inicio ? filmeEscolhido.fim : filmeEscolhido.fim + 24;
        int fimNovoFilme = novoFilme.fim >= novoFilme.inicio ? novoFilme.fim : novoFilme.fim + 24;

        if (!((novoFilme.inicio >= fimFilmeEscolhido) || (fimNovoFilme <= filmeEscolhido.inicio))) {
            return true;
        }
    }

    // Se não houver conflito de horários e a categoria do filme ainda não tiver atingido o limite, retorna false
    return false;
}

int main(){

    // Inicia a contagem do tempo de execução do programa
    auto start_time = std::chrono::high_resolution_clock::now();

    int N = 0; // n de filmes
    int C = 0; // n de categorias

    vector<int> limites;
    vector <filme> filmes, maratona; 

    cin >> N >> C;

    filmes.reserve(N);
    limites.reserve(C);

    // Captura o número de filmes para cada categoria
    int cat_max;
    for (int i = 0; i < C; i++){
        cin >> cat_max;
        limites.push_back(cat_max);
    }

    // Captura os filmes
    for (int i = 0; i < N; i++){
        int inicio, fim, categoria;
        cin >> inicio >> fim >> categoria;
        filmes.push_back({i, inicio, fim, categoria});
    }

    bestSchedule maratonaFinal;
    maratonaFinal.filmes = {};
    maratonaFinal.tempoSemTela = 24;

    // Número de combinações possíveis
    int slent = pow(2, N);
    
    // https://stackoverflow.com/questions/43241174/javascript-generating-all-combinations-of-elements-in-a-single-array-in-pairs

    // ------------------------ Algoritmo Exaustivo ------------------------
    for (int i = 0; i < slent; i++){
        
        bestSchedule maratonaAtual;
        maratonaAtual.filmes = {};
        maratonaAtual.tempoSemTela = 24;

        vector<int> local_limites(C,0);

        for (int j = 0; j < N; j++){
            if ((i & int(pow(2, j))) && !conflito(filmes[j], limites, local_limites, maratonaAtual.filmes)){
                maratonaAtual.filmes.push_back(filmes[j]);
                local_limites[filmes[j].categoria-1]++;
                if (filmes[j].fim >= filmes[j].inicio){
                    maratonaAtual.tempoSemTela -= filmes[j].fim - filmes[j].inicio;
                } else {
                    maratonaAtual.tempoSemTela -= filmes[j].fim + 24 - filmes[j].inicio;
                }
            }

            // Se o número de filmes de cada categoria já foi atingido, para a iteração
            // Ou se já bateu 24 horas de tempo de tela, para a iteração
            if ((local_limites == limites) || maratonaAtual.tempoSemTela == 0){
                break;
            }

            // Atualiza a melhor maratona
            if (maratonaAtual.tempoSemTela < maratonaFinal.tempoSemTela){
                maratonaFinal = maratonaAtual;
            }
        }
    }

    // ---------------------------------------------------------------------
    
    // Encerra a contagem do tempo de execução do programa.
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    double time_taken = duration.count()/1000.0;

    int totalDeFilmes = maratonaFinal.filmes.size();

    // Exibe o resultado.
    cout << "\nFilmes assistidos na Exaustiva Sequencial: " << totalDeFilmes << endl;
    
    // Escreve o resultado em um arquivo.
    ofstream myfile;
    myfile.open ("output_exaustiva_sequencial.txt", ios::app);
    myfile << N << " " << C << " " << time_taken << " " << totalDeFilmes << "\n";
    myfile.close();

    return 0;
}