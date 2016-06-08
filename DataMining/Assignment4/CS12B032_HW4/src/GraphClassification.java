import java.io.*;
import java.util.HashMap;
import java.util.Map;

/**
 * Created by abhisheky on 1/4/16.
 */
class Creator{
    public int numFeatures;
    public int numGraphs;
    public HashMap<Integer, Integer> labelsMap;
    public HashMap<Integer, Integer> idMap;
    public HashMap<Integer, String> featuresMap;
    public Creator(){
        idMap= new HashMap<>();
        featuresMap= new HashMap<>();
        labelsMap= new HashMap<>();
        numFeatures=0;
        numGraphs=0;
    }

    public void getLabels(){
        String activeFileName= "ca_train.txt";
        String inactiveFileName= "ci_train.txt";
        File file= new File(activeFileName);
        try {
            BufferedReader reader= new BufferedReader(new FileReader(file));
            String str;
            while ((str= reader.readLine())!= null)
                labelsMap.put(Integer.parseInt(str),1);
            reader.close();
            reader= new BufferedReader(new FileReader(inactiveFileName));
            while ((str=reader.readLine())!=null)
                labelsMap.put(Integer.parseInt(str),-1);
            reader.close();
        }catch (IOException ex){
            ex.printStackTrace();
        }
    }

    public void getIdMap(String fileName){
        File file= new File(fileName);
        try{
            BufferedReader reader= new BufferedReader(new FileReader(file));
            String str;
            while ((str=reader.readLine())!= null){
                String [] strings= str.split(" ");
                numGraphs++;
                idMap.put(Integer.parseInt(strings[0]), Integer.parseInt(strings[1]));
            }
            reader.close();
        }catch (IOException ex){
            ex.printStackTrace();
        }
    }

    public void getFeatures(String featuresFileName){
        File featuresFile = new File(featuresFileName);
        try{
            BufferedReader reader = new BufferedReader(new FileReader(featuresFile));
            String str;
            int id;
            String [] comps;
            while ((str= reader.readLine())!=null){
                if(str.length()>0){
                    if(str.charAt(0)=='t'){
                        comps= str.split(" ");
                        id= Integer.parseInt(comps[2]);
                        while ((str= reader.readLine())!= null){
                            if(str.charAt(0)=='x') {
                                featuresMap.put(id, str);
                                numFeatures++;
                                break;
                            }
                        }
                    }
                }
            }
            reader.close();
        }catch (IOException ex){
            ex.printStackTrace();
        }
    }

    public void createFeatureMatrix(){
        int [][]featureMatrix= new int[numGraphs][numFeatures];

        String labelsFile= "labels.txt";
        try{
            BufferedWriter writer = new BufferedWriter(new FileWriter(new File(labelsFile)));
            int label;
            for(int i=0;i< numGraphs; i++) {
                label=0;
                if (labelsMap.containsKey(idMap.get(i)))
                    label= labelsMap.get(idMap.get(i));
                writer.write(label+"\n");
            }
            writer.close();
        }catch (IOException ex){
            ex.printStackTrace();
        }

        for(Map.Entry<Integer, String> entry: featuresMap.entrySet()){
            int featureId= entry.getKey();
            String[] graphs= entry.getValue().split(" ");
            int len= graphs.length;
            for(int i=1; i< len; i++)
                featureMatrix[Integer.parseInt(graphs[i])][featureId]=1;
        }

        /*Dump the feature Matrix info into a file*/
        try{
            BufferedWriter writer= new BufferedWriter(new FileWriter(new File("featuresDump.txt")));
            for(int i=0; i< numGraphs; i++) {
                writer.write(""+featureMatrix[i][0]);
                    for (int j = 1; j < numFeatures; j++)
                        writer.write("," + featureMatrix[i][j]);
                    writer.write("\n");
            }
            writer.close();
        }catch (IOException ex){
            ex.printStackTrace();
        }
    }

    public static void main(String []args){
        String idMappingFileName= "idMapping.txt";
        Creator creator= new Creator();
        creator.getIdMap(idMappingFileName);
        creator.getFeatures("gSpanInput.txt.fp");
        creator.getLabels();
        System.out.println(creator.labelsMap.size());
        System.out.println(creator.numGraphs+" "+creator.numFeatures);
        creator.createFeatureMatrix();
    }
}

public class GraphClassification {
    HashMap<Integer, Integer> idMap;
    public GraphClassification(){
        idMap= new HashMap<>();
    }

    public void mergeGraphs(String trainFileName, String testFileName){
        File trainFile= new File(trainFileName);
        File testFile= new File(testFileName);
        String generatedFileName= "generatedFile.txt";
        File generatedFile= new File(generatedFileName);
        int counter=0;
        try{
            BufferedReader reader= new BufferedReader(new FileReader(trainFile));
            BufferedWriter writer= new BufferedWriter(new FileWriter(generatedFile));
            String str;
            while ((str= reader.readLine())!= null) {
                writer.write(str + "\n");
                str= str.trim();
                if(str.charAt(0)=='#')
                    idMap.put(counter++,Integer.parseInt(str.substring(1)));
            }
            reader.close();;
            reader= new BufferedReader(new FileReader(testFile));
            while ((str= reader.readLine())!= null) {
                writer.write(str + "\n");
                str= str.trim();
                if(str.charAt(0)=='#')
                    idMap.put(counter++, Integer.parseInt(str.substring(1)));
            }
            reader.close();
            writer.close();
        }catch (IOException ex){
            ex.printStackTrace();
        }
    }

    public static void main(String [] args){
        String trainFile= args[0];
//        String activeFile= args[1];
//        String inActiveFile= args[2];
        String testFile= args[3];
        GraphClassification classification= new GraphClassification();
        classification.mergeGraphs(trainFile, testFile);

        try{
            BufferedWriter writer = new BufferedWriter(new FileWriter("idMapping.txt"));
            for(Map.Entry<Integer, Integer> entry: classification.idMap.entrySet())
                writer.write(entry.getKey()+" "+entry.getValue()+"\n");
            writer.close();
        }catch (IOException ex){
            ex.printStackTrace();
        }
    }
}
