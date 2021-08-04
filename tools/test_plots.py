
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np


def main():

  data_file_path = "/Users/maorshutman/repos/bipmat/tests/test_data.txt"

  df = pd.read_csv(data_file_path)
  df.columns = ["prob_size", "sol_time"]

  prob_sizes = sorted(df["prob_size"].unique())

  mean_sol_time = []
  for sz in prob_sizes:
    mean_sol_time.append(df[df["prob_size"] == sz]["sol_time"].mean())

  x = np.linspace(min(prob_sizes), max(prob_sizes), 1000)
  y = 1.5e-9 * (x**3)

  plt.plot(prob_sizes, mean_sol_time)
  plt.xlabel("Problem size")
  plt.ylabel("Time [sec]")
  plt.plot(x, y)
  plt.show()


if __name__ == "__main__":
  main()
