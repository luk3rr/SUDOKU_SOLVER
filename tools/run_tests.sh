#!/usr/bin/env sh

if [ "$#" -ne 2 ]; then
	echo "Usage: $0 <start> <end>"
	exit 1
fi

start=$1
end=$2

ROOT_DIR=".."
INPUT_DIR="${ROOT_DIR}/test/inputs"
GENERATOR="${ROOT_DIR}/tools/generator.py"

# Salvar o diretório atual
original_dir=$(pwd)

# Navegar para o diretório do script
cd "$(dirname "$0")"

# Grade based on the number of expanded nodes (hard, medium, easy)
hard=100000
medium=50000
easy=1000

function cleanup {
    rm -f "${INPUT_DIR}/case*.in"
    rm -f "${INPUT_DIR}/case*.out"
}

function generate {
    for ((i = start; i <= end; i++)); do
        echo "Generating case ${i}..."
        python3 ${GENERATOR} > "${INPUT_DIR}/case${i}.in"
    done
}

function grade {
    for ((i = start; i <= end; i++)); do
        echo "Running case ${i}..."

        ${ROOT_DIR}/bin/Release/sudoku_solver B $(cat "${INPUT_DIR}/case${i}.in") > "${INPUT_DIR}/case${i}.out"

        result=$(cat "${INPUT_DIR}/case${i}.out" | grep 'Total expanded' | awk '{print $4}')

        # Grade based on the number of expanded nodes
        # rename the case based on the grade
        folder=""
        if [ $result -ge $hard ]; then
            folder="hard"
        elif [ $result -ge $easy ] && [ $result -lt $medium ]; then
            folder="medium"
        else
            folder="easy"
        fi
        mkdir -p "${INPUT_DIR}/${folder}"
        mv "${INPUT_DIR}/case${i}.in" "${INPUT_DIR}/${folder}/case${i}.in"
        mv "${INPUT_DIR}/case${i}.out" "${INPUT_DIR}/${folder}/case${i}.out"
    done
}

generate
grade

cd "$original_dir"
