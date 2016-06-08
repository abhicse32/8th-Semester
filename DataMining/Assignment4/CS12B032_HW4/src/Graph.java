import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;

/**
 * Created by abhisheky on 17/3/16.
 */
class TrackInfo implements Cloneable{
    public int orderCounter;
    public int prevCounter;
    public int prevNode;
    public int[] visited;
    public int[] visitOrder;

    public TrackInfo(Integer nodes){
        orderCounter=0;
        prevCounter=0;
        prevNode=0;
        visited= new int[nodes+1];
        visitOrder= new int[nodes+1];
    }

    public TrackInfo clone(){
        TrackInfo info;
        try{
            info=(TrackInfo)super.clone();
            info.visited= this.visited.clone();
            info.visitOrder= this.visitOrder.clone();
            info.orderCounter= this.orderCounter;
            info.prevNode= this.prevNode;
            return info;
        }catch (CloneNotSupportedException ex){
            ex.printStackTrace();
            throw  new RuntimeException();
        }
    }
}

class Code implements Comparable<Code>{
    public Integer srcNode;
    public Integer destNode;
    public String edgeCode;

    public Code(Integer srcNode, Integer destNode, String edgeCode){
        this.srcNode= srcNode;
        this.edgeCode= edgeCode;
        this.destNode= destNode;
    }
    public Code(){

    }

    public int compareTo(Code c){
        int temp1= this.srcNode - c.srcNode;
        if(temp1 < 0) return -1;
        else if(temp1 > 0) return 1;
        temp1= this.destNode - c.destNode;
        if(temp1 < 0) return -1;
        else if(temp1 > 0) return 1;
        return this.edgeCode.compareTo(c.edgeCode);
    }
}

class  Pair_<T,P> implements Comparable<Pair_<T,P>>{
    public T first;
    public P second;

    public int compareTo(Pair_<T,P> pair){
        return ((String)(this.second)).compareTo((String)pair.second);
    }

    public Pair_(T first, P second){
        this.first= first;
        this.second= second;
    }
}

public class Graph {
    public int nNodes;
    public int nEdges;
    public ArrayList<ArrayList<Pair_<Integer, String>>> adList;
    public HashMap<Integer, String> nodesMap;
    public Graph(int nNodes) {
        this.nNodes= nNodes;
        this.nEdges=0;
        nodesMap = new HashMap<>();
        adList= new ArrayList<>();
        for(int i=0; i< nNodes; i++)
            adList.add(new ArrayList<>());
    }

    public void addNode(Integer nodeNum, String nodeLabel){
        nodesMap.put(nodeNum, nodeLabel);
    }

    public void append(ArrayList<Integer> destList, ArrayList<Integer> srcList){
        Iterator<Integer> iterator= srcList.iterator();
        while (iterator.hasNext())
            destList.add(iterator.next());
    }

    public static void main(String [] args){
        File file= new File("res.txt");
        try {
            BufferedReader reader= new BufferedReader(new FileReader(file));
            String str;
            while ((str= reader.readLine())!= null) {

            }
            reader.close();
        }catch (IOException ex){
            ex.printStackTrace();
        }
    }
}