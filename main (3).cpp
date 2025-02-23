#include <iostream>
#include <cmath>
using namespace std;
struct Point {
    double x, y;
};
double euclidean_distance(const Point &a, const Point &b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}
double** compute_distance_matrix(Point* points, int n) {
    double** dist = new double*[n];
    for (int i = 0; i < n; i++) {
        dist[i] = new double[n];
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j)
                dist[i][j] = 0.0;
            else
                dist[i][j] = euclidean_distance(points[i], points[j]);
        }
    }
    return dist;
}
int* nearest_neighbor(double** dist, int n) {
    bool* visited = new bool[n];
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }
    int* tour = new int[n];
    tour[0] = 0;
    visited[0] = true;
    int current = 0;
    for (int i = 1; i < n; i++) {
        int next = -1;
        double best = numeric_limits<double>::max();
        for (int j = 0; j < n; j++) {
            if (!visited[j] && dist[current][j] < best) {
                best = dist[current][j];
                next = j;
            }
        }
        tour[i] = next;
        visited[next] = true;
        current = next;
    }
    delete [] visited;
    return tour;
}
double compute_length(int* tour, double** dist, int n, bool cycle) {
    double length = 0.0;
    for (int i = 0; i < n - 1; i++) {
        length += dist[tour[i]][tour[i + 1]];
    }
    if (cycle) {
        length += dist[tour[n - 1]][tour[0]];
    }
    return length;
}
void reverse_segment(int* arr, int start, int end) {
    while (start < end) {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}
bool two_opt_swap(int* tour, double** dist, int n) {
    double oldLength = compute_length(tour, dist, n, true);
    for (int i = 0; i < n - 1; i++) {
        for (int k = i + 1; k < n; k++) {
            int* new_tour = new int[n];
            for (int m = 0; m < n; m++) {
                new_tour[m] = tour[m];
            }
            reverse_segment(new_tour, i + 1, k);
            double newLength = compute_length(new_tour, dist, n, true);
            if (newLength < oldLength) {
                for (int m = 0; m < n; m++) {
                    tour[m] = new_tour[m];
                }
                delete [] new_tour;
                return true;
            }
            delete [] new_tour;
        }
    }
    return false;
}

int main() {
    int n;
    cin >> n;
    Point* points = new Point[n];
    for (int i = 0; i < n; i++) {
        cin >> points[i].x >> points[i].y;
    }
    double** dist = compute_distance_matrix(points, n);
    int* tour = nearest_neighbor(dist, n);
    while (two_opt_swap(tour, dist, n)) {
    }
    bool is_cycle = true;
    double length = compute_length(tour, dist, n, is_cycle);
    cout << length << "\n";
    cout << (is_cycle ? 1 : 0) << "\n";
    for (int i = 0; i < n; i++) {
        cout << tour[i] + 1 << " ";
    }
    cout << "\n";
    delete [] tour;
    delete [] points;
    for (int i = 0; i < n; i++) {
        delete [] dist[i];
    }
    delete [] dist;
    return 0;
}
