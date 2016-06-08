__author__ = 'abhishek'
import matplotlib.pyplot as plt
import sys
plt.figure()
lst_a=[5,10,25,50,95]
lst_gSpan=[184.4,95.1,38.5,13.2,0.7]
lst_gaston=[38.7575,16.9704,5.82853,2.46855,0.397861]
lst_fsg=[242.966,117.217,43.6418,20.2219,6.00204]
plt.plot(lst_a,lst_fsg,label="fsg")
plt.plot(lst_a,lst_gSpan,label="gSpan")
plt.plot(lst_a,lst_gaston,label="gaston")
plt.legend(loc=1)
plt.title("performance comparison of subgraph mining algorithms")
plt.xlabel("Minsup(%)")
plt.ylabel("time(s)")
plt.show()
