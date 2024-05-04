# SUDOKU_SOLVER

SUDOKU_SOLVER é um programa que tem como objetivo encontrar
uma solução para um quebra-cabeça de
[Sudoku](https://en.wikipedia.org/wiki/Sudoku) representado por uma
matriz 9x9, caso uma solução exista. Esse programa é o produto de um
trabalho prático da disciplina de Introdução à Inteligência Artificial
do [Departamento de Ciência da Computação da
UFMG](https://dcc.ufmg.br/).

## Sumário

- [Compilação](#Compilação)
- [Execução](#Execução)
- [Benchmarks](#Benchmarks)
- [Documentação](#Documentação)

# Compilação

O script `run` é responsável por gerenciar a compilação com o
[CMake](https://en.wikipedia.org/wiki/CMake) e a execução do programa.
Ele oferece várias opções para facilitar diferentes operações
relacionadas ao projeto. A tabela abaixo apresenta essas opções:

| Parâmetro do script `run`       | Descrição                                                                                         |
|---------------------------------|---------------------------------------------------------------------------------------------------|
| `-h, --help`                    | Exibe uma mensagem de ajuda                                                                       |
| `-b, --build`                   | Executa a compilação                                                                              |
| `-d, --debug`                   | Compila o projeto em modo de depuração (debug mode)                                               |
| `-e, --exec`                    | Executa o programa                                                                                |
| `-c, --clean`                   | Limpa os artefatos gerados pelo CMake, como os diretórios de compilação e os binários             |
| `-t, --test`                    | Executa os testes unitários                                                                       |
| `-v, --valgrind`                | Executa testes com Valgrind para verificar vazamentos e o uso de memória                          |
| `-x <args>, --exec_args <args>` | Permite passar argumentos adicionais para o programa. Pode ser utilizado com as flags `-e` e `-v` |

Assim, SUDOKU_SOLVER pode ser compilado com o seguinte
comando:

``` bash
$ ./run --build
# ou
$ ./run -b
```

OBS.: O programa foi desenvolvido utilizando o g++ `13.2.1`. Gentileza
compilá-lo com essa versão do compilador.

# Execução

A execução do programa pode ser feita com o comando:

``` bash
$ ./run -e -x <program params> # Executa o programa com a passagem de parâmetros
```

Também é possível executar o binário do programa diretamente, por
exemplo:

``` bash
$ bin/Release/sudoku_solver <program params>
```

SUDOKU_SOLVER espera a passagem dos seguintes parâmetros:

- uma letra maiúscula que define qual algoritmo de [busca em espaços de
  estado](https://en.wikipedia.org/wiki/State_space_search) será
  empregado para encontrar a solução
- a matriz inicial do Sudoku.

A tabela abaixo apresenta todos os algoritmos de busca implementados.

| Parâmetro    | Descrição                                                                |
|--------------|--------------------------------------------------------------------------|
| `B <matrix>` | Busca uma solução com o algoritmo Breadth-First Search                   |
| `I <matrix>` | Busca uma solução com o algoritmo Iterative Deepening Depth-First Search |
| `U <matrix>` | Busca uma solução com o algoritmo Uniform-Cost Search                    |
| `A <matrix>` | Busca uma solução com o algoritmo A\* Search                             |
| `G <matrix>` | Busca uma solução com o algoritmo Greedy Best-First Search               |

A matriz é dada por 9 conjuntos de 9 números, onde o primeiro conjunto é
a primeira linha da matriz, o segundo é a segunda linha etc.

Exemplo de execução:

``` bash
                # linha 1  linha 2   linha 3   linha 4   linha 5   linha 6   linha 7   linha 8   linha 9
$ ./run -e -x U 800000000 003600000 070090200 050007000 000045700 000100030 001000068 008500010 090000400
```

Zero representa as posições que precisam ser preenchidas.

O comando acima passa a seguinte matriz para o programa:

``` txt
8 0 0 | 0 0 0 | 0 0 0
0 0 3 | 6 0 0 | 0 0 0
0 7 0 | 0 9 0 | 2 0 0
------+-------+------
0 5 0 | 0 0 7 | 0 0 0
0 0 0 | 0 4 5 | 7 0 0
0 0 0 | 1 0 0 | 0 3 0
------+-------+------
0 0 1 | 0 0 0 | 0 6 8
0 0 8 | 5 0 0 | 0 1 0
0 9 0 | 0 0 0 | 4 0 0
```

Saída esperada do programa para a matriz de entrada:

``` txt
1813316 11797
812753649 943682175 675491283 154237896 369845721 287169534 521974368 438526917 796318452
```

# Benchmarks

A discussão dos resultados obtidos durante os teste podem ser lidos na
seção 4 da
[documentação](https://github.com/luk3rr/SUDOKU_SOLVER/tree/main/docs/documentacao.pdf).

# Documentação

A primeira versão da documentação, bem como o enunciado deste trabalho
pode ser lida
[aqui](https://github.com/luk3rr/SUDOKU_SOLVER/tree/main/docs)
