### What this is:
- This is a 'relatively' simple tool that groups a number of people into any number of teams based on a set of preferences, interests and big-5 personality traits.
  It can be extended to partition based on any number of additional factors, with some adjustments to the code in `preprocess.cpp`.
- The partitioning is based on the graph partitioning problem.

### How it works:
 - Construct an undirected graph where the vertices are the people to be partitoined (grouped).
 - Edges reprsent the connection strength between each two indivisuals.
 - The graph is complete, i.e. there's an edge between each two vertices. (`n(n-1)/2` edges in total)
 - The weight on each edge is the weighted sum of:
 	- 30% preferences (whether the two people chose each other (1), or just one of them chose the other (1/2), none chose the other (0)).
 	- 50% interests (the cardinality of the interesection set, between their interests).
 	- 20% big-5 (how different they are to each other, the more different = the higher the score and stronger the connection).
    - Of course, they can be adjusted depending on your needs. Other factors can be introduced too.
 - `preprocess.cpp` handles all of this, but it has to be given the input from the excel file.
 - `partition.nb` is the Mathematica notebook, the algorithm for partitioning the graph is there.

### How to use it:
 - After compiling the `preprocess.cpp` code, feed the data from a csv file to it.
 - **Make sure your data is formatted exactly the same as the sample data file named `data.xlsx`**
 - People's ids must be consecutive, from 1 to n.

 - After running `preprocess.cpp` successfully, an adjacency matrix should be outputted to `connections.txt`.
 - Copy the adjacency matrix, which is the whole `connections.txt`, into the Mathematica notebook such that it's assigned to the variable `clans`.
 - Run the notebook, and you should be all set.

### How good was the partition? (Optional)
 - Graph partitioning is an NP-hard problem, which means there's no known algorithm that can compute the optimal solution in polynomial time, at least as of Apr. 2022.
 - The partitioning is derived using some heuristics and approximation algorithms.
 - If you're willing to see how good the partition is, you can take the results of the partition and input them back to `preprocess.cpp` as described.
 - Then some stats will be outputed to the console.

**Suggestions on the approach/code are really welcome, because this was just my attempt at a such complicated NP-hard problem. 
And I am by no means an expert in the field of graph theory or graph partitioning. I am just someone who happens to be very fascinated by graph theory.**

## Resources
- I'd start here: https://en.wikipedia.org/wiki/Graph_partition
- Bichot, Charles-Edmond; Siarry, Patrick (2011). Graph Partitioning: Optimisation and Applications. ISTE – Wiley. p. 384. ISBN 978-1848212336.
- Possibly the algorithm that Mathematica uses: https://en.wikipedia.org/wiki/Kernighan%E2%80%93Lin_algorithm
- Video lecture: https://www.youtube.com/watch?v=zZae_C2BU_4

