import os,subprocess
size=42682
lst=[5,10,25,50,95]
for k in lst:
    os.system("./fsg -s"+str(k)+" out2.txt")