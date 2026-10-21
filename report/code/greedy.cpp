
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Structure to represent a green-light window
struct GreenWindow {
    int id;
    int start;
    int finish;
};

int main() {
    vector<GreenWindow> windows = {
        {1, 0, 10},
        {2, 2, 5},
        {3, 5, 8},
        {4, 7, 12},
        {5, 8, 14}
    };

    cout << "Available Green-Light Windows:\n";

    for (const auto& window : windows) {
        cout << "Window " << window.id
             << ": " << window.start
             << " - " << window.finish
             << " seconds\n";
    }

    sort(windows.begin(), windows.end(), [](const GreenWindow& a, const GreenWindow& b) {
        return a.finish < b.finish;
    });

    vector<GreenWindow> selected;
    int lastFinish = -1;

    for (const auto& window : windows) {
        if (window.start >= lastFinish) {
            selected.push_back(window);
            lastFinish = window.finish;
        }
    }

    cout << "\nSelected Green-Light Windows:\n";

    for (const auto& window : selected) {
        cout << "Window " << window.id
             << ": " << window.start
             << " - " << window.finish
             << " seconds\n";
    }

    cout << "Total selected: " << selected.size() << '\n';

    return 0;
}
