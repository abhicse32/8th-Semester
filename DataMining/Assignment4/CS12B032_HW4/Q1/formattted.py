__author__ = 'abhishek'
import re
import sys
filename1=sys.argv[1]
filename2=sys.argv[2]
filename3=sys.argv[3]
filename4=sys.argv[4]
file_=open(filename3,"r")

#python formatted.py aid_whatever.txt out1.txt elements.txt out2.txt 
lst,counter,lst_={},1,[]
for lines in file_:
    str_=lines.split()
    lst[str_[3]]=counter
    lst_.append(str_[3])
    counter+=1
file_.close()

file_=open(filename1,"r")
file__=open(filename2,"w")
counter,graph_counter=0,0
for lines in file_:
    if re.match(r" *[A-Z]+[a-z]*",lines):
        file__.write("v "+str(counter)+" "+lines)
        counter+=1
    elif re.match(r"[0-9]+ +[0-9]+ +[0-9]+",lines):
        file__.write("e "+lines)
    elif re.match(r"[ ]*#",lines)!=None:
        graph_counter+=1
        match=re.search(r"[0-9]+",lines)
        file__.write("t # "+str(match.group())+"\n")
        counter=0

file_.close()
file__.close()

print graph_counter
file_=open(filename2,"r")
file__=open(filename4,"w")

for lines in file_:
    if re.match(r"v +[0-9]+[ ]+[A-Z]+[a-z]*",lines):
        str_=lines.split()
        str_[2]=lst[str_[2]]
        str_=str_[0]+" "+str_[1]+" "+str(str_[2])+"\n"
        file__.write(str_)
    else:
        file__.write(lines)