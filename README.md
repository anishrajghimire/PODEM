### PODEM

Implementation of an ATPG using PODEM (Path Oriented Decision Making) 

  ```
  $ make
  $ ./project c17.isc c17.out
  ```
  * Any ISCAS'85 benchmarks can be used. [iscas85](http://www.pld.ttu.ee/~maksim/benchmarks/iscas85/isc/)
  * "c17.out" is the output file containing the test patterns.
  * Different Test patterns are obtained by injecting the fault (giving Stuck-at-fault 0 (SA0) and Stuck-at-fault 1 (SA1) to each node of the circuit.)
  * For example: c17 circuit consists of 17 nodes, so the total number of faults are 34 that results 34 test patterns. 
