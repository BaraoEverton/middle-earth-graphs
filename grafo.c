/* TAREFA 1 - Leitura, nomes e validacao. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#endif

#define MAX 100
#define TAM 128
typedef struct {
    int n, m, peso[MAX][MAX];
    char nome[MAX][TAM];
} Grafo;

static void erro(const char *msg) {
    fprintf(stderr, "Erro: %s\n", msg);
    exit(EXIT_FAILURE);
}
static char *aparar(char *s) {
    while (isspace((unsigned char)*s)) s++;
    size_t n = strlen(s);
    while (n && isspace((unsigned char)s[n-1])) s[--n] = 0;
    return s;
}
static void normalizar(char *s) {
    for (; *s; s++)
        if (isspace((unsigned char)*s)) *s = '_';
}
static int caracteres(const char *s) {
    int n = 0;
    for (; *s; s++)
        if (((unsigned char)*s & 0xC0) != 0x80) n++;
    return n;
}
static int indice(const Grafo *g, const char *nome) {
    for (int i = 0; i < g->n; i++)
        if (!strcmp(g->nome[i], nome)) return i;
    return -1;
}
static int inserir(Grafo *g, const char *nome) {
    int i = indice(g, nome);
    if (i >= 0) return i;
    if (g->n == MAX) erro("mais de 100 vertices");
    strcpy(g->nome[g->n], nome);
    return g->n++;
}
static void carregar(Grafo *g, const char *arquivo) {
    FILE *f = fopen(arquivo, "r");
    if (!f) erro("arquivo inexistente ou inacessivel");
    char linha[512];
    int numero = 0;
    while (fgets(linha, sizeof linha, f)) {
        numero++;
        if (!strchr(linha, '\n') && !feof(f))
            erro("linha excede o limite de leitura");
        char *s = linha;
        if (numero == 1 && !strncmp(s, "\xEF\xBB\xBF", 3))
            s += 3; /* BOM UTF-8 do CSV fornecido. */
        s = aparar(s);
        if (numero == 1 && !strcmp(s,
            "origem,destino,dias_caminhada_estimados"))
            continue;
        char a[512], b[512], w[512], extra;
        char *c1 = strchr(s, ',');
        if (c1) {
            *c1++ = 0;
            char *c2 = strchr(c1, ',');
            if (!c2) erro("linha CSV malformada");
            *c2++ = 0;
            if (strchr(c2, ',')) erro("coluna extra no CSV");
            strcpy(a, aparar(s));
            strcpy(b, aparar(c1));
            strcpy(w, aparar(c2));
        } else if (sscanf(s, "%255s %255s %255s %c",
                          a, b, w, &extra) != 3) {
            erro("esperado origem destino peso");
        }
        if (!*a || !*b || strlen(a) >= TAM || strlen(b) >= TAM
            || caracteres(a) > 31 || caracteres(b) > 31)
            erro("nome vazio ou maior que 31 caracteres");
        if (strchr(a, '"') || strchr(b, '"'))
            erro("aspas nao suportadas nos nomes");
        normalizar(a); normalizar(b);
        char *fim;
        errno = 0;
        long valor = strtol(w, &fim, 10);
        if (errno || fim == w || *fim || valor <= 0
            || valor > INT_MAX) erro("peso inteiro invalido");
        int u = inserir(g, a), v = inserir(g, b);
        if (g->peso[u][v]) erro("aresta duplicada");
        g->peso[u][v] = g->peso[v][u] = (int)valor;
        g->m++;
    }
    if (ferror(f)) erro("falha de leitura");
    fclose(f);
    if (!g->n) erro("arquivo sem arestas");
}
static void nomes(const Grafo *g) {
    printf("Vertices: %d | Arestas: %d\n", g->n, g->m);
    for (int i = 0; i < g->n; i++)
        printf("V%02d = %s\n", i, g->nome[i]);
}

/* TAREFA 2 - Matriz simetrica com rotulos. */
static void matriz(const Grafo *g) {
    /* Vnn identifica o nome na tabela da tarefa 1.
       Blocos de 12 colunas permitem ler a matriz inteira. */
    for (int inicio = 0; inicio < g->n; inicio += 12) {
        int fim = inicio + 12 < g->n ? inicio + 12 : g->n;
        printf("\nColunas V%02d a V%02d\n", inicio, fim-1);
        printf("%-24s", "Vertice");
        for (int j = inicio; j < fim; j++) printf(" V%02d", j);
        putchar('\n');
        for (int i = 0; i < g->n; i++) {
            printf("V%02d %s%*s", i, g->nome[i],
                   20 - caracteres(g->nome[i]), "");
            for (int j = inicio; j < fim; j++)
                printf("%4d", g->peso[i][j]);
            putchar('\n');
        }
    }
}

