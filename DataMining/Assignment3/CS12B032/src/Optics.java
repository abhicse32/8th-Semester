import java.io.*;
import java.util.*;

/**
 * Created by abhisheky on 16/2/16.
 */
public class Optics {
    public ArrayList<Point> points;
    public ArrayList<Point> orderedFile;
    public PriorityQueue<Point> seedList;
    public double epsilon;
    public int minPts;

    public Optics(){
        points= new ArrayList<Point>();
        orderedFile= new ArrayList<Point>();
        seedList= new PriorityQueue<Point>();
        epsilon=10;
        minPts=10;
    }

    public static void main(String []args){
        Optics optics= new Optics();
        optics.getDataPoints();
        final long start_time= System.currentTimeMillis();
        optics.runAlgo();
        optics.printOrderedSet();
        final long end_time= System.currentTimeMillis();
        System.out.println(end_time- start_time);

    }

    public void getDataPoints(){
        File file= new File("/home/abhisheky/Documents/8thSem/DM/Assignments/Assignment3_New/Q1/output.dat");
        String str;
        String [] strArray;
        Integer identifier=0;
        try {
            BufferedReader reader= new BufferedReader(new FileReader(file));
            while ((str=reader.readLine())!=null){
                strArray= str.split(" ");
                Point point= new Point();
                for(String temp_str: strArray)
                    point.addCoord(Double.parseDouble(temp_str));
                point.id=identifier;
                identifier++;
                points.add(point);
            }
        }catch(IOException exception){
            exception.printStackTrace();
        }
    }

    public void runAlgo(){
        Iterator<Point> iterator= points.iterator();
        while (iterator.hasNext()){
            Point object= iterator.next();
            if(!object.processed){
                expandClusterOrder(object);
            }
        }
    }

    public void expandClusterOrder(Point object){
        seedList.clear();
        seedList.add(object);
        while (!seedList.isEmpty()) {
            Point nextObject = seedList.peek();
            seedList.poll();
            if(!nextObject.processed) {
                HashMap<Integer, Double> neighbors = getEpsilonNeighbors(nextObject);
                nextObject.processed = true;
                updateCoreDistance(nextObject, neighbors);
                orderedFile.add(nextObject);
            }
        }
    }

    public HashMap<Integer, Double> getEpsilonNeighbors(Point object){
        HashMap<Integer, Double> neighors= new HashMap<Integer, Double>();
        Iterator<Point> iterator= points.iterator();
        double distance;
        while (iterator.hasNext()){
            Point object2= iterator.next();
            distance= object.getDistance(object2);
            if (distance <= epsilon)
                neighors.put(object2.id, distance);
        }
        return neighors;
    }

    public void updateCoreDistance(Point object, HashMap<Integer, Double> neighbors){
        neighbors.remove(object.id);
        int localMin= minPts-2;
        int len= neighbors.size();
        if(len > localMin) {
            List<Map.Entry<Integer, Double>> list = getSortedList(neighbors);
            Double core_dist = list.get(localMin).getValue();
            object.core_dist = core_dist;
            Point point;
            for (int i = localMin; i >= 0; i--) {
                point = points.get(list.get(i).getKey());
                if (!point.processed && point.reachability_dist > core_dist) {
                    if (seedList.contains(point))
                        seedList.remove(point);
                    point.reachability_dist = core_dist;
                    seedList.add(point);
                }
            }

            for (int i = localMin+1; i < len; i++) {
                point = points.get(list.get(i).getKey());
                if (!point.processed && point.reachability_dist > list.get(i).getValue()) {
                    if (seedList.contains(point))
                        seedList.remove(point);
                    point.reachability_dist = list.get(i).getValue();
                    seedList.add(point);
                }
            }
        }
    }

    public List<Map.Entry<Integer, Double>> getSortedList(HashMap<Integer, Double> passedMap){
        Set<Map.Entry<Integer, Double>> set= passedMap.entrySet();
        List<Map.Entry<Integer, Double>> list= new ArrayList<Map.Entry<Integer, Double>>(set);
        Collections.sort(list, new Comparator<Map.Entry<Integer, Double>>() {
            @Override
            public int compare(Map.Entry<Integer, Double> o1, Map.Entry<Integer, Double> o2) {
                return Double.compare(o1.getValue(), o2.getValue());
            }
        });
        return list;
    }

    public void printOrderedSet(){
        Iterator<Point> iterator= orderedFile.iterator();
        try {
            BufferedWriter writer= new BufferedWriter(new FileWriter(new File("/home/abhisheky/Documents/8thSem/DM/Assignments/Assignment3_New/Q1/Q1_b/reach_core.txt")));
            while (iterator.hasNext()) {
                Point point= iterator.next();
                writer.write("["+point.container.get(0).toString()+", "+point.container.get(1).toString()+"] - ");
                writer.write(Double.toString(point.reachability_dist) + "  "+Double.toString(point.core_dist)+"\n");
            }
            writer.close();
        }catch (IOException ex){
            ex.printStackTrace();
        }
    }
}
