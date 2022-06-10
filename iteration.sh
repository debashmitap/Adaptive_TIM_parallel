temp=${1}
remaining_nodes=${2}
realization=${3}

echo "Running TIM"
./tim -model IC -dataset ${temp} -epsilon 0.05 -k 1 -n ${remaining_nodes}
echo "Running BFS"
influence=`./bfs ${temp} ${realization}` 