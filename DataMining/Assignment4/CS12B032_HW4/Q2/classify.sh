#!/bin/bash
#script is used for graph-classification purpose
echo "Compilation starts......"
javac -d . ../src/GraphClassification.java
echo "done"
echo "Program run starts......"
java GraphClassification $1 $2 $3 $4
# echo "done"
# echo "converting training input into required format for gSpan"
# python formattted.py generatedFile.txt out1.txt elements.txt gSpanInput.txt
# echo "done"
# echo "running gSpan on formatted training input...."
# ./gSpan -f gSpanInput.txt -s 0.1 -i -o > temp
# echo "done"
echo "creating features Matrix....."
java Creator
python classification.py $4

