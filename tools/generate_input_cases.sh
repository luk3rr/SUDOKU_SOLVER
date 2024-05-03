#!/usr/bin/env sh

# Filename: generate_input_cases.sh
# Created on: May  2, 2024
# Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
#
# Description:
#   Generate and grade test cases for the Sudoku Solver
#   This script executes the 'generator.py' script to generate test cases
#
# Usage: ./generate_and_grade.sh <start> <end>

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <start> <end>"
    exit 1
fi

start=$1
end=$2

ROOT_DIR=".."
INPUT_DIR="${ROOT_DIR}/test/inputs"
GENERATOR="${ROOT_DIR}/tools/generator.py"

# Save the original directory
original_dir=$(pwd)

# Change to the directory of this script
cd "$(dirname "$0")"

# Grade based on the number of expanded nodes (hard, medium, easy)
super_hard=500000 # >= 500000 is super hard
hard=100000 # >= 100000 and < 500000 is hard
medium=25000 # >= 25000 and < 100000 is medium
easy=1000 # >= 1000 and < 25000 is easy
super_easy=1000 # < 1000 is super easy

# Cleanup the input directory
function cleanup () {
    rm -f "${INPUT_DIR}/case*.in"
    rm -f "${INPUT_DIR}/case*.out"
}

# Check if the case already exists
function case_exists () {
    for dir in "$@"; do
        if [ -f "${INPUT_DIR}/${dir}/case${padded}.in" ]; then
            return 0
        fi
    done

    return 1
}

# Generate test cases
function generate () {
    for ((i = start; i <= end; i++)); do
        printf -v padded "%03d" $i  # Format the number with three digits
        echo "Generating case ${padded}..."

        if case_exists "." "super_easy" "easy" "medium" "hard" "super_hard"; then
            echo "Case ${padded} already exists. Skipping..."
            continue
        fi

        python3 ${GENERATOR} > "${INPUT_DIR}/case${padded}.in"
    done
}

# Grade the test cases
function grade () {
    for ((i = start; i <= end; i++)); do

        printf -v padded "%03d" $i  # Format the number with two digits

        echo "Running case ${padded}..."

        if ! case_exists "."; then
            echo "Case ${padded} does not exist or has been moved. Skipping..."
            continue
        fi

        ${ROOT_DIR}/bin/Release/sudoku_solver B $(cat "${INPUT_DIR}/case${padded}.in") > "${INPUT_DIR}/case${padded}.out"

        result=$(cat "${INPUT_DIR}/case${padded}.out" | grep 'Total expanded' | awk '{print $4}')

        # Grade based on the number of expanded nodes by BFS until the solution is found
        folder=""

        if [ $result -ge $super_hard ]; then
            folder="super_hard"
        elif [ $result -ge $hard ]; then
            folder="hard"
        elif [ $result -ge $medium ]; then
            folder="medium"
        elif [ $result -ge $easy ]; then
            folder="easy"
        else
            folder="super_easy"
        fi

        mkdir -p "${INPUT_DIR}/${folder}"
        mv "${INPUT_DIR}/case${padded}.in" "${INPUT_DIR}/${folder}/case${padded}.in"
        mv "${INPUT_DIR}/case${padded}.out" "${INPUT_DIR}/${folder}/case${padded}.out"
    done
}

# Generate and grade the test cases
generate
grade

# Change back to the original directory
cd "$original_dir"
