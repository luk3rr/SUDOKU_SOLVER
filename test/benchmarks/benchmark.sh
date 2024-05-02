#!/usr/bin/env sh

# Filename: benchmark.sh
# Created on: May  2, 2024
# Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
#
# This script is used to benchmark the algorithms implemented in the sudoku
# solver. It will run the algorithms for the test cases in the range [start, end]
# and save the results in the data directory
#
# Usage: ./benchmark.sh <start> <end>
#
# NOTE: Execute tools/generate_test_cases.sh before running this script, if the
#       test/inputs directory is empty


if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <start> <end>"
    exit 1
fi

start=$1
end=$2

ROOT_DIR="../.."
INPUT_DIR="${ROOT_DIR}/test/inputs"
ALGORITHMS=("B" "I" "A" "U" "G")
PROGRAM="${ROOT_DIR}/bin/Release/sudoku_solver"
DATA_DIR="data"

# Save the original directory
original_dir=$(pwd)

# Change to the directory of this script
cd "$(dirname "$0")"

# Check if the case already exists
function case_exists () {
    for dir in "$@"; do
        if [ -f "${INPUT_DIR}/${dir}/case${padded}.in" ]; then
            dir_case_name="$dir"
            return 0
        fi
    done

    return 1
}

# Execute the sudoku solver for all algorithms and save the results
function execute () {
    for algorithm in "${ALGORITHMS[@]}"; do
        # Run the program with the algorithm and the input file
        result=$($PROGRAM $algorithm $(cat "${INPUT_DIR}/${1}/case${2}.in"))

        # Filter the relevant lines and store them in variables
        algorithm_used=$(echo "$result" | grep -E "^Algorithm" | awk '{print $2}')
        total_time=$(echo "$result" | grep -E "^Total time" | awk '{print $3}')
        expanded_states=$(echo "$result" | grep -E "^Total expanded" | awk '{print $4}')

        # Save the results in the data file
        filename="${DATA_DIR}/${1}/${algorithm_used}_${1}.dat"

        # The dat file has the following format:
        # CASE TIME EXPANDED_STATES
        echo "case${padded} $total_time $expanded_states" >> "$filename"
    done
}

# Run the benchmark for the range [start, end]
function benchmark () {

    # Check if the data directory exists
    if [ -d "$DATA_DIR" ]; then
        echo "The data directory already exists. To continue, the data will be overwritten. Do you want to continue? [y/n]"
        read -r answer

        if [ "$answer" != "y" ] && [ "$answer" != "Y" ]; then
            exit 0
        fi

        rm -rf "$DATA_DIR"
    fi

    for ((i = start; i <= end; i++)); do
        printf -v padded "%03d" $i  # Format the number with three digits

        # Find the case directory
        if case_exists "super_easy" "easy" "medium" "hard" "super_hard"; then
            tput el
            echo -ne "Running case${padded} in $dir_case_name\r"
            mkdir -p "$DATA_DIR/${dir_case_name}"
            execute $dir_case_name $padded
        fi
    done

    echo "Benchmark finished!"
}

benchmark

# Change back to the original directory
cd "$original_dir"
