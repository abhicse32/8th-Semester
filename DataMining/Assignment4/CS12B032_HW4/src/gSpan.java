import java.io.BufferedReader;
        import java.io.File;
        import java.io.FileReader;
        import java.io.IOException;
        import java.util.*;

/**
 * Created by abhisheky on 17/3/16.
 */

public class gSpan {
    public HashMap<String, Graph> database;
    ArrayList<ArrayList<Pair_<Integer, String>>> currAdlist;
    public gSpan(){
        database= new HashMap<>();
        currAdlist= null;
    }

    // function to print graph nodes, helps in debugging, no relevance with the algo
    public void printGraph(){
        for(Map.Entry<String, Graph> entry: database.entrySet()){
            Graph graph= entry.getValue();
            ArrayList<ArrayList<Pair_<Integer, String>>> adList=  graph.adList;
            int nodes= graph.nNodes;
            System.out.println("id: "+ entry.getKey());
            for(int i=0; i<nodes; i++){
                ArrayList<Pair_<Integer, String>> neighbors= adList.get(i);
                Iterator<Pair_<Integer, String>> iterator= neighbors.iterator();
                System.out.print(i+": ");
                while (iterator.hasNext()){
                    Pair_<Integer, String> pair= iterator.next();
                    System.out.print("<"+pair.first + ",("+ pair.second+")> ");
                }
                System.out.println();
            }
            break;
        }
    }

    public void sortNeighborsList(){
        for(Map.Entry<String, Graph> entry: database.entrySet()){
            Graph graph= entry.getValue();
            ArrayList<ArrayList<Pair_<Integer, String>>> adList=  graph.adList;
            int nodes= graph.nNodes;
            for(int i=0; i< nodes; i++)
                Collections.sort(adList.get(i));
        }
    }

    //pruning the starts edges to avoid exponential growth of the dfs code
    // for each graph we go and check upto two levels of the edges and prune
    // those which are not candidate start edges

    public ArrayList<Pair_<TrackInfo, Code>> addBackEdges(Integer srcNode, TrackInfo info){
        int [] visitOrder= info.visitOrder;
        Integer orderCounter= info.orderCounter;
        Iterator<Pair_<Integer, String>> iterator;
        ArrayList<Pair_<Integer, String>> nbrsList= currAdlist.get(srcNode);
        Pair_<Integer, String> pair;
        int temp, prevNode= info.prevNode;
        ArrayList<Pair_<TrackInfo, Code>> dfsCode= new ArrayList<>();
        for(int i=0; i < orderCounter; i++){
            iterator= nbrsList.iterator();
            temp= visitOrder[i];
            while(iterator.hasNext()){
                pair= iterator.next();
                if((pair.first!= prevNode) && (pair.first== temp)){
                    dfsCode.add(new Pair_<>(info, new Code(orderCounter, i, pair.second)));
                 //   System.out.println(pair.first+" ("+pair.second+")");
                    break;
                }
            }
        }
        return  dfsCode;
    }

    public ArrayList<Pair_<TrackInfo, Code>> getSmallestCode(ArrayList<ArrayList<Pair_<TrackInfo,Code>>> dfsCodes){
        Iterator<ArrayList<Pair_<TrackInfo, Code>>> iterator= dfsCodes.iterator();
        ArrayList<Pair_<TrackInfo,Code>> smallest= iterator.next();
        Code smallestCode, currCode;
        int len= smallest.size();
        int compRes;
        while (iterator.hasNext()){
            ArrayList<Pair_<TrackInfo, Code>> current= iterator.next();
            for(int i=0; i< len; i++){
                smallestCode= smallest.get(i).second;
                currCode= current.get(i).second;
                compRes= currCode.compareTo(smallestCode);
                if(compRes>0) break;
                else if(compRes<0){
                    smallest= current;
                    break;
                }
            }
        }
        return  smallest;
    }

    public void appendCodes(ArrayList<Pair_<TrackInfo, Code>> destList, ArrayList<Pair_<TrackInfo, Code>> srcList){
        Iterator<Pair_<TrackInfo, Code>> iterator= srcList.iterator();
        while (iterator.hasNext())
            destList.add(iterator.next());
    }

