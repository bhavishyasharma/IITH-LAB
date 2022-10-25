import numpy as np
from scipy.linalg import null_space

def is_vertex(A,B,sol,n):
    thisx = (np.matmul(A,sol.transpose()))
    tight = 0
    for i in range(len(B)):
        if(round(B[i],1)==round(thisx[i],1)):
            tight=tight+1
    return (tight==n)

def get_tight_matrix(A,B,sol):
    print("Getting Tight Rows...")
    thisx = (np.matmul(A,sol.transpose()))
    tight = 0
    tight_matr=[]
    for i in range(len(B)):
        if(round(B[i],1)==round(thisx[i],1)):
            tight=tight+1
            tight_matr.append(A[i])
    tight_matr=np.array(tight_matr)
    return tight_matr

def next_vertex(A,B,present,along,n):
    print("Finding Next vertex...")
    len=1
    sol=present+len*along
    ver=sol
    counter = 0
    while(not is_vertex(A, B, sol, n)):
        len=round(len+0.1,1)
        sol=present+len*along
        ver=sol
        counter += 1
        if counter == 100000:
            return True, ver
        
    return False, ver

def is_optimum(matr_comb,ans,c,maxm):
    print("Checking if vertex is optimum...")
    b1 = np.linalg.inv(matr_comb)
    b1 = np.negative(b1)
    col1 = []
    for i in range (np.shape(b1)[1]):
        col1.append(list(b1[:,i]))
    col  = np.array(col1) 
    new_ans=[]
    along=[]
    for l in col:
        if (np.matmul(c,l.transpose()))>0:
            new_ans=(ans+l)
            along=l
    if(len(new_ans) and np.matmul(c,new_ans.transpose())>maxm):
        print("Solution not optimum...")
        return False,along
    return True,[]


def check_constraint(A,B,sol):
    thisx = (np.matmul(A,sol.transpose()))
    for i in range(len(B)):
        if(round(B[i],1)<round(thisx[i],1)):
            return False
    return True


def read_vector(n):
    data = []
    while(len(data)<n):
        data.extend(list(map(float, input().split())))
    return np.array(data)

def read_matrix(row, col):
    data = []
    while(len(data)<row*col):
        data.extend(list(map(float, input().split())))
    return np.array(data).reshape(row, col)

def main():
    line = input('No. of Rows & Columns : \n')
    if(len(line.split())==2):
        row, col = map(int ,line.split())
    else:
        row = int(line)
        col = int(input())

    print('Enter {row}x{col} Matrix : '.format(row=row, col=col))
    matrixA = read_matrix(row, col)

    print('Enter {n} dimension B vector : '.format(n=row))
    vectorB = read_vector(row)

    print('Enter {n} dimension C vector : '.format(n=col))
    vectorC = read_vector(col)

    print('Enter {n} initial feasible points : '.format(n=col))
    initialFeasiblePoint = read_vector(col)

    possible_solution = initialFeasiblePoint
    unbounded = False
    if(not is_vertex(matrixA, vectorB, possible_solution, col) and check_constraint(matrixA, vectorB, possible_solution)):
        print("Going from inner point to vertex...")
        tight_matr=get_tight_matrix(matrixA, vectorB, possible_solution)
        print(tight_matr)
        ns = null_space(tight_matr)
        ns=ns.flatten()
        along =  ns
        along = along / min(along)
        unbounded, possible_solution = next_vertex(matrixA, vectorB, possible_solution, along, col)
        print("Next Vertex is : " + str(possible_solution))

    optimum = False
    counter = 0
    while(not optimum and not unbounded):
        maximum = np.matmul(vectorC, possible_solution.transpose())
        print('Value C.X = ' + str(maximum))

        tight_matrixA = get_tight_matrix(matrixA, vectorB, possible_solution)
        print(tight_matrixA)

        optimum, along = is_optimum(tight_matrixA, possible_solution, vectorC, maximum)
        if(optimum):
            print('Optimum Solution Found!')
            if(check_constraint(matrixA, vectorB, possible_solution)):
                print('Constraint Satisfied!')
            else:
                optimum = False
                b1 = np.linalg.inv(tight_matrixA)
                b1 = np.negative(b1)
                col1 = []
                for i in range(np.shape(b1)[1]):
                    col1.append(list(b1[:,i]))
                col1 = np.array(col1)
                direction = []
                along = []
                for l in col1:
                    direction.append(np.matmul(vectorC, l.transpose()))
                for l in col1:
                    if(np.matmul(vectorC, l.transpose())==max(direction)):
                        along = l
                        break
        if(not optimum):
            print('Non-Optimum Solution Found!')
            unbounded, possible_solution = next_vertex(matrixA, vectorB, possible_solution, along, col)
            print('Next Vertex : ' + str(possible_solution))

    if unbounded:
        print('Region is unbounded!')
    else:
        print('Region is bounded!')

    print('Final Solution : ')
    print('Vector X : '+ str(possible_solution))
    print('Optimum Value of C.X : '+str(maximum))

if __name__=='__main__':
    main()