__author__ = 'abhisheky'
"""Create files containing only reachability values"""
def extractReachable(fileName):
    with open(fileName,"r") as reader:
        with open("reachability_Only.txt","w") as writer:
            for line in reader:
                str_= line.split('-')
                writer.write(str_[1])

import matplotlib.pyplot as plt
import numpy as np

def plotSample(fileName):
    xCoord=[]
    yCoord=[]
    with open(fileName,"r") as reader:
        for line in reader:
            temp= line.strip("\n").split()
            xCoord.append(float(temp[0]))
            yCoord.append(float(temp[1]))

    colors= np.random.rand(20);
    plt.xlabel("X-axis")
    plt.ylabel("Y-axis")
    plt.title("scatter plot of random sample of 10000 data-points")
    plt.scatter(xCoord,yCoord, c=colors)
    plt.show()

def plotGraph():
    x=[10000,25000, 100000, 200000, ]
    #y_kd_tree=[3160, 20570, 526094, 2033093]
    #y_normal= [8271, 56008, 1000499, 3921970]
    y_kd_tree=[.115, .615, 11.083, 54.820]
    y_normal=[3.120, 16.479, 228.210, 1001.355]
    plt.xlabel("sample-size");
    plt.ylabel("time(ms)")
    plt.title("Plot comparing the running times of two implementations")
    plt.plot(x,y_kd_tree)
    plt.plot(x,y_normal)
    plt.legend(["kd_tree", "Naive", "e=0.1"], loc="upper left")
    plt.show()

plotSample("/home/abhisheky/Documents/8thSem/DM/Assignments/Assignment3_New/Q1/sample10000.txt");
#plotGraph()
#extractReachable("normal_out10000.txt")