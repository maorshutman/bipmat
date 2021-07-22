import os
import random
import time
import json

import numpy as np
from scipy.optimize import linear_sum_assignment


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
                    f.write(str(costs[i,j]) + " ")
                f.write("\n")

    elif fmt == "edges":
        with open(input_file_path, "w") as f:
            f.write(str(rows) + "\n")
            for i in range(rows):
                for j in range(cols):
                    f.write(str(i) + " " + str(j) + " " + str(costs[i,j]) + "\n")

    with open(output_file_path, "w") as f:
        f.write(str(min_cost) + "\n")
        f.write(str(time_to_solve) + "\n")


def generate_test_files(
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
        # TODO: don't save abs paths, relative to tests

        save_test_files(input_file_path, output_file_path, fmt, min_cost, time_to_solve, rows, cols, costs)


def main():
    random.seed(10)

    # TODO: Add args

    inputs_dir = "/Users/maorshutman/repos/bipmat/tests/inputs"
    outputs_dir = "/Users/maorshutman/repos/bipmat/tests/outputs"

    input_file_paths = []
    output_file_paths = []

    generate_test_files(inputs_dir, outputs_dir, input_file_paths, output_file_paths, 10, "edges", 2, 2)
    generate_test_files(inputs_dir, outputs_dir, input_file_paths, output_file_paths, 10, "edges", 3, 3)
    generate_test_files(inputs_dir, outputs_dir, input_file_paths, output_file_paths, 10, "edges", 4, 4)
    generate_test_files(inputs_dir, outputs_dir, input_file_paths, output_file_paths, 10, "edges", 5, 5)
    generate_test_files(inputs_dir, outputs_dir, input_file_paths, output_file_paths, 10, "edges", 7, 7)
    generate_test_files(inputs_dir, outputs_dir, input_file_paths, output_file_paths, 10, "edges", 10, 10)
    generate_test_files(inputs_dir, outputs_dir, input_file_paths, output_file_paths, 10, "edges", 20, 20)
    generate_test_files(inputs_dir, outputs_dir, input_file_paths, output_file_paths, 10, "edges", 50, 50)
    generate_test_files(inputs_dir, outputs_dir, input_file_paths, output_file_paths, 10, "edges", 200, 200)
    generate_test_files(inputs_dir, outputs_dir, input_file_paths, output_file_paths, 10, "edges", 500, 500)


    with open(os.path.join(inputs_dir, "inputs_file_list.txt"), "w") as f:
        for path in input_file_paths:
            f.write(path + "\n")   

    with open(os.path.join(outputs_dir, "outputs_file_list.txt"), "w") as f:
        for path in output_file_paths:
            f.write(path + "\n")   


if __name__ == "__main__":
    main()
