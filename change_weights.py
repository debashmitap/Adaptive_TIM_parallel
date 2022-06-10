import os
import sys
import random

random.seed = 2

file_path = sys.argv[1]
possible_values = [0.5]

output = []

with open(file_path, "r") as file:
    for line in file.readlines():
        values = line.split(" ")
        new_value = random.choice(possible_values)
        values[2] = str(new_value)

        output.append(values)


with open("weights_output.txt", "w") as file:
    for line in output:
        file.write(" ".join(line)+"\n")