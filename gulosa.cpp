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

void print_limits(map<int, int> limites, int categorias){
    for (int i = 1; i <= categorias; i++) {
        cout << "[CAT " << i << "]: " << limites[i] << " filmes";
        if (i < categorias) {
            cout << " | ";
        }
    }
    cout << "\n";
}

void check_categories(vector<filme> maratona){

    // Conta o número de filmes de cada categoria na maratona
    map<int, int> contagem;
    for (auto& el : maratona){
        contagem[el.categoria]++;
    }

    // Conferindo se o número de filmes de cada categoria foi respeitado
    cout << "\nCONTAGEM POR CATEGORIA NA MARATONA:\n";
    cout << "-----------------------------------------------------------------------------------\n";
    int i = 1;
    for (auto& el : contagem){
        cout << "CATEGORIA " << el.first << ": " << el.second << " filmes \n";
        i++;
    }
}

bool conflito(const filme& f, const vector<filme>& maratona){
    for (const auto& el : maratona){
        if (!((f.inicio >= el.fim) || (f.fim <= el.inicio))){
            return true;
        }
    }
    return false;
}

void correct_time(vector<filme> filmes){
    for (auto& el : filmes){
        if (el.fim < el.inicio){
            el.fim += 24;
        }
    }
}

int main(){

    auto start_time = std::chrono::high_resolution_clock::now();

    int N = 0; // n de filmes
    int C = 0; // n de categorias

    map<int, int> limites;
    vector <filme> filmes, maratona; 

    cin >> N >> C;

    filmes.reserve(N);

    // Captura o número de filmes para cada categoria
    for (int i = 0; i < C; i++){
        cin >> limites[i+1];
    }

    // print_limits(limites, C);

    // Captura os filmes
    for (int i = 0; i < N; i++){
        int inicio, fim, categoria;
        cin >> inicio >> fim >> categoria;
        filmes.push_back({i, inicio, fim, categoria});
    }

    // Corrige os filmes que viram noite
    correct_time(filmes);
 
    // ------------------------ Algoritmo Guloso ------------------------ 
    
    // Ordena os filmes por hora de fim crescente
    // INVARIANTE 1
    sort(filmes.begin(), filmes.end(), [](auto& i, auto& j){return i.fim < j.fim;}); 
    
    int tempoSemTela = 0;

    for (int i = 0; i < N; i++){

        if (filmes[i].fim <= filmes[i].inicio) {
            continue; // Ignora filmes sem duração ou com duração negativa
        }

        if (maratona.empty()){
            maratona.push_back(filmes[i]);
            limites[filmes[i].categoria]--;
        }
        else{
            if ((filmes[i].inicio >= maratona.back().fim) && (limites[filmes[i].categoria] > 0) && !conflito(filmes[i], maratona)){

                tempoSemTela += filmes[i].inicio - maratona.back().fim;

                maratona.push_back(filmes[i]);
                limites[filmes[i].categoria]--;
            }
        }
    }

    // Contabilizando o tempo não aproveitado depois do último filme da maratona
    if (maratona.back().fim < 24){
        tempoSemTela += 24 - maratona.back().fim;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    double time_taken = duration.count()/1000.0;

    // Output visual
    cout << "\nAGENDA DE FILMES (" << maratona.size() << ")\n";
    cout << "-----------------------------------------------------------------------------------\n";
    cout << "  HORAS |";
    for (int h = 0; h < 25; ++h) {
        cout << setw(3) << h;
    }
    cout << "\n-----------------------------------------------------------------------------------\n";
 
    for (const auto &filme : maratona) {
        cout << "  FILM " << setw(2) << filme.id << " |";
        for (int h = 0; h < 24; ++h) {
            if (h >= filme.inicio && h < filme.fim) {
                cout << "###";
            } else {
                cout << "   ";
            }
        }
        cout << " | CAT:" << filme.categoria << "\n";
    }
    
    cout << "-----------------------------------------------------------------------------------";

    // check_categories(maratona);

    int totalDeFilmes = maratona.size();
    int tempoDeTela = 24 - tempoSemTela;

    cout << "\nFilmes assistidos: " << maratona.size() << "\n";
    cout << "Tempo de tela: " << tempoDeTela << " horas."<< endl;
    std::cout << "Execution time: " << time_taken << " ms." << std::endl;

    ofstream myfile;
    myfile.open ("gulosa_output.txt", ios::app);
    myfile << N << " " << C << " " << time_taken << " " << tempoDeTela << " " << totalDeFilmes << "\n";
    myfile.close();

    return 0;
}