    public ArrayList<Pair_<TrackInfo, Code>> similarEdgesHandler(ArrayList<Pair_<Integer, String>> neighbors,ArrayList<Pair_<Integer, String>> startEdges,
                                               TrackInfo info) {
        Iterator<Pair_<Integer, String>> iterator = startEdges.iterator();
        ArrayList<ArrayList<Pair_<TrackInfo, Code>>> dfsCodes = new ArrayList<>();
        TrackInfo info1;
        String smallest;
        ArrayList<Pair_<Integer, String>> nextStartEdges = new ArrayList<>();
        ArrayList<Pair_<TrackInfo, Code>> dfsResult;
        Pair_<Integer, String> nbr;
        int prevCounter= info.prevCounter, prevNode= info.prevNode;
        int len= startEdges.size();
        while (iterator.hasNext()) {
            nbr = iterator.next();
            if(len>1) {
                info1 = info.clone();
            }else info1= info;
            ArrayList<Pair_<TrackInfo, Code>> pathCode = new ArrayList<>();
            pathCode.add(new Pair_<>(info1,new Code(info1.prevCounter, info1.orderCounter, nbr.second)));
          //  System.out.println(nbr.first+" ("+nbr.second+")");
           // pathCode.get(0).second= dfsResult.get(0).second;
            appendCodes(pathCode, dfsTraversal(nbr.first, info1));
            info1= pathCode.get(pathCode.size()-1).first;
            Iterator<Pair_<Integer, String>> iterator1 = neighbors.iterator();
            while (iterator1.hasNext()) {
                nbr = iterator1.next();
                if (info1.visited[nbr.first] == 0) {
                    smallest = nbr.second;
                    nextStartEdges.add(nbr);
                    while (iterator1.hasNext()) {
                        nbr = iterator1.next();
                        if (smallest.equals(nbr.second)) {
                            if (info1.visited[nbr.first] == 0)
                                nextStartEdges.add(nbr);
                        } else break;
                    }
                }
                if(nextStartEdges.size()>0) {
                    info1.prevNode= prevNode;
                    info1.prevCounter= prevCounter;
                    info1.orderCounter++;
                    appendCodes(pathCode, similarEdgesHandler(neighbors, nextStartEdges, info1));
                }
                nextStartEdges.clear();
            }
            dfsCodes.add(pathCode);
        }
        return getSmallestCode(dfsCodes);
    }


    public ArrayList<Pair_<TrackInfo, Code>> dfsTraversal(Integer srcNode, TrackInfo info){
        info.visited[srcNode]=1;
        info.visitOrder[info.orderCounter]= srcNode;
        ArrayList<Pair_<TrackInfo, Code>> dfsCode= addBackEdges(srcNode, info);
        ArrayList<Pair_<Integer, String>> children= currAdlist.get(srcNode);
        Iterator<Pair_<Integer, String>> nbrIterator= children.iterator();
        ArrayList<Pair_<Integer, String>> startEdges= new ArrayList<>();
        String smallest;
        Pair_<Integer, String> nbr;
        while (nbrIterator.hasNext()){
            nbr= nbrIterator.next();
            if(info.visited[nbr.first]==0){
                smallest= nbr.second;
                startEdges.add(nbr);
                while (nbrIterator.hasNext()){
                    nbr= nbrIterator.next();
                    if(smallest.equals(nbr.second)){
                        if(info.visited[nbr.first]==0)
                            startEdges.add(nbr);
                    }else break;
                }
                break;
            }
        }
//        nbrIterator= startEdges.iterator();
//        while (nbrIterator.hasNext()){
//            nbr= nbrIterator.next();
//            System.out.println(nbr.first+" "+nbr.second);
//        }
//        System.out.println();

        if(startEdges.size()>0) {
            info.prevNode= srcNode;
            info.prevCounter= info.orderCounter;
            info.orderCounter++;
            appendCodes(dfsCode, similarEdgesHandler(children, startEdges, info));
        }
        return dfsCode;
    }

