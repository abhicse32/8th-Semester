__author__ = 'abhisheky'
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans
from scipy.spatial import distance

k=9
def getVariance(centroids, map_):
    variance=0
    for key, value in map_.iteritems():
        centroid= centroids[key]
        intra_dist=0
        for element in value:
            intra_dist+= (distance.euclidean(element, centroid))**2
        variance+=(2*len(value)*intra_dist)
    return variance

def inter_cluster_distance(centroids):
    min_distance=999999999999999
    for i in range(len(centroids)):
        for j in range(i+1,len(centroids)):
            dist= distance.euclidean(centroids[i], centroids[j])
            if dist < min_distance:
                min_distance= dist

    return min_distance


def draw_plot(variances):
    k_values=[i for i in range(1,k)]
    plt.plot(k_values, variances)
    plt.xlabel("K")
    plt.ylabel("Intra/Inter")
    plt.show()


with open("Q2.txt","r") as read_file:
    data_array=[]
    for lines in read_file:
        data=[float(entry) for entry in  lines.rstrip('\n').split()]
        data_array.append(data)

    variances= []
    for i in range(1,k):
        map_={}
        kmeans= KMeans(n_clusters=i)
        kmeans.fit(data_array)
        labels= kmeans.labels_
        centroids= kmeans.cluster_centers_

        for i in range(len(labels)):
            if labels[i] not in map_:
                map_[labels[i]]=[]
            map_[labels[i]].append(data_array[i])

        variances.append(getVariance(centroids, map_)/len(data_array))

    temp= variances[0]
    for i in range(len(variances)):
        variances[i]/= temp

    draw_plot(variances)









