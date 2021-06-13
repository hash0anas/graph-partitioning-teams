### What this is
- This is a simple tool that groups a number of people into any number of teams based on a set of preferences, interests and big-5 personality traits. It can be extended to partition based on any number of additional factors, with some adjustments to the code in `preprocess.cpp`.

### How it works
 - Construct an undirected graph where the vertices are the people to be partitoined.
 - Edges reprsent the connection strength between each two.
 - The graph is complete, i.e. there's an edge between each two vertices. (`n(n-1)` edges in total)
 - The weight on each edge is the weighted sum of:
 	- 30% preferences (whether the two people chose each other (1), or just one of them chose the other (1/2), none chose the other (0)).
 	- 50% interests (the cardinality of the interesection set, between their interests).
 	- 20% big-5 (how different they are to each other, the more different = the higher the score and stronger the connection).
 - `preprocess.cpp` handles all of this, but it has to be given the input from the excel file.
 - `partition.nb` is the Mathematica notebook, the algorithm for partitioning a graph is there.

### How to use it
 - `random-generator.cpp` is just there to test the preprocessing on randomly generated data, it isn't needed to use the tool.
 - After compiling the `preprocess.cpp` code, feed the data from the excel sheet to it.
 - **Make sure your data is formatted exactly the same as the excel sheet.**
  - People's ids must be consecutive, from 1 to n.
 - After the preprocessing is done, the adjacency matrix is outputted to `connections.txt`.
 - Copy the ajd. matrix into the Mathematica notebook, and assign it to the variable `clans`.
 - Run the notebook, and you should be all set.

**Suggestions on the approach/code are really welcome, because this was just my attempt at a such complicated NP-complete problem. 
And I am by no means an expert in the field of graph theory or graph partitioning. I am just an undergrad student who just happens to be very fascinated by graph theory.**

# Resources
- https://en.wikipedia.org/wiki/Graph_partition
- Bichot, Charles-Edmond; Siarry, Patrick (2011). Graph Partitioning: Optimisation and Applications. ISTE – Wiley. p. 384. ISBN 978-1848212336.
- https://en.wikipedia.org/wiki/Kernighan%E2%80%93Lin_algorithm