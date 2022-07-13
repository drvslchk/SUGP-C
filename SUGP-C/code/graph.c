#include "graph.h"

struct Graph *createGraph(int vertices, int edges, int rows, int columns) { //funckja tworząca graf
    struct Graph *graph = malloc(sizeof(struct Graph)); //tworzymy graf i przydzielamy dla niego pamięć
    graph->numVertices = vertices; //zapisujemy ilość wierzchołków
    graph->numEdges = edges; //zapisujemy ilość krawędzi
    graph->columns = columns; //zapisujemy ilość kolumn
    graph->rows = rows; //zapisujemy ilość wierszy
    graph->adjLists = malloc(vertices * sizeof(struct Node *)); //przydzielamy pamięć do listy sąsiedctwa
    graph->visited = (int*)(malloc(vertices * sizeof(int))); //przydzielamy pamięć dla odwiedzonych wierzchołków

    for (int i = 0; i < vertices; i++) { //zerujemy tablicy
        graph->adjLists[i] = NULL; //lista sąsiedctwa równa NULL
        graph->visited[i] = 0; //nie odwiedzone wierzchołki równe 0
    }
    return graph; //zwracamy utworzony graf
}

double generateWeight(double minValue, double maxValue) { //funkcja dla generacji wagi
    double weight = (double) (rand()) / RAND_MAX * (maxValue - minValue) + minValue; //generujemy liczbę double w zakresie od minValue do maxValue
    return weight; //zwracamy utworzoną liczbę
}

