min_sup=0.01
from random import shuffle
import time
###########################################################################
#function to calculate the 1-frequent set

start_time= time.time()
def find_frequent_1_itemset():
    lst=[]
    with open("retail.dat","r") as data:
        for lines in data:
            lst_=[int(val) for val in lines.strip("\n ").split()]
            lst_.sort()
            lst.append(lst_)
    return lst

def quick(lst):
	frequent_set,trans_id,i={},{},0
	for lst_ in lst:
		for num in lst_:
			if num in frequent_set:
				frequent_set[num]+=1
				trans_id[num].append(i)
			else:
				frequent_set[num]=1
				trans_id[num]=[i]
			i+=1
	mod_frequent_set,mod_trans_id={},{}
	min_sup_count=int((len(lst)*min_sup))

	for items in frequent_set:
		if frequent_set[items] >= min_sup_count:
			mod_frequent_set[items]=frequent_set[items]
			mod_trans_id[items]=trans_id[items]
	del frequent_set
	del trans_id
	return mod_frequent_set,mod_trans_id,min_sup_count
###############################################################################

#checks if Ck_1 contains all the subsets of size k_1 of Ck
def has_infrequent_subset(Ck_1,Ck):
    len_=len(Ck)
    subsets=Ck[:len_-1]
    #print subsets,Ck_1
    if subsets not in Ck_1:
        return True
    for i in range(1,len_):
        subsets=Ck[:i-1]+Ck[i:]
        if subsets not in Ck_1:
            return True
    return False
################################################################################

def find_difference(lst1,lst2):
    len_,count=len(lst1),0
    for i in range(len_):
        if lst1[i] != lst2[i]:
            count+=1
        if count>1:
            break
    return count
##################################################################################
def apriori_gen(C,min_sup_count,mod_trans_id):
    len_=len(C)
    C_k,C_,trans_id=[],[],[]
    set_list=[set(items) for items in mod_trans_id]
    for i in range(len_):
        for j in range(i+1,len_):
            if find_difference(C[i],C[j])==1:
                if C[i][-1:] < C[j][-1:]:
                    C_=[vals for vals in C[i]]
                    C_.append(C[j][-1])
                else:
                    C_=[vals for vals in C[j]]
                    C_.append(C[i][-1])
                
                temp=set_list[i].intersection(set_list[j])
                
                #print C_
                if has_infrequent_subset(C,C_)==False:
                    C_k.append(C_)
                    trans_id.append(list(temp))

    return C_k,trans_id
"""
def get_top_X(X,C):
	lst_new=[]
	with open("retail.dat","r") as data:
		for lines in data:
			lst_=[int(val) for val in lines.strip("\n ").split()]
			lst__=[]
			for val in lst_:
				if C[val] >= X:
					lst__.append(val)
			lst__.sort()
			lst_new.append(lst__)
	return lst_new
"""
###############################################################################
"""main function
             C -> set of frequent itemsets of size-1
  mod_trans_id -> contains the transaction ID's of all the 1-itemsets
  lst          -> transation details
  min_sup_count-> minimum threshold support count
"""
lst=find_frequent_1_itemset()
shuffle(lst)
len_lst=len(lst)
#percent=1
C,mod_trans_id,min_sup_count=quick(lst[:int(percent*len_lst)])
i=1
#print len(C),len(mod_trans_id),len(lst),min_sup_count
frequent_items,mod_trans={},{}
frequent_items["C"+str(i)]=[[item] for item in C]
mod_trans["C"+str(i)]=[mod_trans_id[items] for items in mod_trans_id]
support_values,temp=[],[]
for values in C:
    temp.append(C[values])
support_values.append(temp)

while True:
    C,trans_id=apriori_gen(frequent_items["C"+str(i)],min_sup_count,mod_trans["C"+str(i)])
    if C==[]:
        break
    i+=1
    temp_trans,temp_items=[],[]
    len_=len(trans_id)
    for j in range(len_):
        if len(trans_id[j]) >= min_sup_count:
            temp_trans.append(trans_id[j])
            temp_items.append(C[j])
    frequent_items["C"+str(i)]=temp_items
    mod_trans["C"+str(i)]=temp_trans
################################################################################################

sorted_frequent_items=sorted(frequent_items.items(), key=lambda s:s[0])

for keys in sorted_frequent_items:
    for items in keys[1]:
        items.sort()
    keys[1].sort()

min_sup=percent*100
print "%d  %s"%(min_sup,time.time()-start_time)
#print sorted_frequent_items

with open("CS12B032_1a.txt","a") as outfile:
    for values in sorted_frequent_items:
        for vals in values[1]:
            for val in vals: 
                outfile.write(str(val)+' ')
            outfile.write('\n')
    outfile.write('minimum support threshold: %f\n'%(min_sup))

###############################################################################################

