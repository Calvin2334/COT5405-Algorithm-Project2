#include <iostream>
#include <vector>
#include "max_subarray.h"
using namespace std;

int main() {
    // Vehicles arriving and departing an intersection approach per minute
    vector<int> arrivals   = {12,  8, 14, 10,  6, 13, 16,  7, 15,  4};
    vector<int> departures = { 9, 10,  9, 11, 12,  9,  9, 10,  9, 12};

    // Net queue change per minute
    vector<int> net;
    for (size_t i = 0; i < arrivals.size(); i++) {
        net.push_back(arrivals[i] - departures[i]);
    }

    cout << "Minute | Arrivals | Departures | Net change\n";
    for (size_t i = 0; i < net.size(); i++) {
        cout << i + 1 << " | " << arrivals[i] << " | "
             << departures[i] << " | " << net[i] << '\n';
    }

    Result worst = maxSubarray(net);

    cout << "\nWorst queue build-up period: minutes "
         << worst.left + 1 << " to " << worst.right + 1 << '\n';
    cout << "Queue growth over that period: "
         << worst.sum << " vehicles\n";

    return 0;
}
