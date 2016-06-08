__author__ = 'abhisheky'
from matplotlib import pyplot as plt
import numpy as np

def getClusterCount(infile):
    list_=[]
    with open(infile, "r") as reader:
        for line in reader:
            nodes=len(line.split())
            if nodes>2:
                list_.append(nodes)
    return list_

def plotHistogram(list_):
    plt.title("histogram of cluster sizes")
    plt.xlabel("cluster-size")
    plt.ylabel("size-freuency/count")
    plt.hist(list_)
    plt.show()

def writeToFile(list_,fileName):
    with open(fileName,"w") as writeFile:
        for value in list_:
            writeFile.write(str(value)+"\n")

ground_fileName="com-amazon.top5000.cmty.txt";
list_= getClusterCount(ground_fileName)
plotHistogram(list_)
writeToFile(sorted(list_),"histFile.txt")
list_= getClusterCount("dump.data.amazon.mci.I123")
plotHistogram(list_)
writeToFile(sorted(list_),"histFile_amazon.txt")