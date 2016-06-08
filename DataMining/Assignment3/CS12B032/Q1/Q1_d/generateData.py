__author__ = 'abhisheky'

with open("/home/abhisheky/Documents/8thSem/DM/Assignments/Assignment3_New/Q1/Q1_d/R_Tree_ep100.txt","r") as reader:
    with open("rax100.txt","w") as writer:
        for line in reader:
            str_= line.split("-")
            writer.write(str_[1])


