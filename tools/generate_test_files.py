import os
import random
import time
import json
import argparse

import numpy as np
from scipy.sparse import random
from scipy.optimize import linear_sum_assignment
from scipy.sparse import csr_matrix, csgraph


def save_test_files(
  input_file_path,
  output_file_path,
  fmt,
  min_cost,
  time_to_solve,
  rows,
  cols,
  costs
):

  if fmt == "matrix":
    with open(input_file_path, "w") as f:
      f.write(str(rows) + "\n")
      for i in range(rows):
        for j in range(cols):
          # TODO: deal with sparse case
          f.write(str(costs[i,j]) + " ")
        f.write("\n")

  elif fmt == "edges":
    with open(input_file_path, "w") as f:
      f.write(str(rows) + "\n")
      for i in range(rows):
        for j in range(cols):
          if costs[i,j] != np.inf:
            f.write(str(i) + " " + str(j) + " " + str(int(costs[i,j])) + "\n")

  with open(output_file_path, "w") as f:
    f.write(str(min_cost) + "\n")
    f.write(str(time_to_solve) + "\n")


def generate_dense_test_files(
  inputs_dir,
  outputs_dir,
  input_file_paths,
  output_file_paths,
  num_tests,
  fmt,
  rows,
  cols
):

  for i in range(num_tests):
    costs = np.random.randint(low=1, high=rows, size=(rows, cols))
    t0 = time.time()
    row_ind, col_ind = linear_sum_assignment(costs)
    min_cost = costs[row_ind, col_ind].sum()
    time_to_solve = time.time() - t0

    input_file_path = os.path.join(inputs_dir, f"{fmt}_{rows}x{cols}_input_{i}.txt")
    output_file_path = os.path.join(outputs_dir, f"{fmt}_{rows}x{cols}_output_{i}.txt")
    input_file_paths.append(input_file_path)
    output_file_paths.append(output_file_path)

    save_test_files(input_file_path, output_file_path, fmt, min_cost, time_to_solve, rows, cols, costs)


def generate_sparse_test_files(
  inputs_dir,
  outputs_dir,
  input_file_paths,
  output_file_paths,
  num_tests,
  fmt,
  rows,
  cols
):

  density = 0.5

  for i in range(num_tests):
    sparse = random(rows, cols, density=density, format='coo') * 10
    sparse.data = np.ceil(sparse.data)
    costs = sparse.toarray()
    costs = np.full(sparse.shape, np.inf)
    costs[sparse.row, sparse.col] = sparse.data

    try:
      t0 = time.time()
      row_ind, col_ind = linear_sum_assignment(costs)
      min_cost = costs[row_ind, col_ind].sum()
      time_to_solve = time.time() - t0
      print(min_cost)
    except:
      continue

    input_file_path = os.path.join(inputs_dir, f"sparse_{fmt}_{rows}x{cols}_input_{i}.txt")
    output_file_path = os.path.join(outputs_dir, f"sparse_{fmt}_{rows}x{cols}_output_{i}.txt")
    input_file_paths.append(input_file_path)
    output_file_paths.append(output_file_path)

    save_test_files(input_file_path, output_file_path, fmt, min_cost, time_to_solve, rows, cols, costs)



def parse_args():
  parser = argparse.ArgumentParser(description='')
  parser.add_argument('--inputs_dir', type=str, required=True, default='', help='')
  parser.add_argument('--outputs_dir', type=str, required=True, default='', help='')
  parser.add_argument('--mode', type=str, required=False, default='dense', help='')
  return parser.parse_args()


def main():

  args = parse_args()

  input_file_paths = []
  output_file_paths = []

  prob_sizes = [int(2 * (1.10**i)) for i in range(1, 70)]

  for idx, n in enumerate(prob_sizes):
    print(idx, n)
    if args.mode == "dense":
      generate_dense_test_files(
        args.inputs_dir, args.outputs_dir, input_file_paths, output_file_paths, 1, "edges", n, n
      )
    elif args.mode == "sparse":
      generate_sparse_test_files(
        args.inputs_dir, args.outputs_dir, input_file_paths, output_file_paths, 1, "edges", n, n
      )

  with open(os.path.join(args.inputs_dir, "inputs_file_list.txt"), "w") as f:
    for path in input_file_paths:
      f.write(path + "\n")

  with open(os.path.join(args.outputs_dir, "outputs_file_list.txt"), "w") as f:
    for path in output_file_paths:
      f.write(path + "\n")


if __name__ == "__main__":
    main()
