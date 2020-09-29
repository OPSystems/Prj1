all: MyCompress MyDecompress ForkCompress PipeCompress ParFork MiniShell MoreShell DupShell ParThread

MyCompress: MyCompress.c
	cc MyCompress.c -o MyCompress

MyDecompress: MyDecompress.c
	cc MyDecompress.c -o MyDecompress

ForkCompress: ForkCompress.c
	cc ForkCompress.c -o ForkCompress

PipeCompress: PipeCompress.c
	cc PipeCompress.c -o PipeCompress

ParFork: ParFork.c
	cc ParFork.c -o ParFork

MiniShell: MiniShell.c
	cc MiniShell.c -o MiniShell

MoreShell: MoreShell.c
	cc MoreShell.c -o MoreShell

DupShell: DupShell.c
	cc DupShell.c -o DupShell

ParThread: ParThread.c
	cc ParThread.c -o ParThread -lpthread