import os,subprocess
size=42682
lst=[.05,0.1,0.25,0.5,0.95]
for k in lst:
    os.system("./gaston "+str(k*size) +" out2.txt result.txt")