__author__="abhishek"
import scipy.io as sio
import numpy as np
import sys
import operator
import matplotlib.pyplot as plt


def read_mat(file_,filename1):
    filename1=open(filename1,"w")
    mat=sio.loadmat(file_)
    dict_=mat[mat.keys()[0]]
    for elements in dict_:
        filename1.write(str(elements[0])+"\n")
    filename1.close()

def read_original(filename,filename__):
    file_=open(filename,"r")
    dict_={}
    for lines in file_:
        lines=int(lines.rstrip('\n'))
        if lines in dict_:
            dict_[lines]+=1
        else:
            dict_[lines]=1
    file_.close()
    """file_=open(filename__,"w")
    dict__=sorted(dict_.items(),key=lambda x:x[1],reverse=True)
    for vals in dict__:
        file_.write(str(vals)+'\n')
    file_.close()"""
    dict_=dict_.values()
    dict_.sort(reverse=True)
    return dict_[:5010]

def readFile(filename,filename__):
    lst,lst2=[],[]
    filename=open(filename,"r")
    for lines in filename:
        temp=[int(str_) for str_ in lines.split()]
        lst.append(temp[1])
        lst2.append(temp[:2])
    filename.close()
    file_=open(filename__,"w")
    for nodes in lst2:
        file_.write(str(tuple(nodes))+'\n')
    file_.close()
    lst.sort(reverse=True)
    return lst[:5010]

def plot_figure(lst1,lst2,label1,label2,label_,Dataset):
    plt.figure()
    plt.plot(lst1,label=label1)
    plt.plot(lst2,label=label2)
    plt.ylabel(label_)
    plt.title(Dataset)
    plt.legend(loc=1)
    plt.show()

filename1=sys.argv[1]
filename2=sys.argv[2]
filename3=sys.argv[3]
filename4=sys.argv[4]

#original_x_count=read_original(filename1,filename5)
#original_y_count=read_original(filename2,filename6)
space_saving_x_count=readFile(filename1,filename3)
space_saving_y_count=readFile(filename2,filename4)


"""accuracy_x=[]
accuracy_y=[]
top_k=[100,500,1000,5000]
for k in top_k:
    difference_x=[abs(original_x_count[i]-space_saving_x_count[i]) for i in range(k)]
    difference_y=[abs(original_y_count[i]-space_saving_y_count[i]) for i in range(k)]
    accuracy_x.append(sum(difference_x)/k*1.0)
    accuracy_y.append(sum(difference_y)/k*1.0)
"""

"""print accuracy_x
print accuracy_y
plt.figure()
plt.plot(top_k,accuracy_x,label="x-Dataset")
plt.plot(top_k,accuracy_y,label="y-Dataset")
plt.legend(loc=1)
plt.xlabel('k')
plt.ylabel('Average Error')
plt.title('Average error vs k')
plt.show()
"""
"""plot_figure(original_x_count,space_saving_x_count,"frequency-original","frequency-Space-Saving","Frequency","x-Dataset")
plot_figure(original_y_count,space_saving_y_count,"frequency-original","frequency-Space-Saving","Frequency","y-Dataset")"""
