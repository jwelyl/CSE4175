# 1. How to compile
## 1-1. For debugging
g++ -g -Wall -o distvec_20161663 distvec_20161663.cc && \
g++ -g -Wall -o linkstate_20161663 linkstate_20161663.cc

## 1-2. For submit
g++ -Wall -o distvec_20161663 distvec_20161663.cc && \
g++ -Wall -o linkstate_20161663 linkstate_20161663.cc

# 2. How to run
./linkstate_20161663 topologyfile messagefile changesfile
./distvec_20161663 topologyfile messagefile changesfile

## 2-1. example
./linkstate_20161663 topology.txt messages.txt changes.txt
./distvec_20161663 topology.txt messages.txt changes.txt

# 3. How to test
cmp output_ls.txt output_ls_ans.txt
cmp output_dv.txt output_dv_ans.txt

# 4. How to clean
rm linkstate_20161663 distvec_20161663 output_ls.txt output_dv.txt
