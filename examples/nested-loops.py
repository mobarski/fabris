from time import time
t0=time()
x=16
out = 0
xr = range(x)
for _ in xr:
	for _ in xr:
		for _ in xr:
			for _ in xr:
				for _ in xr:
					for _ in xr:
						out += 1
print(out)
print(time()-t0)
