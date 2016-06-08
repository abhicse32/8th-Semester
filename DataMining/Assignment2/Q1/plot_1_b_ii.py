import matplotlib.pyplot as plt
# lpot for 1b_i
lst=[[10 ,21.605417],
[25 ,17.538208],
[75,19.691659],
[100 ,20.952556]]

lst2=[[10, 0.333225],
[25, 1.444862],
[75, 22.277402],
[100, 45.840727]
]

plt.figure()
plt.plot([x[0] for x in lst],[x[1] for x in lst],label="apriori")
plt.plot([x[0] for x in lst2], [x[1] for x in lst2],label="fp-growth")
plt.legend(loc=1)
plt.xlabel('minimum threshold(%)')
plt.ylabel('time')
plt.title('title')
plt.show()

