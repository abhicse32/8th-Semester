#!/usr/bin/python
import sys

def createEscapesExpectedOutput():
	expected_escapes_d['test1'] = set(['call'])
	expected_escapes_d['test2'] = set(['call'])
	expected_escapes_d['test3'] = set(['call', 'call1', 'call3'])
	expected_escapes_d['test4'] = set(['call1'])
	expected_escapes_d['test5'] = set(['call1', 'call2', 'call3'])
	expected_escapes_d['test6'] = set(['call', 'call1', 'call2', 'call4'])
	expected_escapes_d['test7'] = set(['call', 'call1', 'call3']) 


def createStudentEscapeOutput(student_outfile):
	f = open(student_outfile)
	content = f.read()
#print content
	lines = content.split('\n')
	escapes = set(lines[0].split())
	return escapes 

def evaluateEscape(student_escapes):
	expected_escapes = expected_escapes_d[testcase]

	print expected_escapes
	print student_escapes 
	if not (expected_escapes == student_escapes):
		print "Escape is wrong"
		return False
	return True

student_outfile = sys.argv[2]
inputfile = sys.argv[1]
#print student_outfile
expected_escapes_d = {} 
total_marks = 0

testcase = inputfile.split('/')[-1].split('.')[0]

createEscapesExpectedOutput()

student_escapes = createStudentEscapeOutput(student_outfile)
if (evaluateEscape(student_escapes)):
	print testcase,  ": passed"
	print
	exit(0)
else:
	print testcase,  ": failed"
	print
	exit(1)
