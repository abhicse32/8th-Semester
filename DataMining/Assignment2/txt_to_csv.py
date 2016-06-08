writeTo = open("retail.csv","w")
with open("retail.dat","r") as readFile:
	for lines in readFile:
		lines=lines.strip()
		lines=lines.replace(' ', ',')
		writeTo.write("%s\n"%lines)
writeTo.close()
