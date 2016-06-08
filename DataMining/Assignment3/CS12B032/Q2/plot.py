__author__ = 'abhisheky'
import matplotlib.pyplot as plt
import numpy as np
import scipy.stats as stats

files= ["out10.txt","out100.txt", "out500.txt","out1000.txt","out10000.txt"]
for file in files:
    file_= open(file, "r")
    line= file_.readline().split()
    line= sorted([float(val) for val in line])
    #fit= stats.norm.pdf(line,np.mean(line), np.std(line))
    #plt.plot(line, fit, '-o')
    plt.hist(line, normed=True)
    plt.xlabel("Angles")
    plt.ylabel("frequency/normalized_Frequency")
    number= file.split('.')[0][3:]
    plt.title(str(number)+"-D datapoints");
    plt.hist(line)
    plt.show()





