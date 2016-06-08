# Directories
INPUT_DIR=/home/raghesh/phd/sem8/pa/A2/scripts/input
BUILD_DIR=/home/raghesh/phd/tools/llvm/llvm-3.5/lib/Analysis
SRC_DIR=/home/raghesh/phd/tools/llvm/llvm-3.5/lib/Analysis
TEST_CASES_DIR=/home/raghesh/phd/sem8/pa/A2/testcases

# Escape Library
SHARED_LIB=/home/raghesh/phd/tools/llvm/llvm-3.5/Debug+Asserts/lib/LLVMEscape.so

if [ $# -lt 1 ]
then
  echo "usage: ./eval.sh <ROLL NUMBER>"
  exit
fi


# Extract student directory
tar xvzf $INPUT_DIR/$1.tar.gz -C $SRC_DIR
tar xvzf $INPUT_DIR/$1.tar.gz -C $BUILD_DIR

# Build Escape library
cd $BUILD_DIR/$1
make clean
make
if [ $? -ne 0 ]
then
  echo "Build Error"
  exit
fi
cd -

# Execute testcases
let marks=0
echo "Evaluating $1"
echo "-------------"

echo -n "$1 |" >> result
for testcase in `ls $TEST_CASES_DIR/*.c`
do
  clang -O0 $testcase -emit-llvm -S -o $testcase.ll 2> /dev/null
  opt -load $SHARED_LIB -escape $testcase.ll -o $testcase.bc 2> /dev/null > $TEST_CASES_DIR/student.out
  cat $TEST_CASES_DIR/student.out
  ./eval_escapes.py $testcase.out $TEST_CASES_DIR/student.out
  if [ $? -eq 0 ]
  then
    marks=`expr $marks + .5`
    echo -n ".5 | " >> result
  else
    echo -n "0 | " >> result
  fi
  ./eval_pointsto.py $testcase.out $TEST_CASES_DIR/student.out
  if [ $? -eq 0 ]
  then
    marks=`expr $marks + .5`
    echo -n "0.5 | " >> result
  else
    echo -n "0 | " >> result
  fi

done

echo "Total Marks = $marks / 7" >> result

rm $TEST_CASES_DIR/student.out $SHARED_LIB
rm -rf $SRC_DIR/$1 $BUILD_DIR/$1
