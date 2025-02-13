import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import subprocess
import os
import csv

for root, dirs, files in os.walk('results/sequential'):
    for f in files:
        os.unlink(os.path.join(root, f))

print("Running sequential version with matrix sizes ", end="")

for i in range(10, 15):
  print(str(2**i) + "...", end="")
  subprocess.run(["likwid-perfctr -C 0 -o results/sequential/" + str(2**i) + ".csv -O -g CLOCK -m ./sequential.out " + str(2**i)] + " 2", shell=True)

print("\nSequential version done")
print("Reading output files...")

# Read files in results/sequential and plot
for filename in os.listdir("results/sequential"):
  header_row = 0
  # Skip all rows until the one with the header of the metrics table
  with open("results/sequential/" + filename, "r+") as f:
    rows = f.readlines()
    f.seek(0)
    for i, row in enumerate(rows):
      if "TABLE,Region transpose,Group 1 Metric" in str(row):
        f.writelines(rows[(i+1):])
        break
    f.truncate()

  data = pd.read_csv("results/sequential/" + filename, delimiter=',', usecols=range(0, 2))
  # Plot the runtime
  plt.plot(int(filename.replace(".csv", "")), data[data["Metric"] == "Runtime (RDTSC) [s]"]["Core 0"], label=filename, marker='o', color='b')

plt.xlabel("Matrix size")
plt.ylabel("Runtime [s]")
plt.yscale("log", base=2)
fig, ax = plt.subplots()
ax.yaxis.set_major_formatter(ticker.ScalarFormatter())
plt.savefig("sequential_plot.png")

print("Drawn plot to sequential_plot.png")
