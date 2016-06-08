__author__ = 'abhishek'
import numpy as np
import matplotlib.pyplot as plt
from math import e,log
lst=[]
def calculate_derivative():
    lst=list(np.arange(0.1,20,0.1))
    print len(lst)
    lst_=[]
    for i in lst:
        val= 1-e**(-i/8.0)
        expr=(val**i)*(log(val)+(i*(1-val))/(8.0*val))
        lst_.append(tuple([round(i,3),round(expr,13)]))
    print lst_
lst_x,lst_y=[],[]
for i in range(1,21):
    lst_x.append(i)
    lst_y.append((1-e**(-i/8.0))**i)

# plt.figure()
# plt.plot(lst_x,lst_y)
# plt.title("BloomFilter")
# plt.xlabel("k")
# plt.ylabel("False Positives")
# plt.show()
calculate_derivative();
