import java.io.*;
import java.util.ArrayList;
import java.util.Iterator;

/**
 * Created by abhisheky on 25/2/16.
 */
class PointStore{
    String point;
    Double reach;
    Double core;
    public PointStore(String point, Double reach, Double core){
        this.core=core;
        this.reach= reach;
        this.point=point;
    }

    public PointStore(){
        point=null;
        reach=1000.0;
        core= 1000.0;
    }
}

public class ClusterExtraction {
    public Double epsilon;
    public Integer minPts;

    public ClusterExtraction(Integer minPts, Double epsilon){
        this.epsilon= epsilon;
        this.minPts= minPts;

    }
    public ArrayList<PointStore> readFile(){
        File file= new File("/home/abhisheky/Documents/8thSem/DM/Assignments/Assignment3_New/Q1/Q1_b/reach_core.txt");
        ArrayList<PointStore> store= new ArrayList<PointStore>();
        try {
            BufferedReader reader= new BufferedReader(new FileReader(file));
            String line, temp;
            while ((line=reader.readLine())!=null){
                String [] strArray= line.split("-");
                temp= strArray[0];
                strArray=strArray[1].split("  ");
               // System.out.println(strArray[0]+"  "+strArray[1]);
               store.add(new PointStore(temp,Double.parseDouble(strArray[0]), Double.parseDouble(strArray[1])));
            }
        }catch (IOException ex){
            ex.printStackTrace();
        }
        return store;
    }

    public ArrayList<ArrayList<String>> exctract(ArrayList<PointStore> store){
        Iterator<PointStore> sotreIterator= store.iterator();
        ArrayList<ArrayList<String>> totalClusters= new ArrayList<ArrayList<String>>();
        totalClusters.add(new ArrayList<String>());
        int i=0;
        Double reachability;
        while (sotreIterator.hasNext()){
            PointStore tempStore= sotreIterator.next();
            if (Double.compare(tempStore.reach , epsilon)>0){
                if (tempStore.core <= epsilon){
                    if(totalClusters.get(i).size()>0){
                        totalClusters.add(new ArrayList<String>());
                        i++;
                    }
                    totalClusters.get(i).add(tempStore.point);
                }
            }else{
                totalClusters.get(i).add(tempStore.point);
            }
        }
        return totalClusters;
    }

    public void printExtractedClusters(ArrayList<ArrayList<String>> clusters){
        Iterator<ArrayList<String>> iterator1= clusters.iterator();
        System.out.println(clusters.size());
        File file= new File("out_b_0123.txt");
        try {
            BufferedWriter writer= new BufferedWriter(new BufferedWriter(new FileWriter(file)));
            while (iterator1.hasNext())
                writer.write(Integer.toString(iterator1.next().size())+" ");
            writer.close();
        }catch (IOException ex){
            ex.printStackTrace();
        }
        System.out.println();
    }

    public static void main(String []args){
        ClusterExtraction extraction= new ClusterExtraction(10,.0124);
        ArrayList<PointStore> store= extraction.readFile();
        ArrayList<ArrayList<String>> clusters= extraction.exctract(store);
        extraction.printExtractedClusters(clusters);
    }
}
