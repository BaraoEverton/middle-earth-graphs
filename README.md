# Grafos em C — Rotas da Terra Média

Projeto acadêmico de grafos ponderados em linguagem C. O programa lê uma lista de rotas da Terra Média em CSV, monta uma **matriz de adjacência** e encontra caminhos com **busca em largura (BFS)** e com o **algoritmo de Dijkstra**.

Trabalho desenvolvido para a disciplina **Análise e Otimização de Sistemas**, do curso de Ciência da Computação da Universidade Veiga de Almeida, por Everton Barão.

## Objetivo acadêmico

O trabalho pratica a representação e a análise de grafos ponderados em quatro tarefas:

| Tarefa | O que faz |
|---|---|
| 1 | Lê o CSV, valida as linhas e associa cada localidade a um índice (`V00`, `V01`, …) |
| 2 | Imprime a matriz de adjacência simétrica (0 = sem aresta; valor = dias de caminhada) |
| 3 | BFS: caminho com o **menor número de arestas** entre duas localidades |
| 4 | Dijkstra: caminho com o **menor custo total** em dias |

O grafo é não direcionado. Os pesos representam dias estimados de caminhada.

## Arquivos

| Arquivo | Conteúdo |
|---|---|
| `grafo.c` | Programa completo (leitura, matriz, BFS e Dijkstra) |
| `grafo_rotas_terramedia_lista_arestas 1.csv` | Lista de arestas fornecida na atividade (36 localidades, 55 rotas) |

Formato do CSV:

```csv
origem,destino,dias_caminhada_estimados
Grey Havens,Lindon,2
Lindon,Harlindon,3
```

O programa também aceita linhas no formato `origem destino peso`, separadas por espaço.

## Compilação

Com GCC (Linux, macOS, WSL ou MSYS2 no Windows):

```bash
gcc -std=c11 -Wall -Wextra -Werror -pedantic grafo.c -o grafo
```

No Dev-C++ (TDM-GCC 4.9.2), acrescente `-D__USE_MINGW_ANSI_STDIO=1`. Sem essa opção, esse compilador antigo não reconhece o formato `%lld` e, com `-Werror`, a compilação falha:

```bash
gcc -std=c11 -Wall -Wextra -Werror -pedantic -D__USE_MINGW_ANSI_STDIO=1 grafo.c -o grafo.exe
```

## Execução

```
grafo <arquivo.csv> <tarefa> [origem destino]
```

```bash
./grafo "grafo_rotas_terramedia_lista_arestas 1.csv" 1
./grafo "grafo_rotas_terramedia_lista_arestas 1.csv" 2
./grafo "grafo_rotas_terramedia_lista_arestas 1.csv" 3
./grafo "grafo_rotas_terramedia_lista_arestas 1.csv" 4
```

No Windows, use `grafo.exe` no lugar de `./grafo`.

- Sem origem e destino, a tarefa 3 vai de **Bree** a **Gondor Border**, e a tarefa 4 vai de **Rivendell** a **Mordor Gate**.
- Nas tarefas 3 e 4 é possível informar outra origem e outro destino. Espaços nos nomes viram `_`, então `"Mount Doom"` e `Mount_Doom` são equivalentes.

## Exemplos

Saídas reais do programa com o CSV deste repositório.

**Tarefa 1** (trecho):

```
Tarefa 1
Vertices: 36 | Arestas: 55
V00 = Grey_Havens
V01 = Lindon
V02 = Harlindon
...
V35 = Khand
```

**Tarefa 2** (trecho do primeiro bloco de 12 colunas):

```
Colunas V00 a V11
Vertice                  V00 V01 V02 V03 V04 V05 V06 V07 V08 V09 V10 V11
V00 Grey_Havens            0   2   0   2   0   0   0   0   0   0   0   0
V01 Lindon                 2   0   3   0   5   0   0   0   0   0   0   0
V02 Harlindon              0   3   0   0   0   0   0   0   0   0   0   0
```

**Tarefa 3** — BFS de Bree a Gondor Border:

```
Tarefa 3
Caminho:
Bree -> Tharbad -> Lórien -> Rohan -> Gondor_Border
Arestas: 4
Peso total: 23 dias
```

**Tarefa 4** — Dijkstra de Rivendell a Mordor Gate:

```
Tarefa 4
Caminho:
Rivendell -> Khazad-dûm -> Rohan -> Gondor_Border -> Minas_Tirith -> Osgiliath -> Minas_Morgul -> Mordor_Gate
Arestas: 7
Peso total: 26 dias
```

**Consulta com origem e destino informados:**

```bash
./grafo "grafo_rotas_terramedia_lista_arestas 1.csv" 4 "Grey Havens" "Mount Doom"
```

```
Tarefa 4
Caminho:
Grey_Havens -> Lindon -> Archet -> Bree -> Tharbad -> Lórien -> Rohan -> Gondor_Border -> Minas_Tirith -> Osgiliath -> Minas_Morgul -> Mount_Doom
Arestas: 11
Peso total: 42 dias
```

A BFS minimiza o número de arestas, não a soma dos pesos. Quando há empate, vale a ordem em que as localidades aparecem no CSV.

## Validações e limites

- Até 100 vértices; nomes com até 31 caracteres UTF-8.
- O programa encerra com mensagem de erro e código de saída diferente de zero quando há arquivo inexistente, linha malformada, coluna extra, peso que não é inteiro positivo, aresta duplicada, nome vazio, nome longo demais, nome com aspas, mais de 100 vértices ou localidade desconhecida na consulta.
- Quando origem e destino existem mas não estão conectados, o programa imprime `Nao existe rota.`.
- As distâncias usam `long long`, o que evita estouro ao somar pesos grandes.

**Nomes com acento no Windows:** o Windows entrega os argumentos da linha de comando em uma codificação diferente do UTF-8 usado no CSV. Por isso, no Windows, o programa relê a origem e o destino em Unicode (`GetCommandLineW`) e os converte para UTF-8. Assim, consultas como `Annúminas` e `Khazad-dûm` funcionam. Nos demais sistemas os argumentos são usados como chegam.

## Complexidade

Com V vértices e E arestas:

- Matriz de adjacência: O(V²) de memória.
- Leitura: O(E·V), por causa da busca linear pelos nomes.
- Impressão da matriz, BFS e Dijkstra sobre matriz: O(V²) cada.

## Apoio de IA no desenvolvimento

Este projeto contou com apoio de ferramentas de inteligência artificial durante o desenvolvimento. A organização deste repositório, a conferência dos resultados e a redação deste README foram feitas com auxílio do Claude (Anthropic). O código, as escolhas de implementação e a entrega acadêmica são de responsabilidade do autor.