void addEdge(struct Graph *graph, int src, int dest, double weight) { // funckja dodająca krawędź do grafu
    struct Node *newNode = createNode(dest, weight); //tworzymy nową krawędź
    newNode->next = graph->adjLists[src]; //wskaźnik na następną krawędź, jest to wskaźnik na pierwszą krawędź w liście sąsiedctwa
    graph->adjLists[src] = newNode; //zapisujemy utworzoną krawędź w listę sąsiedctwa
    newNode = createNode(src, weight); //robimy to samo, ale teraz nie z х в у???, a z у в х???
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

void bfs(struct Graph *graph, int startVertex) { //algorytm bfs
    struct queue *q = createQueue(graph->numEdges); //tworzymy kolejkę

    graph->visited[startVertex] = 1; //oznaczmy pierwszy wierzchołek jako odwiedzony
    enqueue(q, startVertex); //dodajemy ten wierzchołej do kolejki

    while (!isEmpty(q)) { //dopóki kolejka nie jest pusta
        int currentVertex = dequeue(q); //wyjmujemy element z kolejki
        struct Node *temp = graph->adjLists[currentVertex]; //tworzymy jednostkę z wierchołkami, do których można trafić z otrzymanej kolejki

        while (temp) { //dopóki ta jednostka nie jest pustaя
            int adjVertex = temp->vertex; //wyjmujemy nowy wierzchołek

            if (graph->visited[adjVertex] == 0) { //jeżeli nie jest odwiedzony
                graph->visited[adjVertex] = 1; //zaznaczamy jako visited
                enqueue(q, adjVertex); //dodajemy do kolejki
            }
            temp = temp->next; //przechodzimy do kolejnego wierzchołka
        }
        free(temp); //zwalniamy pamięć
    }
}

int conectivityCheck(struct Graph *graph) { //funkcja sprawdzająca graf na spójnośc
    int v = 0; //bieżący wierczhołek
    while (v < graph->numVertices && graph->adjLists[v] == NULL) v++; //dopóki bieżący wierczhołęk nie ma połączeń, zwiększamy wartość bieżącego wierzchołka
    bfs(graph, v); //uruchamiamy bfs ze znalezionego wierzchołka
    for (int i = 0; i < graph->numVertices; i++) { //idziemy po wszystkich wierzchołkach grafu
        if (graph->visited[i] == 0) return -1; //jeżeli któryś wierzchołek jest nie odwiedzony, to znaczy że graf jest niespójny - zwracamy -1
    }
    return 1; //jeżeli graf jest spójny - zwracamy 1
}

struct Graph* generateGraph(int rows, int columns, double minValue, double maxValue) { //funkcja dla generacji grafu
    srand(time(NULL)); //włączamy randomowe jądro generacji
    int cntVectices = rows * columns; //odczytujemy ilość wierzchołków
    int cntEdges = (2 * rows * columns) - rows - columns; //odczytujemy ilość krawędzi
    struct Graph *graph = createGraph(cntVectices, cntEdges, rows, columns); //tworzymy graf
    for (int i = 0; i < rows; i++) { //idziemy po wszystkich wierszach
        for (int j = 0; j < columns; j++) { //i wszystkich kolumnach
            int v = i * columns + j; //znajdujemy numer bieżącego wierzchołku
            if ((v + 1) % columns > 0) { //jeżeli ten wierzchołek nie jest ostatnim z prawej strony
                addEdge(graph, v, v + 1, generateWeight(minValue, maxValue)); //tworzymy krawędź z bieżącej krawędzi do prawej
            }
            if (i < rows - 1) { //jeżeli ta krawędź nie jest w dolnym rzędzie
                addEdge(graph, v, v + columns, generateWeight(minValue, maxValue)); //tworzymy krawędź z bieżącego wierzchołku do dolnego
            }
        }
    }
    return graph; //zwracamy utworzony graf
}

double dijkstra(struct Graph *graph, int start, int finish) { //funkcja Dijkstry
    if (conectivityCheck(graph) == -1) { //sprawdzamy cze graf jest spójny i jeżeli nie, to wyświetlamy komuniakt o błędzie
        printf("Inconsistent graph");
        exit(-1);
    }
    double *dist = (double *) malloc(graph->numVertices * sizeof(double)); //tworzymy tablicę gdzie będziemy przechowywać długość od wierzchołka startowego do pozostałych i przydzielamy jej pamięć
    const int INF = 1e9; //const oznaczająca nieskończoność
    double min = 0; //zmienna do przechowywania minimum
    int minindex = 0; //zmienna przechowująca adres minimuma
    for (int i = 0; i < graph->numVertices; i++) { //zerujemy tablicy odwiedzonych wierzchołków, а długości wypełniamy nieskończonościami 
        graph->visited[i] = 0;
        dist[i] = INF;
    }

    dist[start] = 0; //długości do wierzczhołka startowego robimy równą 0
    do { //robimy cykl dopóki nie obejdziemy cały graf
        minindex = INF; //adres równy nieskończoności
        min = INF; //wartość minimalna równa nieskończoności
        for (int i = 0; i < graph->numVertices; i++) { //dla wszystkich wierzchołków grafu
            if (graph->visited[i] == 0 && dist[i] < min) { //jeżeli wierzchołek nie jest odwiedzony i długość do niego jest mniejsza od monimum
                min = dist[i]; //minimum się staję tą wartością(długością)
                minindex = i; //adres wskazuje na ten wierzchołek
            }
        }
        if (minindex != INF) { //jeżeli znaleźliśmy jakiś adres, długość do którego nam odpowiada
            struct Node *temp = graph->adjLists[minindex]; // tworzymy jednostkę dla iteracji wszystkich wierzchołków, do których możemy trafić z danego adresu
            while (temp) { //dopóki ta jednostka nie jest pusta
                if (min + temp->weight < dist[temp->vertex]) { //jeżeli suma min + waga bieżącej krawędzi bieżącej krawędzi jest mniejsza, niż bieżąca długość do tej krawędzi, to aktualizujemy długośc
                    dist[temp->vertex] = min + temp->weight;
                }
                temp = temp->next; //przechodzimy do następnego wierczhołka
            }
            free(temp); //zwalniamy pamięć
            graph->visited[minindex] = 1; //oznaczamy bieżący wierzchołek z numerem indeks jako odwiedzoną
        }
    } while (minindex != INF);
    double distance = dist[finish]; //długość od startowego do końcowego wierzchołka przechowywana jest w jednostce z indeksem finish
    free(dist); //znowu zwalniamy pamięć
    return distance; //zwracamy długość od wierzchołka startowego do potrzebnego
}

double findDistanseBetweenVertices(struct Graph *graph, int startVertex, int destinationVertex) { //funkcja dla wyszukiwania długości między dwoma wierczhołkami
    if (startVertex < 0 || startVertex > graph->numVertices - 1 || destinationVertex < 0 || //jeżeli dane wejściowe są niepoprawne - wyświetlamy komunikat z błędem
        destinationVertex > graph->numVertices - 1) {
        printf("Invalid program invocation arguments.");
        exit(-1);
    }
    double dist = dijkstra(graph, startVertex, destinationVertex); //szukamy długość pomiędzy wierzchołkami
    return dist; //zwracamy długość
}
