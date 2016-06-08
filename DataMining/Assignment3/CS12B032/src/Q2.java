import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.*;

/**
 * Created by abhisheky on 17/2/16.
 */
public class Q2 {
    ArrayList<Point> points;
    public int num_points;
    public HashMap<Integer, ArrayList<Double>> map;

    public Q2(){
        points= new ArrayList<Point>();
        map= new HashMap<Integer, ArrayList<Double>>();
        num_points=1000;
    }

    public static void main(String [] args){
        Q2 q2= new Q2();
        Integer []array={10,100,500,1000,10000};
        for(Integer dim: array) {
            q2.generatePoints(dim);
            Point origin= q2.generateOrigin(dim);
            q2.map.put(dim, q2.calculateAngles(origin));
            q2.points.clear();
        }
        for(Map.Entry<Integer, ArrayList<Double>> iterator: q2.map.entrySet()){
            String fileName="out"+iterator.getKey().toString()+".txt";
            System.out.println(iterator.getValue().size());
            try {
                BufferedWriter writer = new BufferedWriter(new FileWriter(new File(fileName)));
                Iterator<Double> iterator1 = iterator.getValue().iterator();
                while (iterator1.hasNext())
                    writer.write(iterator1.next().toString()+" ");
                writer.close();

            }catch (IOException ex){
                ex.printStackTrace();
            }
        }
    }

    public ArrayList<Double> calculateAngles(Point origin){
        double dotProduct;
        double modMultiplication;
        ArrayList<Double> angles= new ArrayList<Double>();

        for(int i=0; i< num_points; i++){
            Point currentPoint= points.get(i);
            for(int j=i+1; j< num_points; j++){
                dotProduct= currentPoint.dot(points.get(j));
                modMultiplication=currentPoint.getDistance(origin);
                modMultiplication*= points.get(j).getDistance(origin);
                angles.add((Math.acos(dotProduct/modMultiplication)*180)/Math.PI);
            }
        }
        return  angles;
    }

    public void generatePoints(int dimension){
        Random rand= new Random();
        for(int i=0;i< num_points; i++) {
            Point point = new Point();
            for (int j = 0; j < dimension; j++)
                point.addCoord((rand.nextFloat()-0.5)*2);
            points.add(point);
        }
    }

    public Point generateOrigin(int dim){
        Point point= new Point();
        for(int i=0; i<dim ; i++)
            point.addCoord(0);
        return point;
    }

    public void generateSample(){
        Integer [] array={10000, 25000, 100000};
        Optics optics= new Optics();
        optics.getDataPoints();
        ArrayList<Point> dataPoints= optics.points;
        for (Integer sampleSize: array){

        }
    }
}
