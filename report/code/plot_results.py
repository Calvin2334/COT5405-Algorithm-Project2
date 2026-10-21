import csv
import matplotlib.pyplot as plt

sizes = []
times = []
theoretical = []

with open("Greedy/results.csv", newline="") as file:
    reader = csv.DictReader(file)

    for row in reader:
        sizes.append(int(row["InputSize"]))
        times.append(float(row["MedianTimeMs"]))
        theoretical.append(float(row["NLogN"]))

# Scale the theoretical curve to match
# the measured runtime at the largest input size
scale = times[-1] / theoretical[-1]
scaled_theory = [value * scale for value in theoretical]

plt.figure(figsize=(9, 5))

plt.plot(
    sizes,
    times,
    marker="o",
    label="Measured median runtime"
)

plt.plot(
    sizes,
    scaled_theory,
    linestyle="--",
    label="Scaled O(n log n)"
)

plt.xlabel("Number of green-light windows (n)")
plt.ylabel("Execution time (ms)")
plt.title("Greedy Algorithm: Experimental Runtime Analysis")
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()

plt.savefig("Greedy/runtime_graph.png", dpi=300)
plt.show()