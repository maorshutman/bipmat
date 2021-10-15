import argparse

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

plt.rcParams['axes.linewidth'] = 0.5


def parse_args():
  parser = argparse.ArgumentParser(description='')
  parser.add_argument('--data_file_path', type=str, required=True, default='', help='')
  return parser.parse_args()


def main():
  args = parse_args()

  df = pd.read_csv(args.data_file_path)
  df.columns = ["prob_size", "sol_time"]

  prob_sizes = sorted(df["prob_size"].unique())

  mean_sol_time = []
  for sz in prob_sizes:
    mean_sol_time.append(df[df["prob_size"] == sz]["sol_time"].mean())

  plt.plot(prob_sizes, mean_sol_time, linewidth=2)
  plt.xlabel("Problem size", style="italic")
  plt.ylabel("Time [sec]", style="italic")
  plt.yscale("log")
  plt.xscale("log")

  plt.grid(linestyle="--", linewidth=1)
  plt.show()


if __name__ == "__main__":
  main()
