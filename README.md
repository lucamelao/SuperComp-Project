# SuperComp-Project

Repositório para entrega do Projeto de SuperComputação (2023.1)

## Arquivos e seus conteúdos

[**Relatório Final**](./RelatorioFinal.ipynb): Jupyter Notebook, contendo o relatório intermediário do projeto. O relatório parcial pode ser encontrado [**aqui**](./RelatorioParcial.ipynb).

Arquivos incluídos no projeto e seus conteúdos:

1. [**execute.py**](./execute.py): Script em Python que automatiza a geração dos resultados e executa cada heurística para cada input gerado.

2. [**gulosa.cpp**](./gulosa.cpp): Implementação em C++ da heurística gulosa para o problema da maratona de filmes.

3. [**aleatoria.cpp**](./aleatoria.cpp): Implementação em C++ da heurística aleatoria para o problema da maratona de filmes.

4. [**old_inputs**](./old_inputs): Pasta contendo os 20 inputs gerados para teste das heurísticas. Os inputs possuem um número N de filmes de 100, 1000, 10000, 100000 e 1000000 filmes, e um número C de categorias que varia entre 4, 6, 8, 10. Os inputs são gerados aleatoriamente pelo código fornecido pelo professor.

5. [**new_inputs**](./new_inputs): Pasta contendo os 8 inputs gerados para teste da heurística exaustiva. Os inputs possuem um número N de filmes de 10, 12, 14, 16 e 18, 20, 22 e 24 filmes, e um número C de categorias de 4, 6, 8, 10. Os inputs forams gerados aleatoriamente pelo código fornecido pelo professor.

6. [**outputs**](./outputs): Pasta contendo os outputs gerados por cada heurística para cada input gerado.

7. [**exaustiva_sequencial.cpp**](./exaustiva_sequencial.cpp): Implementação em C++ da heurística exaustiva para o problema da maratona de filmes.

8. [**exaustiva_paralela.cpp**](./exaustiva_paralela.cpp): Implementação em C++ da heurística exaustiva para o problema da maratona de filmes.

9. [**exaustiva_gpu.cu**](./exaustiva_gpu.cu): Implementação em C++ da heurística exaustiva para o problema da maratona de filmes.

10. [**profiling**](./profiling): Pasta contendo o profiling (valgrind) realizado para as heurísticas gulosa e aleatória.
