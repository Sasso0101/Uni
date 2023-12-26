## General principles
This chapter will deal with routing algorithms, i.e. algorithms that determine "good" paths in the network of routers.
A path is a sequence of routers that a packet must travel to reach a destination. A "good" path may be the least expensive, least congested, fastest...
### Abstraction of a network
A network can be seen as a non-oriented graph where nodes are routers and links are connections between them. A graph G is denoted by $G=(N, E)$, where N is the set of routers and E is the set of links.
$$N = \{U, V, X, W, Y, Z\}$$
$$E = \{(u, v), (u, x), (v, w), (v, x), (x, w), (x, y), (y, z), (w, z), (u, w), (w, y)\}$$
![300](Images/Pasted%20image%2020231225113815.png)
Each link has an associated cost, which is given by the cost function $c: N \times N \to \mathbb{N}$. For example for the link $(w, z)$ the associated cost is $c(w, z) = 5$.
The cost can be determined on many parameters, such as number of hops, inverse of  bandwidth (higher bandwidth -> lower cost), congestion...
### Types of routing algorithms
We can categorise routing algorithms on 2 characteristics:
- Global or decentralised information: for global algorithms all routers know everything about the network. In the other case routers know only their neighbours and have to talk to them to gain more information. The first algorithms are called *link state* algorithms, while the second are *distance vector* algorithms
- Static or dynamic tables: if routing tables are static they are defined once by the network administrator and don't change with time, thus routing algorithms are not needed. If tables are dynamic they are periodically updated to respond to changes in the costs of the network.
## Dijkstra's algorithm
Dijkstra's algorithm is a link-state algorithm. This means that all routers know all the link costs of the network. They achieve that by exchanging information with each other. The algorithm is run for every destination, so that after *k* iterations we know the cost *k* destinations. The notation used is the following:
- $c(x, y)$: link cost from node x to y ($\infty$ if no direct neighbours)
- $D(v)$: cost of path from source to destination v
- $p(v)$: predecessor of router v in the path
- $N$: set of all routers
- $N'$: set of already visited routers (routers for which the minimum cost has already been computed)
```python
# Start from node u
N` = {u}
# Initialise cost table
foreach (v in U) do
	# Find adjacent nodes
	if (v in u.adj()) then
		# Add node to cost table
		D(v) = c(u, v)
	else
		D(v) = inf

do
	# From the set of unvisited routers (N - N`) take the one with the minimum associated cost
	node w = min(N - N`)
	N`.insert(w)
	# For each adjacent node that is not in the set of already visited routers
	foreach (v in w.adj() - N`)
		# If the cost through node w is smaller than the cost in the table save this new path
		if (D(w) + c(w, v) < D(v)) then
			D(v) = D(w) + c(w, v)
			p(v) = w
while (N`.size() < N.size()) # Repeat untill we have visited all nodes
```
Steps to compute Dijkstra's algorithm.
![](Images/Pasted%20image%2020231226122517.png)
### Complexity and problems
In a network of n nodes in the worst case all nodes except the current get checked at every iteration, therefore the time complexity is $\frac{n(n-1)}{2}=O(n^2)$. Some implementations reduce the complexity to $O(nlog(n))$.
Another problem with Dijkstra's algorithm is that if a path becomes the least expensive then all routers will choose that path, therefore making it very congested, while other paths become completely free. Then the algorithm will pick another path as the best and make that very congested, therefore inducing a lot of oscillations.
## OSPF protocol
