import csv
import numpy as np
import matplotlib.pyplot as plt

sizes = []
times = []
theoretical = []

with open("DivideConquer/results.csv", newline="") as file:
    reader = csv.DictReader(file)

    for row in reader:
        sizes.append(int(row["InputSize"]))
        times.append(float(row["MedianTimeMs"]))
        theoretical.append(float(row["NLogN"]))

sizes = np.array(sizes, dtype=float)
times = np.array(times)
theoretical = np.array(theoretical)

# Pure O(n log n) curve scaled to the largest input size
scaled_theory = theoretical * (times[-1] / theoretical[-1])

# Time per element in nanoseconds; if T(n) = a n log2 n + b n,
# then T(n) / n = a log2 n + b is a straight line in log2 n
per_element = times * 1e6 / sizes
log_n = np.log2(sizes)
slope, intercept = np.polyfit(log_n, per_element, 1)
line = slope * log_n + intercept
ss_res = np.sum((per_element - line) ** 2)
ss_tot = np.sum((per_element - per_element.mean()) ** 2)
r_squared = 1 - ss_res / ss_tot

# Fitted model T(n) = a n log2 n + b n, converted back to ms
fitted = line * sizes / 1e6

print(f"Fit: T(n)/n = {slope:.3f} * log2 n + {intercept:.3f} ns, R^2 = {r_squared:.4f}")

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(7, 8))

ax1.plot(sizes / 1e6, times, marker="o", label="Measured median runtime")
ax1.plot(sizes / 1e6, scaled_theory, linestyle="--", label="Scaled O(n log n)")
ax1.plot(sizes / 1e6, fitted, linestyle=":", label="Fit a n log n + b n")
ax1.set_xlabel("Number of minutes (n, millions)")
ax1.set_ylabel("Execution time (ms)")
ax1.set_title("Divide and Conquer: Experimental Runtime Analysis")
ax1.legend()
ax1.grid(True, alpha=0.3)

ax2.plot(log_n, per_element, marker="o", label="Measured time / n")
ax2.plot(log_n, line, linestyle="--",
         label=f"Linear fit in log n (R$^2$ = {r_squared:.3f})")
ax2.set_xlabel("log$_2$ n")
ax2.set_ylabel("Time per minute of data (ns)")
ax2.set_title("Normalized Runtime: T(n) / n vs. log$_2$ n")
ax2.legend()
ax2.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig("DivideConquer/dc_runtime_graph.png", dpi=300)
plt.show()
