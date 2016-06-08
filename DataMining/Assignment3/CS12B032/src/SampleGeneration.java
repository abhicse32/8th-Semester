import java.io.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Random;

/**
 * Created by abhisheky on 22/2/16.
 */

public class SampleGeneration{
    public static void main(String [] args){
        Optics optics= new Optics();
        optics.getDataPoints();
        ArrayList<Point> dataPoints= optics.points;
        Integer[] array= {10000, 25000, 100000};
        Random random= new Random();
        int size=0, index;
        String fileName="sample";

        for (Integer sampleSize: array){
            HashMap<Integer, Point> map=new HashMap<Integer, Point>();
            size=0;
            while (size< sampleSize){
                index = random.nextInt(200000);
                if (!map.containsKey(index)) {
                    map.put(index, dataPoints.get(index));
                    size++;
                }
            }

            try{
                BufferedWriter writer= new BufferedWriter(new FileWriter(new File(fileName+sampleSize+".txt")));
                for (Map.Entry<Integer, Point> entry: map.entrySet())
                    writer.write(entry.getValue().container.get(0)+" "
                            + entry.getValue().container.get(1)+"\n");
                writer.close();
            }catch (IOException ex){
                ex.printStackTrace();
            }
        }
    }
}