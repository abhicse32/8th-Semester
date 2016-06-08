#!/usr/bin/python
import sys

def createPoinstsToExpectedOutput():
    expected_pointsto_d['test1'] = {'i': set(['call']), 'j': set(['call1'])}
    expected_pointsto_d['test2'] = {'i': set(['call']), 'j': set(['call1']), 'k': set(['call'])}
    expected_pointsto_d['test3'] = {'i': set(['call']), 'j': set(['call1']), 'm': set(['call2'])}
    expected_pointsto_d['test4'] = {'i': set(['call']), 'j': set(['call1']), 'k': set(['call1'])}
    expected_pointsto_d['test5'] = {'a': set(['call']), 'i': set(['call1']), 'j': set(['call2']), 'k': set(['call3'])}
    expected_pointsto_d['test6'] = {'a': set(['call']), 'j': set(['call1', 'call4']), 'k': set(['call2']), 'b': set(['call3'])}
    expected_pointsto_d['test7'] = {'a': set(['call', 'call3']), 'k': set(['call1']), 'i': set(['call2'])}

def createStudentPointsToOutput(student_outfile):
    f = open(student_outfile)
    content = f.read()
#print content
    lines = content.split('\n')

    pointers = []
    pointees = set([])
    d = {}
    for line in lines[1:]:
				variables = line.split()
				if len(variables) > 0:
						pointer = variables[0]
						pointees = set(variables[1:])
						d[pointer] = pointees
    f.close()
    return d

def evaluatePointsTo(studentd):
    expectedd = expected_pointsto_d[testcase]

    print expectedd
    print studentd 
    for pointer in expectedd:
				pointees_expected = expectedd[pointer]
				if not studentd.has_key(pointer):
						print "Pointer", pointer , "not found"
						return False
				pointees_student = studentd[pointer]
				if not ((pointees_expected & pointees_student) == pointees_expected):
						print "Pointees list of", pointer, "is wrong"
						return False
    return True

student_outfile = sys.argv[2]
inputfile = sys.argv[1]
#print student_outfile
expected_pointsto_d = {}
total_marks = 0

testcase = inputfile.split('/')[-1].split('.')[0]

createPoinstsToExpectedOutput()

student_pointsto_d = createStudentPointsToOutput(student_outfile)
if (evaluatePointsTo(student_pointsto_d)):
		print testcase,  ": passed"
		print
		exit(0)
else:
		print testcase,  ": failed"
		print
		exit(1)
