__author__ = 'abhisheky'
fileName= open("/home/abhisheky/Documents/8thSem/DM/Assignments/Assignment3_New/Q1/output.dat","r");
xCoords, yCoords= [],[]
for line in fileName:
    coords= line.rstrip("\n").split()
    xCoords.append(float(coords[0]))
    yCoords.append(float(coords[1]))

import matplotlib.pyplot as plt
import numpy as np
colors= np.random.rand(20000)
plt.scatter(xCoords, yCoords, c=colors)
plt.xlabel("X-axis")
plt.ylabel("Y-axis")
plt.title("scatter plot of original dataset")
plt.show()