    public void filterCandidateStartEdges(){
        Graph graph;
        ArrayList<Pair_<Integer, String>> nbrsList;
        ArrayList<ArrayList<Pair_<TrackInfo, Code>>> dfsCodes= new ArrayList<>();
        int nodes, compareBit;
        String smallest,label;
        ArrayList<Integer> startEdges= new ArrayList<>();
        int counter=0;
        for(Map.Entry<String, Graph> entry: database.entrySet()){
            graph = entry.getValue();
            //graph= database.get("#7229");
            System.out.print(entry.getKey()+":");
            nodes= graph.nNodes;
            currAdlist= graph.adList;
            smallest= "ZZZZZ";
            startEdges.clear();
            for(int i=0; i< nodes; i++){
                nbrsList= currAdlist.get(i);
                label= nbrsList.get(0).second;
                compareBit = label.compareTo(smallest);
                if(compareBit < 0 ){
                    smallest= label;
                    startEdges.clear();
                }
                if(compareBit <= 0 )
                    startEdges.add(i);
            }
            //System.out.println(startEdges.size());
            Iterator<Integer> iter= startEdges.iterator();
            while (iter.hasNext()){
                int currNode= iter.next();
                TrackInfo info= new TrackInfo(nodes);
               // System.out.println(currNode);
                ArrayList<Pair_<TrackInfo, Code>> temp= dfsTraversal(currNode,info);
                dfsCodes.add(temp);
//                Iterator<Pair_<TrackInfo,Code>> iterator= temp.iterator();
//                while (iterator.hasNext()){
//                    Code code= iterator.next().second;
//                    System.out.print ("<"+code.srcNode+","+code.destNode+","+code.edgeCode+"> ");
//                }
//                System.out.println("\n"+temp.size()+"\n");
            }
            ArrayList<Pair_<TrackInfo,Code>> smallestDFS= getSmallestCode(dfsCodes);
            //System.out.println(smallestDFS.size());
            Iterator<Pair_<TrackInfo, Code>> iterator1= smallestDFS.iterator();
            while (iterator1.hasNext()){
                Code code= iterator1.next().second;
                System.out.print("<" + code.srcNode + "," + code.destNode + "," + code.edgeCode + "> ");
            }
            System.out.println();
        }
    }

    public static void main(String [] args){
        gSpan obj= new gSpan();
        obj.readDatabase(args[0]);
        obj.sortNeighborsList();
        //System.out.println(obj.database.get("#102").nNodes);
        //obj.printGraph();
        obj.filterCandidateStartEdges();
    }

    public void addEdge(Graph graph, String[] edgeVar){
        int srcNode, destNode;
        String srcNodeLabel, destNodeLabel, edgeLabel;
        HashMap<Integer, String> nodesMap= graph.nodesMap;
        edgeLabel= edgeVar[2];

        srcNode= Integer.parseInt(edgeVar[0]);
        destNode= Integer.parseInt(edgeVar[1]);
        srcNodeLabel = nodesMap.get(srcNode);
        destNodeLabel= nodesMap.get(destNode);
        String edgeCode1= srcNodeLabel+","+edgeLabel+","+destNodeLabel;
        String edgeCode2= destNodeLabel+","+edgeLabel+","+srcNodeLabel;

        graph.adList.get(srcNode).add(new Pair_<>(destNode, edgeCode1));
        graph.adList.get(destNode).add(new Pair_<>(srcNode, edgeCode2));
    }

    public void readDatabase(String fileName){
        File file= new File(fileName);
        try{
            BufferedReader reader= new BufferedReader(new FileReader(file));
            String tempStr;
            String graphId;
            int nNodes, nEdges;
            while((tempStr= reader.readLine())!= null){
                tempStr.trim();
                if(tempStr.charAt(0)=='#'){
                    graphId= tempStr;
                    nNodes= Integer.parseInt(reader.readLine().trim());
                    Graph graph= new Graph(nNodes);
                    for(int i=0; i< nNodes; i++){
                        graph.addNode(i, reader.readLine().trim());
                    }

                    nEdges= Integer.parseInt(reader.readLine().trim());
                    graph.nEdges= nEdges;
                    for(int i=0; i< nEdges; i++){
                        tempStr= reader.readLine().trim();
                        String[] edgeVar= tempStr.split(" ");
                        addEdge(graph,edgeVar);
                    }
                    database.put(graphId, graph);
                }
            }
            //System.out.println(database.size());
            reader.close();
        }catch (IOException ex){
            ex.printStackTrace();
        }
    }
}
