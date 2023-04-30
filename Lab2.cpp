#include <iostream>
#include <stdlib.h>
#include <climits>
#include <ctime>
#include <chrono>
#include <queue>
#include <fstream>


using namespace std;

int** fillMatrix(int Num, int connect = 0) {
    int** grapghMatrix = new int* [Num];
    for (int i = 0; i < Num; i++) {
        grapghMatrix[i] = new int[Num];
    }

    for (int i = 0; i < Num; i++) {
        for (int j = 0; j < Num; j++) {
            grapghMatrix[i][j] = 0;
        }
    }


    //генерация граней
    int edges = 0;

    switch (connect) {
    case 2:
        edges = ((Num * (Num - 1)) / 2); break;
    case 1:
        edges = Num + rand() % (int((Num * (Num - 1)) / 2) - Num + 1); break;
    case 0:
        edges = Num + rand() % (int((Num * (Num - 1)) / 4) - Num + 1); break;
    default:break;
    }


    int e = 0;
    int v1 = 0, v2 = 0;
    while (e < edges)
    {
        while (v1 == v2) {
            v1 = rand() % Num; v2 = rand() % Num;
            if (grapghMatrix[v1][v2] != 0) {
                v1 = 0; v2 = 0;
            }
        }
        grapghMatrix[v1][v2] = 1 + (rand() % 50); grapghMatrix[v2][v1] = grapghMatrix[v1][v2];
        e += 1; v1 = 0; v2 = 0;
    }

    bool connected = false; v2 = 0;
    for (int i = 0; i < Num; i++) {
        connected = false; v2 = i;
        for (int j = 0; j < Num; j++) {
            if (grapghMatrix[i][j] != 0) { connected = true; break; }
        }
        if (connected == false) {
            while (v2 == i) { v2 = rand() % Num; }
            grapghMatrix[i][v2] = 1 + (rand() % 50); grapghMatrix[v2][i] = grapghMatrix[i][v2];
        }
    }

    return grapghMatrix;
}

//стандарт

