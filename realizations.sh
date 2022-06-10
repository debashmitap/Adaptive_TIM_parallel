# Deletes and re-generates realizations for a dataset
# Usage: ./realization.sh [dataset] [realizations]
dataset=${1}
r=${2}

echo "Generating new realizations"

rm ${dataset}/realization_*
./realization ${dataset}/ ${r} > ${dataset}/n.txt