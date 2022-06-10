import os
import sys

dataset = sys.argv[1]
number = int(sys.argv[2])

with open("spread.csv", "w") as spread_file:
    for i in range(0, number):
        temp_path = os.path.join(dataset+"_"+str(i), "spread.csv")
        with open(temp_path, "r") as temp_file:
            if i == 0:
                # Include only the first header
                spread_file.writelines(temp_file.readlines()[:3001])
            else:
                spread_file.writelines(temp_file.readlines()[1:3001])