/* TAREFA 3 - BFS e impressao do caminho. */
static void caminho(const Grafo *g, int alvo, const int pai[]) {
    int rota[MAX], n = 0;
    long long custo = 0;
    for (int v = alvo; v != -1; v = pai[v]) rota[n++] = v;
    printf("Caminho:\n");
    for (int i = n-1; i >= 0; i--) {
        printf("%s%s", g->nome[rota[i]], i ? " -> " : "\n");
        if (i) custo += g->peso[rota[i]][rota[i-1]];
    }
    printf("Arestas: %d\nPeso total: %lld dias\n", n-1, custo);
}
static void bfs(const Grafo *g, int origem, int alvo) {
    int fila[MAX], visto[MAX] = {0}, pai[MAX];
    int primeiro = 0, ultimo = 0;
    for (int i = 0; i < g->n; i++) pai[i] = -1;
    fila[ultimo++] = origem;
    visto[origem] = 1;
    while (primeiro < ultimo) {
        int u = fila[primeiro++];
        if (u == alvo) break;
        for (int v = 0; v < g->n; v++) {
            if (g->peso[u][v] && !visto[v]) {
                visto[v] = 1; pai[v] = u;
                fila[ultimo++] = v;
            }
        }
    }
    if (!visto[alvo]) puts("Nao existe rota.");
    else caminho(g, alvo, pai);
}

/* TAREFA 4 - Dijkstra sobre a matriz. */
static void dijkstra(const Grafo *g, int origem, int alvo) {
    long long dist[MAX];
    int usado[MAX] = {0}, pai[MAX];
    for (int i = 0; i < g->n; i++) {
        dist[i] = LLONG_MAX; pai[i] = -1;
    }
    dist[origem] = 0;
    for (int passo = 0; passo < g->n; passo++) {
        int u = -1;
        for (int i = 0; i < g->n; i++)
            if (!usado[i] && (u == -1 || dist[i] < dist[u]))
                u = i;
        if (u == -1 || dist[u] == LLONG_MAX) break;
        usado[u] = 1;
        if (u == alvo) break;
        for (int v = 0; v < g->n; v++) {
            if (g->peso[u][v] && !usado[v]) {
                long long nova = dist[u] + g->peso[u][v];
                if (nova < dist[v]) {
                    dist[v] = nova; pai[v] = u;
                }
            }
        }
    }
    if (dist[alvo] == LLONG_MAX) puts("Nao existe rota.");
    else caminho(g, alvo, pai);
}

/* PROGRAMA PRINCIPAL - Selecao da tarefa e dos destinos. */
#ifdef _WIN32
/* No Windows, argv usa a pagina de codigo local; o CSV usa UTF-8. */
static const char *argumento(int argc, char *argv[], int i,
                             char *buf, int tam) {
    int n;
    wchar_t **w = CommandLineToArgvW(GetCommandLineW(), &n);
    const char *s = argv[i];
    if (w && n == argc && WideCharToMultiByte(CP_UTF8, 0, w[i], -1,
                                              buf, tam, NULL, NULL))
        s = buf;
    LocalFree(w);
    return s;
}
#else
static const char *argumento(int argc, char *argv[], int i,
                             char *buf, int tam) {
    (void)argc; (void)buf; (void)tam;
    return argv[i];
}
#endif
int main(int argc, char *argv[]) {
    if (argc != 3 && argc != 5)
        erro("uso: grafo arquivo tarefa [origem destino]");
    if (strlen(argv[2]) != 1 || argv[2][0] < '1'
        || argv[2][0] > '4') erro("tarefa deve ser 1, 2, 3 ou 4");
    int tarefa = argv[2][0] - '0';
    if (argc == 5 && tarefa < 3) erro("destinos apenas em 3 ou 4");
    Grafo g = {0};
    carregar(&g, argv[1]);
    printf("Tarefa %d\n", tarefa);
    if (tarefa == 1) nomes(&g);
    else if (tarefa == 2) matriz(&g);
    else {
        char a[TAM], b[TAM], ua[4*TAM], ub[4*TAM];
        const char *o = argc == 5 ? argumento(argc, argv, 3, ua, 4*TAM) :
                        (tarefa == 3 ? "Bree" : "Rivendell");
        const char *d = argc == 5 ? argumento(argc, argv, 4, ub, 4*TAM) :
                        (tarefa == 3 ? "Gondor_Border" : "Mordor_Gate");
        if (strlen(o) >= TAM || strlen(d) >= TAM
            || caracteres(o) > 31 || caracteres(d) > 31)
            erro("nome consultado muito longo");
        strcpy(a, o); strcpy(b, d);
        normalizar(a); normalizar(b);
        int origem = indice(&g, a), alvo = indice(&g, b);
        if (origem < 0 || alvo < 0) erro("vertice desconhecido");
        if (tarefa == 3) bfs(&g, origem, alvo);
        else dijkstra(&g, origem, alvo);
    }
    return EXIT_SUCCESS;
}
