from time import time
import sys

def fib(n):
    if (n < 2):
        return(1)
    return( fib(n-2) + fib(n-1) )

t0=time()
n = 32
#sys.setrecursionlimit(3000)
print(fib(n))
print(time()-t0)

# n=32 -> 6s, fabris->0,6s
