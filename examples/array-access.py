import sys

def main():
    n = int(sys.argv[1])
    x = n * [0]       
    y = n * [0]
    for i in xrange(0,n):
        x[i] = i + 1
    for k in xrange(0,1000):
        for i in xrange(n-1,-1,-1):
            y[i] += x[i]
    print y[0], y[-1]

main()
