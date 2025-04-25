#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

bool bfs(vector<vector<int>>& capacity, vector<int>& parent, int source, int sink) {
    int n = capacity.size();
    fill(parent.begin(), parent.end(), -1);
    queue<pair<int, int>> q;
    q.push({ source, INT_MAX });

    while (!q.empty()) {
        int current = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int next = 0; next < n; ++next) {
            if (parent[next] == -1 && capacity[current][next] > 0) { 
                parent[next] = current;
                int new_flow = min(flow, capacity[current][next]);
                if (next == sink) {
                    return new_flow;
                }
                q.push({ next, new_flow });
            }
        }
    }
    return 0;
}


int edmondsKarp(vector<vector<int>> capacity, int source, int sink, vector<vector<int>>& flow_routes) {
    int n = capacity.size();
    int max_flow = 0;
    vector<int> parent(n);
    int new_flow;

    while ((new_flow = bfs(capacity, parent, source, sink))) {
        max_flow += new_flow;

        int current = sink;
        vector<int> path;
        while (current != source) {
            int prev = parent[current];
            capacity[prev][current] -= new_flow;
            capacity[current][prev] += new_flow;
            path.push_back(current);
            current = prev;
        }
        path.push_back(source);
        reverse(path.begin(), path.end());
        flow_routes.push_back(path);
    }

    return max_flow;
}

bool hamiltonianDFS(int v, vector<bool>& visited, vector<int>& path, vector<vector<int>>& matrix, int totalVertices) {
    visited[v] = true;
    path.push_back(v);

    if (path.size() == totalVertices) {
        return true; 
    }

    for (int next = 0; next < totalVertices; ++next) {
        if (matrix[v][next] > 0 && !visited[next]) { 
            if (hamiltonianDFS(next, visited, path, matrix, totalVertices)) {
                return true;
            }
        }
    }

    visited[v] = false; 
    path.pop_back();
    return false;
}

vector<int> findHamiltonianPath(vector<vector<int>>& matrix) {
    int totalVertices = matrix.size();
    vector<bool> visited(totalVertices, false);
    vector<int> path;

    for (int start = 0; start < totalVertices; ++start) {
        if (hamiltonianDFS(start, visited, path, matrix, totalVertices)) {
            return path;
        }
    }

    return {}; 
}

int main() {
    setlocale(LC_ALL, "RUS");
    vector<vector<int>> matrix =
    {
        { 0, 0, 0, 7, 6, 0, 1, 4, 4, 5, 6, 6, 7 },
        { 0, 0, 6, 4, 4, 1, 2, 3, 2, 0, 1, 4, 2 },
        { 0, 6, 0, 8, 8, 4, 3, 6, 5, 3, 6, 6, 5 },
        { 7, 4, 8, 0, 5, 4, 5, 8, 0, 9, 3, 6, 8 },
        { 6, 4, 8, 5, 0, 1, 4, 2, 7, 7, 7, 2, 0 },
        { 0, 1, 4, 4, 1, 0, 7, 4, 4, 2, 4, 2, 6 },
        { 1, 2, 3, 5, 4, 7, 0, 7, 1, 2, 2, 9, 8 },
        { 4, 3, 6, 8, 2, 4, 7, 0, 8, 4, 2, 3, 2 },
        { 4, 2, 5, 0, 7, 4, 1, 8, 0, 2, 5, 8, 1 },
        { 5, 0, 3, 9, 7, 2, 2, 4, 2, 0, 5, 9, 7 },
        { 6, 1, 6, 3, 7, 4, 2, 2, 5, 5, 0, 9, 6 },
        { 6, 4, 6, 6, 2, 2, 9, 3, 8, 9, 9, 0, 5 },
        { 7, 2, 5, 8, 0, 6, 8, 2, 1, 7, 6, 5, 0 }
    };

    int source = 0;      
    int sink = matrix.size() - 1; 

    vector<vector<int>> flow_routes;
    int max_flow = edmondsKarp(matrix, source, sink, flow_routes);

    cout << "Суммарный максимальный поток: " << max_flow << "\n";
    cout << "Маршруты потоков:\n";
    for (auto& route : flow_routes) {
        for (int node : route) {
            cout << node << " ";
        }
        cout << "\n";
    }

    vector<int> hamiltonianPath = findHamiltonianPath(matrix);

    if (!hamiltonianPath.empty()) {
        cout << "Гамильтонов путь: ";
        for (int v : hamiltonianPath) {
            cout << v << " ";
        }
        cout << "\n";
    }
    else {
        cout << "Гамильтонов путь не найден.\n";
    }

    return 0;
}
