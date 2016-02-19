import sys

def main():
    nl = nw = nc = 0
    rl = sys.stdin.readlines
    lines = rl(4096)
    while lines:
        for line in lines:
            nl += 1
            nc += len(line)
            nw += len(line.split())
        lines = rl(4096)

    print "%d %d %d" % (nl, nw, nc)

main()

