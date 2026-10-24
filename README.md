# COT5405-Algorithm-Project2
Greedy and Divide-and-Conquer Algorithms

## Divide and Conquer (worst queue build-up / maximum subarray)

Run from the repository root:

```
g++ -std=c++14 -O2 DivideConquer/max_subarray.cpp -o dc_demo && ./dc_demo
g++ -std=c++14 -O2 DivideConquer/experiment.cpp -o dc_exp && ./dc_exp
python DivideConquer/plot_results.py
```

The experiment writes `DivideConquer/results.csv`; the plot script writes
`DivideConquer/dc_runtime_graph.png` (requires `numpy` and `matplotlib`).
After re-running, copy the code into `report/code/` and the graph into `report/`.

The report uses `fontspec`, so compile `report/main.tex` with XeLaTeX or LuaLaTeX.
