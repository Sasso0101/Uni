import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from collections import namedtuple
import os

def collect_data(path, marker, color, label):
  print("Plotting data from ", path)
  data = []
  for filename in os.listdir(path):
    with open(path + filename, 'r') as file:
      size = filename.split('_')[0]
      value = file.readline()
      data.append((int(size), float(value)))
  
  df = pd.DataFrame(data, columns=['size', 'time'])
  avg = df.groupby(['size']).mean().reset_index()
  ax1.plot('size', 'time', data=avg, marker=marker, color=color, linestyle='none', label=label)
  avg['flops'] = (avg['size']**2)*2/1e9/avg['time']
  ax2.plot('size', 'flops', data=avg, marker=marker, color=color, linestyle='none', label=label)

# Initialize plot
fig1, ax1 = plt.subplots()
ax1.set_xlabel("Matrix size")
ax1.set_xscale("log", base=2)
ax1.set_ylabel("Runtime [s]")
ax1.set_yscale("log", base=2)
ax1.grid(True, which="both", ls="-")
ax1.xaxis.set_major_formatter(ticker.ScalarFormatter())
ax1.yaxis.set_major_formatter(ticker.StrMethodFormatter("{x:.3f}"))

fig2, ax2 = plt.subplots()
ax2.set_xlabel("Matrix size")
ax2.set_xscale("log", base=2)
ax2.set_ylabel("Bandwidth [GFLOPS/s]")
ax2.grid(True, which="both", ls="-")
ax2.xaxis.set_major_formatter(ticker.ScalarFormatter())
ax2.yaxis.set_major_formatter(ticker.ScalarFormatter())

collect_data("results/sequential.out/", 'o', 'r', 'no optimizations')
collect_data("results/sequential-unroll.out/", '^', 'g', 'unrolled')
collect_data("results/sequential-unroll-native.out/", 's', 'b', 'unrolled + native')
collect_data("results/sequential-unroll-native-vect.out/", '*', 'purple', 'unrolled + native + vectorized')

ax1.legend()
ax2.legend()

fig1.savefig("results/plots/sequential_time.svg")
fig2.savefig("results/plots/sequential_flops.svg")
