# Graphs in C — Middle-earth Routes

Academic project on weighted graphs in C. The program reads a list of Middle-earth routes from a CSV file, builds an **adjacency matrix**, and finds paths with **breadth-first search (BFS)** and **Dijkstra's algorithm**.

Developed by Everton Barão for the **Systems Analysis and Optimization** course (*Análise e Otimização de Sistemas*), Computer Science program at Universidade Veiga de Almeida (Brazil).

> The source code, its comments and the program output are in Portuguese, as submitted for the course.

## Academic goal

The assignment practices representing and analyzing weighted graphs through four tasks:

| Task | What it does |
|---|---|
| 1 | Reads the CSV, validates each line and maps every location to an index (`V00`, `V01`, …) |
| 2 | Prints the symmetric adjacency matrix (0 = no edge; value = days of walking) |
| 3 | BFS: path with the **fewest edges** between two locations |
| 4 | Dijkstra: path with the **lowest total cost** in days |

The graph is undirected. Weights are estimated walking days.

## Files

| File | Contents |
|---|---|
| `grafo.c` | Full program (reading, matrix, BFS and Dijkstra) |
| `grafo_rotas_terramedia_lista_arestas 1.csv` | Edge list provided with the assignment (36 locations, 55 routes) |

CSV format (`origem` = origin, `destino` = destination, `dias_caminhada_estimados` = estimated walking days):

```csv
origem,destino,dias_caminhada_estimados
Grey Havens,Lindon,2
Lindon,Harlindon,3
```

The program also accepts space-separated lines in the form `origin destination weight`.

## Building

With GCC (Linux, macOS, WSL or MSYS2 on Windows):

```bash
gcc -std=c11 -Wall -Wextra -Werror -pedantic grafo.c -o grafo
```

On Dev-C++ (TDM-GCC 4.9.2), add `-D__USE_MINGW_ANSI_STDIO=1`. Without it, this older compiler does not recognize the `%lld` format and the build fails under `-Werror`:

```bash
gcc -std=c11 -Wall -Wextra -Werror -pedantic -D__USE_MINGW_ANSI_STDIO=1 grafo.c -o grafo.exe
```

## Running

```
grafo <file.csv> <task> [origin destination]
```

```bash
./grafo "grafo_rotas_terramedia_lista_arestas 1.csv" 1
./grafo "grafo_rotas_terramedia_lista_arestas 1.csv" 2
./grafo "grafo_rotas_terramedia_lista_arestas 1.csv" 3
./grafo "grafo_rotas_terramedia_lista_arestas 1.csv" 4
```

On Windows, use `grafo.exe` instead of `./grafo`.

- Without origin and destination, task 3 goes from **Bree** to **Gondor Border**, and task 4 goes from **Rivendell** to **Mordor Gate**.
- Tasks 3 and 4 accept any other origin and destination. Spaces in names become `_`, so `"Mount Doom"` and `Mount_Doom` are equivalent.

## Examples

Actual program output using the CSV in this repository. Output labels are in Portuguese: *Caminho* = path, *Arestas* = edges, *Peso total* = total weight, *dias* = days.

**Task 1** (excerpt):

```
Tarefa 1
Vertices: 36 | Arestas: 55
V00 = Grey_Havens
V01 = Lindon
V02 = Harlindon
...
V35 = Khand
```

**Task 2** (excerpt from the first 12-column block):

```
Colunas V00 a V11
Vertice                  V00 V01 V02 V03 V04 V05 V06 V07 V08 V09 V10 V11
V00 Grey_Havens            0   2   0   2   0   0   0   0   0   0   0   0
V01 Lindon                 2   0   3   0   5   0   0   0   0   0   0   0
V02 Harlindon              0   3   0   0   0   0   0   0   0   0   0   0
```

**Task 3** — BFS from Bree to Gondor Border:

```
Tarefa 3
Caminho:
Bree -> Tharbad -> Lórien -> Rohan -> Gondor_Border
Arestas: 4
Peso total: 23 dias
```

**Task 4** — Dijkstra from Rivendell to Mordor Gate:

```
Tarefa 4
Caminho:
Rivendell -> Khazad-dûm -> Rohan -> Gondor_Border -> Minas_Tirith -> Osgiliath -> Minas_Morgul -> Mordor_Gate
Arestas: 7
Peso total: 26 dias
```

**Query with a custom origin and destination:**

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

BFS minimizes the number of edges, not the sum of weights. Ties are broken by the order in which locations appear in the CSV.

## Validation and limits

- Up to 100 vertices; names up to 31 UTF-8 characters.
- The program exits with an error message and a non-zero exit code on: missing file, malformed line, extra column, weight that is not a positive integer, duplicate edge, empty name, name too long, name with quotes, more than 100 vertices, or an unknown location in a query.
- When origin and destination exist but are not connected, the program prints `Nao existe rota.` (no route exists).
- Distances use `long long` to avoid overflow when adding large weights.

**Accented names on Windows:** Windows passes command-line arguments in an encoding other than the UTF-8 used by the CSV. On Windows, the program therefore re-reads the origin and destination as Unicode (`GetCommandLineW`) and converts them to UTF-8, so queries such as `Annúminas` and `Khazad-dûm` work. On other systems the arguments are used as received.

## Complexity

With V vertices and E edges:

- Adjacency matrix: O(V²) memory.
- Reading: O(E·V), due to the linear name lookup.
- Matrix printing, BFS and Dijkstra over the matrix: O(V²) each.

## AI assistance

This project was developed with the help of artificial intelligence tools. Organizing this repository, checking the results and writing this README were done with the help of Claude (Anthropic). The code, implementation choices and academic submission are the author's responsibility.
