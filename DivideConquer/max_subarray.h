#ifndef MAX_SUBARRAY_H
#define MAX_SUBARRAY_H

#include <vector>
using namespace std;

// Result of a search: the period [left, right] (0-based, inclusive)
// and the total queue build-up over that period
struct Result {
    int left;
    int right;
    long long sum;
};

// Best period that crosses the midpoint: it must contain
// minutes mid and mid + 1
Result maxCrossing(const vector<int>& x, int lo, int mid, int hi) {
    // Best suffix of the left half ending at mid
    long long leftSum = x[mid];
    long long running = 0;
    int bestLeft = mid;
    for (int i = mid; i >= lo; i--) {
        running += x[i];
        if (running > leftSum) {
            leftSum = running;
            bestLeft = i;
        }
    }

    // Best prefix of the right half starting at mid + 1
    long long rightSum = x[mid + 1];
    running = 0;
    int bestRight = mid + 1;
    for (int j = mid + 1; j <= hi; j++) {
        running += x[j];
        if (running > rightSum) {
            rightSum = running;
            bestRight = j;
        }
    }

    return {bestLeft, bestRight, leftSum + rightSum};
}

// Divide-and-conquer maximum subarray on x[lo..hi]
Result maxSubarray(const vector<int>& x, int lo, int hi) {
    // Base case: a single minute
    if (lo == hi) {
        return {lo, hi, x[lo]};
    }

    // Divide
    int mid = lo + (hi - lo) / 2;

    // Conquer
    Result leftBest = maxSubarray(x, lo, mid);
    Result rightBest = maxSubarray(x, mid + 1, hi);

    // Combine
    Result crossBest = maxCrossing(x, lo, mid, hi);

    if (leftBest.sum >= rightBest.sum && leftBest.sum >= crossBest.sum) {
        return leftBest;
    }
    if (rightBest.sum >= crossBest.sum) {
        return rightBest;
    }
    return crossBest;
}

// Convenience wrapper for a non-empty sequence
Result maxSubarray(const vector<int>& x) {
    return maxSubarray(x, 0, (int)x.size() - 1);
}

#endif
