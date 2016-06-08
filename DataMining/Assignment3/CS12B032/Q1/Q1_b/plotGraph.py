__author__ = 'abhisheky'
import matplotlib.pyplot as plt
epsilon=[ 0.0102, 0.0105, 0.0115, 0.012, 0.122, 0.0124 ]
clusters=[176, 106, 9, 8, 6, 6]
plt.xlabel("epsilon")
plt.ylabel("numbe of clusters extracted")
plt.title("epsilon' vs number of clusters extracted")
plt.plot(epsilon, clusters)
plt.show()