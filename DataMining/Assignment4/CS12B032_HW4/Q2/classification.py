from numpy import genfromtxt
import numpy as np
import sys
import sklearn.linear_model as sm
from sklearn.metrics import f1_score
data = genfromtxt('featuresDump.csv', delimiter=',')
labels = genfromtxt('labels.csv', delimiter=',')
with open(sys.argv[1]) as f_t:
	t = f_t.readlines()
test_size = 0
for s in t:
	if s[0] == '#':
		test_size+=1
number_test_train = labels.shape[0]
train_size = number_test_train-test_size
trainData = data[:train_size]
testData = data[train_size:number_test_train]
usefulData = []
usefulLabel = []
for i in range(labels.shape[0]):
	if labels[i] != 0:
		usefulData.append(trainData[i].tolist())
		usefulLabel.append(labels[i])	
x = sm.LogisticRegression(class_weight='balanced')
x.fit(usefulData,usefulLabel)
y_pred = x.predict(testData).tolist()
fw = open('output.txt','w')
for i in y_pred:
	if i == 1:
		fw.write("1\n")
	if i == -1:
		fw.write("0\n")
fw.close()
print "Number of test Molecules were :",len(testData)
