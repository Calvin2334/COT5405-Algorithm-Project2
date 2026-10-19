
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <fstream>
#include <cmath>
using namespace std;

// Represents a green-light window
struct GreenWindow {
    int id;
    int start;
    int finish;
};

int main() {

    // Number of windows for each experiment
    vector<int> sizes = {1000, 5000, 10000, 25000, 50000, 100000, 200000};

    // Random number generator
    mt19937 rng(42);

    // Random start times
    uniform_int_distribution<int> startDist(0, 1000000);

    // Create CSV file to save experimental results
    ofstream csv("Greedy/results.csv");

    if (!csv.is_open()) {
        cerr << "Error: Could not open results.csv\n";
        return 1;
    }

    // CSV column headings
    csv << "InputSize,MedianTimeMs,NLogN\n";

    for (int n : sizes) {

        vector<GreenWindow> windows;

        // Generate random green-light windows
        for (int i = 0; i < n; i++) {
            int start = startDist(rng);
            int finish = start + 1 + (rng() % 100);

            windows.push_back({i + 1, start, finish});
        }

        cout << "Generated " << windows.size()
             << " green-light windows\n";

        vector<double> times;

        // Repeat each experiment 10 times
        for (int repeat = 0; repeat < 10; repeat++) {

            // Fresh copy of the original unsorted input
            vector<GreenWindow> testWindows = windows;

            // Start timer
            auto startTime = chrono::steady_clock::now();

            // Sort windows by earliest finishing time
            sort(testWindows.begin(), testWindows.end(),
                [](const GreenWindow& a, const GreenWindow& b) {
                    return a.finish < b.finish;
                });

            // Greedy selection
            vector<GreenWindow> selected;
            int lastFinish = -1;

            for (const auto& window : testWindows) {
                if (window.start >= lastFinish) {
                    selected.push_back(window);
                    lastFinish = window.finish;
                }
            }

            // Stop timer
            auto endTime = chrono::steady_clock::now();

            // Calculate elapsed time in milliseconds
            chrono::duration<double, milli> duration = endTime - startTime;

            times.push_back(duration.count());
        }

        // Sort the measurements to calculate the median
        sort(times.begin(), times.end());

        // Median of 10 measurements
        double medianTime = (times[4] + times[5]) / 2.0;

        // Calculate theoretical O(n log n) growth
        double theoretical = n * log2(n);

        // Save results to CSV
        csv << n << ","
            << medianTime << ","
            << theoretical << "\n";

        // Display results
        cout << "Input size: " << n
             << " | Median runtime: " << medianTime
             << " ms\n\n";
    }

    // Close CSV file
    csv.close();

    cout << "Experimental results saved to Greedy/results.csv\n";

    return 0;
}
