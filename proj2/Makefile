CC = g++
CFLAGS = -g

all: linkstate_20161663 distvec_20161663

linkstate_20161663: linkstate_20161663.cc
	$(CC) -Wall $(CFLAGS) -o linkstate_20161663 $^

divstvec_20161663: distvec_20161663.cc
	$(CC) -Wall $(CFLAGS) -o distvec_20161663 $^

clean:
	rm linkstate_20161663 distvec_20161663 changes.txt output_dv_ans.txt topology.txt messages.txt  output_ls_ans.txt output_ls.txt output_dv.txt

