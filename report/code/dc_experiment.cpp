#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <fstream>
#include <cmath>
#include "max_subarray.h"
using namespace std;

// O(n^2) reference used to check correctness on small inputs
long long bruteForce(const vector<int>& x) {
    long long best = x[0];
    for (size_t i = 0; i < x.size(); i++) {
        long long sum = 0;
        for (size_t j = i; j < x.size(); j++) {
            sum += x[j];
            best = max(best, sum);
        }
    }
    return best;
}

// O(n) reference used to check correctness on large inputs
long long kadane(const vector<int>& x) {
    long long best = x[0];
    long long current = 0;
    for (int value : x) {
        current = max((long long)value, current + value);
        best = max(best, current);
    }
    return best;
}

// Confirms that the reported period actually has the reported sum
bool consistent(const vector<int>& x, const Result& r) {
    long long sum = 0;
    for (int i = r.left; i <= r.right; i++) {
        sum += x[i];
    }
    return r.left <= r.right && sum == r.sum;
}

// Keeps the compiler from optimizing away repeated runs
volatile long long sink = 0;

int main() {

    // Random number generator
    mt19937 rng(42);

    // Net vehicles per minute (arrivals minus departures)
    uniform_int_distribution<int> netDist(-30, 30);

    // Correctness check against brute force on small inputs
    uniform_int_distribution<int> smallSize(1, 60);
    for (int trial = 0; trial < 2000; trial++) {
        vector<int> x(smallSize(rng));
        for (int& value : x) {
            value = netDist(rng);
        }
        Result r = maxSubarray(x);
        if (r.sum != bruteForce(x) || !consistent(x, r)) {
            cerr << "Mismatch with brute force on trial " << trial << '\n';
            return 1;
        }
    }
    cout << "Brute-force correctness check passed (2000 inputs)\n\n";

    // Number of minutes for each experiment
    vector<int> sizes = {10000, 20000, 50000, 100000, 200000, 500000,
                         1000000, 2000000, 4000000, 8000000, 16000000};

    // Create CSV file to save experimental results
    ofstream csv("DivideConquer/results.csv");

    if (!csv.is_open()) {
        cerr << "Error: Could not open results.csv\n";
        return 1;
    }

    // CSV column headings
    csv << "InputSize,MedianTimeMs,NLogN\n";

    for (int n : sizes) {

        // Generate random per-minute net queue changes
        vector<int> x(n);
        for (int& value : x) {
            value = netDist(rng);
        }

        Result r = {0, 0, 0};
        vector<double> times;

        // Small inputs finish faster than the clock resolution, so each
        // measurement runs the algorithm several times and averages
        int runsPerMeasurement = max(1, 4000000 / n);

        // Repeat each experiment 10 times
        for (int repeat = 0; repeat < 10; repeat++) {

            // Start timer
            auto startTime = chrono::steady_clock::now();

            for (int run = 0; run < runsPerMeasurement; run++) {
                r = maxSubarray(x);
                sink = sink + r.sum;
            }

            // Stop timer
            auto endTime = chrono::steady_clock::now();

            // Calculate elapsed time per run in milliseconds
            chrono::duration<double, milli> duration = endTime - startTime;

            times.push_back(duration.count() / runsPerMeasurement);
        }

        // Verify the large-input answer with Kadane's algorithm
        if (r.sum != kadane(x) || !consistent(x, r)) {
            cerr << "Mismatch with Kadane for n = " << n << '\n';
            return 1;
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
             << " ms | Best period sum: " << r.sum << "\n";
    }

    // Close CSV file
    csv.close();

    cout << "\nExperimental results saved to DivideConquer/results.csv\n";

    return 0;
}
