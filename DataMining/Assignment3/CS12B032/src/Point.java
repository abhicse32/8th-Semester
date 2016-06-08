import java.lang.Double;
import java.util.*;
import java.lang.Math;

/**
 * Created by abhisheky on 16/2/16.
 */
public class Point implements Comparable<Point> {
    public ArrayList<Double> container;
    public Integer id;
    public static final Double UNDEFINED= 1.0;
    public boolean processed;
    public double reachability_dist;
    public double core_dist;

    public int compareTo(Point point){
        if(this.reachability_dist > point.reachability_dist)
            return 1;
        else if(this.reachability_dist== point.reachability_dist)
            return 0;
        else return -1;
    }

    public Point(){
        container= new ArrayList<Double>();
        reachability_dist= UNDEFINED;
        core_dist= UNDEFINED;
        processed=false;
        id= 0;
    }

    public void addCoord(double coord){
        container.add(coord);
    }

    public double getDistance(Point pt2){
        double sum=0;
        int dimension= container.size();
        for(int i=0;i< dimension; i++)
            sum+= Math.pow(this.container.get(i)- pt2.container.get(i), 2);
        return Math.sqrt(sum);
    }

    public double dot(Point point){
        double dotProduct=0;
        int dimension= point.container.size();
        for(int i=0; i< dimension; i++)
            dotProduct+= (point.container.get(i)*this.container.get(i));
        return dotProduct;
    }

//    public static void main(String [] args) {
////        HashMap<Integer, Double> map= new HashMap<Integer, Double>();
//        map.put(1,2.3);
//        map.put(3,0.23);
//        map.put(5, 0.84);
//        map.put(4, 0.0008);
//        Optics optics = new Optics();
//        List<Map.Entry<Integer, Double> >list= optics.getSortedList(map);
//        for (int i=0;i< list.size(); i++)
//            System.out.println(list.get(i).getValue());
//    }
}