int minDistance(int dist[], bool marked[],int Num)
{

    //задаем расстояния как бесконечность
    int min = INT_MAX, min_index;

    for (int v = 0; v < Num; v++)
        if (marked[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

int* dijkstra_std(int** graphMatrix, int Num,int Src){
    
    int* dist = new int[Num];
    bool* marked = new bool[Num]; 

    for (int i = 0; i < Num; i++)
        dist[i] = INT_MAX, marked[i] = false;

    dist[Src] = 0;

    for (int count = 0; count < Num - 1; count++) {

        int u = minDistance(dist, marked,Num);

        marked[u] = true;

        for (int v = 0; v < Num; v++) {
            if (!marked[v] && graphMatrix[u][v] && dist[u] != INT_MAX && dist[u] + graphMatrix[u][v] < dist[v]) {
                dist[v] = dist[u] + graphMatrix[u][v];
            }
        }
    }

    return dist;
}

//fибоначчи

class FibonacciHeap {
public:
    int vertex;
    int dist;

    FibonacciHeap(int vertex, int dist) : vertex(vertex), dist(dist) {}

    //оператор для сортировки и сравнения
    bool operator<(const FibonacciHeap& other) const {
        return dist > other.dist;
    }
};

int* dijkstra_fib(int** graphMatrix,int Num,int Src) {
    
    int* dist = new int[Num];
    bool* marked = new bool[Num];
    for (int i = 0; i < Num; i++) {
        dist[i] = INT_MAX;
        marked[i] = false;
    }
    dist[Src] = 0;

    priority_queue<FibonacciHeap> pq;
    pq.push(FibonacciHeap(Src, 0));

    while (!pq.empty()) {
        int u = pq.top().vertex;
        pq.pop();

        if (marked[u]) {
            continue;
        }
        marked[u] = true;

        for (int i = 0; i < Num;i++) {
            if (!marked[i]) {
                if (dist[i] > dist[u] + graphMatrix[i][u]) {
                    dist[i] = dist[u] + graphMatrix[i][u];
                    pq.push(FibonacciHeap(i, dist[i]));
                }
            }
        }
    }

    return dist;
}

//двоичная
namespace BH {
    const int max_n = 3000;
    const int max_e = 1000000;


    int h[max_n];
    int pos2Id[max_n];
    int id2Pos[max_n];
    int BH_size;

    void BH_swap(int i, int j) {
        std::swap(h[i], h[j]);
        std::swap(pos2Id[i], pos2Id[j]);
        std::swap(id2Pos[pos2Id[i]], id2Pos[pos2Id[j]]);
    }

    void BH_Up(int pos) {
        while (pos > 0) {
            int parent = (pos - 1) >> 1;
            if (h[pos] >= h[parent]) {
                break;
            }
            BH_swap(pos, parent);
            pos = parent;
        }
    }

    void BH_add(int id, int prio) {
        h[BH_size] = prio;
        pos2Id[BH_size] = id;
        id2Pos[id] = BH_size;
        BH_Up(BH_size++);
    }

    void BH_PriorityUp(int id, int prio) {
        int pos = id2Pos[id];
        h[pos] = prio;
        BH_Up(pos);
    }

    void BH_Down(int pos) {
        while (pos < (BH_size >> 1)) {
            int child = 2 * pos + 1;
            if (child + 1 < BH_size && h[child + 1] < h[child]) {
                ++child;
            }
            if (h[pos] <= h[child]) {
                break;
            }
            BH_swap(pos, child);
            pos = child;
        }
    }

    int BH_ExtractMin() {
        int res = pos2Id[0];
        int lastNode = h[--BH_size];
        if (BH_size > 0) {
            h[0] = lastNode;
            int id = pos2Id[BH_size];
            id2Pos[id] = 0;
            pos2Id[0] = id;
            BH_Down(0);
        }
        return res;
    }

    int* dijkstra_bin(int** graphMatrix, int Num, int Src) {

        int* marked = new int[Num];
        int* dist = new int[Num];
        for (int i = 0; i < Num; i++) {
            dist[i] = INT_MAX; marked[i] = -1;
        }

        dist[Src] = 0;
        BH_size = 0;
        BH_add(Src, 0);

        while (BH_size) {
            int u = BH_ExtractMin();
            for (int e = 0; e < Num; e++) {
                int ndist = dist[u] + graphMatrix[e][u];
                if (dist[e] > ndist) {
                    if (dist[e] == INT_MAX) {
                        BH_add(e, ndist);
                    }
                    else {
                        BH_PriorityUp(Num, ndist);
                    }
                    dist[e] = ndist;
                    marked[e] = u;
                }
            }
        }
        return dist;
    }

};

//main
int main()
{
    int Num, Src = 0;
    srand((unsigned)time(0));

    ofstream out_std("std_high.txt");
    ofstream out_bin("bin_high.txt");
    ofstream out_fib("fib_high.txt");

    for (int Num = 10; Num <= 3000; Num += 10) {

        int** graphMatrix = fillMatrix(Num, 2);

        //стандарт
        auto start = std::chrono::high_resolution_clock::now();
        int* std_dist = dijkstra_std(graphMatrix, Num, Src);
        auto end = std::chrono::high_resolution_clock::now();

        out_std << chrono::duration_cast<chrono::microseconds>(end - start).count() << endl;
        cout << "Std: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " ";

        //двоичная
        start = std::chrono::high_resolution_clock::now();
        int* bin_dist = BH::dijkstra_bin(graphMatrix, Num, Src);
        end = std::chrono::high_resolution_clock::now();

        out_bin << chrono::duration_cast<chrono::microseconds>(end - start).count() << endl;
        cout << "Bin: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " ";

        //фибоначчи
        start = std::chrono::high_resolution_clock::now();
        int* fib_dist = dijkstra_fib(graphMatrix, Num, Src);
        end = std::chrono::high_resolution_clock::now();

        out_fib << chrono::duration_cast<chrono::microseconds>(end - start).count() << endl;
        cout << "Fib: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << endl;

    }
}
