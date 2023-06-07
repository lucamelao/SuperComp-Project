#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <random>
#include <iomanip>
#include <chrono>
#include <fstream>

using namespace std;

#include <thrust/device_vector.h>
#include <thrust/functional.h>
#include <thrust/transform_reduce.h>
#include <thrust/sequence.h>
#include <thrust/fill.h>

struct filme{
    int id;
    int inicio;
    int fim;
    int categoria;
};

// Functor que aplica a busca exaustiva
struct exaustiva_GPU
{
    int N;
    int C;
    int *cat_limites;
    filme *filmes;

    exaustiva_GPU(int _N, int _C, int *_cat_limites, filme *_filmes) 
    : N(_N), C(_C), cat_limites(_cat_limites), filmes(_filmes) {}

    __device__ int operator()(int i) const
    {
        int local_limites[50];  

        // int tempoSemTela = 24;
        int maratonaAtual = 0;

        filme last_added;
        bool has_added = false;

        // Inicializa limites locais
        for (int j = 0; j < 50; j++) {
            local_limites[j] = 0;
        }

        for (int j = 0; j < N; j++){
            if ((i & (1 << j))){

                // Verifica se a categoria do filme já alcançou o limite
                if (local_limites[filmes[j].categoria - 1] >= cat_limites[filmes[j].categoria - 1]) {
                    return -1;
                }

                if (!has_added){
                    has_added = true;
                }else{
                    int fimlast_added = last_added.fim > last_added.inicio ? last_added.fim : last_added.fim + 24;
                    int fimNovoFilme = filmes[j].fim > filmes[j].inicio ? filmes[j].fim : filmes[j].fim + 24;

                    if (!((filmes[j].inicio >= fimlast_added) || (fimNovoFilme <= last_added.inicio))) {
                        return -1;
                    }
                }

                // Calcula o tempo de tela do filme
                int tempoFilme = 0;

                if (filmes[j].fim >= filmes[j].inicio){
                    tempoFilme = filmes[j].fim - filmes[j].inicio;
                } else {
                    tempoFilme = filmes[j].fim + 24 - filmes[j].inicio;
                }

                // Verifica se há tempo de tela disponível
                if (tempoSemTela < tempoFilme) {
                    return -1;
                }

                // Decrementa conforme adiciona filme
                tempoSemTela -= tempoFilme;

                maratonaAtual++;
                local_limites[filmes[j].categoria - 1]++;
                
                last_added = filmes[j];
            }
        }
        
        // Retorna a quantidade de filmes na maratona atual
        return maratonaAtual;
    }
};

int main() {

    auto start_time = std::chrono::high_resolution_clock::now();

    int N = 0; // Número de filmes
    int C = 0;  // Número de categorias

    cin >> N >> C;

    // Ler os dados do arquivo de entrada
    vector<int> limites;
    vector<filme> filmes, maratona; 

    // Captura o número de filmes para cada categoria
    int cat_limites;
    for (int i = 0; i < C; i++){
        cin >> cat_limites;
        limites.push_back(cat_limites);
    }

    // Captura os filmes
    for (int i = 0; i < N; i++){
        int inicio, fim, categoria;
        cin >> inicio >> fim >> categoria;
        filmes.push_back({i, inicio, fim, categoria});
    }
    
    // Ordena os filmes por hora de início crescente
    sort(filmes.begin(), filmes.end(), [](const filme& a, const filme& b){return a.inicio < b.inicio;});

    // --------------------------------------- Carregamento na CPU ---------------------------------------

    // Carregar os dados do arquivo de entrada na memória da GPU
    thrust::device_vector<filme> filmes_gpu(filmes);
    thrust::device_vector<int> categorias_gpu(limites);

    // Cria um device vector para os resultados
    thrust::device_vector<int> resultados(pow(2, N));

    exaustiva_GPU functor(N, C, thrust::raw_pointer_cast(categorias_gpu.data()), thrust::raw_pointer_cast(filmes_gpu.data()));

    // Aplica o functor
    thrust::transform(thrust::counting_iterator<int>(0), thrust::counting_iterator<int>(pow(2, N)), resultados.begin(), functor);

    // --------------------------------------------- Output ---------------------------------------------

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    double time_taken = duration.count()/1000.0;

    int totalDeFilmes = *thrust::max_element(resultados.begin(), resultados.end());

    cout << "\nFilmes assistidos na Exaustiva com GPU: " << totalDeFilmes << endl;

    // Escreve o resultado em um arquivo de saída
    ofstream myfile;
    myfile.open ("output_exaustiva_gpu.txt", ios::app);
    myfile << N << " " << C << " " << time_taken << " " << totalDeFilmes << "\n";
    myfile.close();

    return 0;

}