import os,subprocess
lst=['.05','.10','.25','.5','.95']
for k in lst:
    os.system("./gSpan -f out2.txt -s " +k+" -o -i")

# __author__ = 'abhishek'

# import numpy as np
# import sys
# import random
# beta = 0.15
# #implementation for bipartite graph
# def read_graph(filename):
#     edges,nodes0,nodes1=[],[],[]
#     file_=open(filename,"r")
#     for lines in file_:
#         temp=[int(val) for val in lines.split()[:2]]
#         edges.append(temp)
#         if temp[0] not in nodes0:
#             nodes0.append(temp[0])
#         if temp[1] not in nodes1:
#             nodes1.append(temp[1])
#     nodes0.sort()
#     nodes1.sort()
#     last=nodes0[-1]
#     for nodes in nodes1:
#         nodes0.append(nodes+last)
#     for edge in edges:
#         edge[1]=edge[1]+last

#     del nodes1
#     neighbors_count={}
#     for edge in edges:
#         if edge[1] not in neighbors_count:
#             neighbors_count[edge[1]]=1
#         else:
#             neighbors_count[edge[1]]+=1
#         if edge[0] not in neighbors_count:
#             neighbors_count[edge[0]]=1
#         else:
#             neighbors_count[edge[0]]+=1
#     file_.close()
#     return nodes0,edges,neighbors_count

# def create_adjacency_matrix(edges,len_,neighbors_count):
#     sim_mat=[[0 for val in range(len_)] for val_ in range(len_)]
#     for edge in edges:
#         sim_mat[edge[0]][edge[1]]=1.0/neighbors_count[edge[1]]
#         sim_mat[edge[1]][edge[0]]=1.0/neighbors_count[edge[0]]
#     ad_mat=np.array(sim_mat)
#     return ad_mat

# def RWR(sim_mat,rand_node):
#     len_=len(sim_mat[0])
#     start_vector=np.array([[0] for val in range(len_)])
#     rank_mat=np.array([[0] for val in range(len_)])
#     start_vector[rand_node,0]=1.0
#     rank_mat[rand_node,0]=1.0
#     sim_mat=sim_mat*beta
#     start_vector=start_vector*(1-beta)
#     for i in range(20):
#         temp=np.dot(sim_mat,rank_mat)
#         rank_mat=np.add(rank_mat,temp)
#     return list(rank_mat)


# filename=sys.argv[1]
# nodes,edges,neighbors_count=read_graph(filename)
# sim_mat=create_adjacency_matrix(edges,len(nodes)+1,neighbors_count)
# top_100=sorted(neighbors_count,key=neighbors_count.get,reverse=True)
# top_100=[val for val in top_100 if val>943][:100]
# del nodes
# del edges
# del neighbors_count
# for nodes in top_100:
#     rank_matrix=RWR(sim_mat,nodes)
#     rank_matrix.sort(reverse=True)
#     print nodes
#     print rank_matrix[:100]