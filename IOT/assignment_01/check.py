import itertools
f=open('output.txt','r')
f2=open('ref_output.txt','r')

ref=list(range(1,7))
for line,line2 in zip(f,f2):
    line = line.split()
    line = list(map(int, line))
    tline = list(map(int, line))
    print(line)
    line.sort()
    print(line)
    if(line==ref):
        print("No Conflict")

    line2=line2.split()
    line2=list(map(int,line2))
    if(line2==tline):
        print("Matches with reference ")

