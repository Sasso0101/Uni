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
### Autonomous system
Internet is organised in many subnetworks which are owned by some entity (ISP, corporations, network providers). Ideally, each one of these networks is autonomous and can be configured independently of every other. These independent systems are called AS (Autonomous system) and are uniquely identified by a number. This division breaks down the problem of generating route tables in two: intra-AS routing and inter-AS routing.
OSPF is a link-state routing protocol for intra-AS routing (routing inside an autonomous system). It uses Dijkstra's algorithm to find the shortest path. Routers talk directly using IP packets (thus not using the transport layer) and using the multicast address 224.0.0.5.
### Working of the protocol
The OSPF protocol implements three procedures:
- Hello protocol: defines the exchange of messages to check which links are still active wherefore finding working neighbours
- Exchange protocol: used to exchange the known topology of the network to new neighbours
- Flooding protocol: used to inform everyone of a link change
The flooding protocol is designed so that all routers forward packets they receive on one interface to all the others. This is called *controlled flooding* end ensures that one message per link or one message per broadcast domain is sent.
### Hierarchical OSPF
In large networks, the network gets divided in a hierarchy with two levels: a backbone network and many area networks. Messages are transmitted only inside the area network and routers inside an area network only know the topology of that area and the shortest path to all other areas. Each area network has a border router which talks to other routers and advertises its own paths. Also the backbone runs OSPF routing limited to the backbone.
![](Images/Pasted%20image%2020231226132927.png)
## Distance vector routing
In this algorithm the routers only know their neighbours and the costs to reach them. Routers learn new destinations via advertisements (distance vectors).
The following notation is used:
- $N_x$: set of neighbours of router X
- $R_x$: routing table of router X
	- $R_x[d]$: row relative to destination d
		- $R_x[d].cost$: cost to reach destination d
		- $R_x[d].nexthop$: gateway to reach destination d
		- $R_x[d].time$: time at which route was set (used to invalidate routes)
- $D_x$: distance vector of router x (vector containing all destinations reachable from x and their costs) $[(d, R_x[d].cost): d \in R_x]$
```python
# Initialises routing table of router x
foreach (n in N_x) do
	R_x[n].cost = c(x, n)
	R_x[n].nexthop = n
	R_x[n].time = now()
# Every T seconds send out a distance vector to all neighbours
D_x = new distance vector
foreach (d in R_x) do
	D_x[d] = (d, D_x[d].cost)
foreach (n in N_x) do
	send(D_x, n)
# Update table when we receive a new distance vector from router y
foreach ((d, cost) in D_y) do
	if (d !in R_x or cost + c(x, y) < R_x[d] or y == R_x[d].nexthop) then
		R_x.cost = cost + c(x, y)
		R_x.nexthop = y
		R_x.time = now()
```
### Example
At the beginning all routers know only their neighbours and the costs to reach them.
![](Images/Pasted%20image%2020231226163507.png)
After Y has sent its distance vector, routers X and Z update their routing tables. X discovers that by going through Y it can reach Z with just a cost of 3. Same happens for Z with X.
X and Z transmit their vector, but nothing has to be changed in the tables.
![400](Images/Pasted%20image%2020231226163805.png)
![400](Images/Pasted%20image%2020231226163820.png)
### Count-to-infinity problem
The count-to-infinity problem happens when a good link breaks down. The hosts that used that link and that are not directly connected are not informed of the change, and therefore continue sending packets in that direction. The router connected to the link that broke, retransmits back the packet. The router that sent the packet updates the cost and if that path is still the cheapest it will retransmit the packet over that path. This will continue until the broken link will be more expensive than the alternatives.
![](Images/Pasted%20image%2020231226165405.png)
![](Images/Pasted%20image%2020231226165446.png)
There have been developed some solutions to the count-to-infinity problem:
- Limit the maximum number of hops to 15: this reduces the time to convergence
- Split horizon: when router sends a distance vector to another router, it doesn't send the destinations reachable through that router (in the previous example router B will omit send to router A the route for NTW_1)
- Poisoned reverse: same as in split horizon, but instead of not sending it send that the cost is inf
The propagation of information through the network is slow (especially when network conditions are deteriorated), because it takes time of a system to reach equilibrium. While the routers are figuring out the best route loops can happen, even with the split horizon or poisoned reverse technique.
## RIP protocol
The Routing Information Protocol is a protocol for intra-AS routing (routing internal to an Autonomous system) and is implemented using distance vectors. The protocol is simple to implement and to manage, but has slow convergence and can handle a limited network size.
The cost in the RIP protocol is given by the hop count (limited to 15). Every 30 seconds (or when the routing table changes) the router sends using UDP on port 520 and as destination address the multicast address 224.0.0.9 its distance vector.
## Comparison of link-state and distance vector algorithms
Number of messages:
- link-state with n nodes and e links: O(nE)
- distance vector: exchange between neighbours only
Speed of convergence:
- link-state: $O(n^2)$, but can have oscillations
- distance vector: convergence time varies, loops can happen, count-to-infinity problem
Robustness: if a router malfunctions and advertises a wrong cost, in the distance vector case the error propagates through the network
## BGP protocol
The Border Gateway Protocol is a routing protocol that deals with routing among different autonomous systems. BDP can run both between two peers in the same AS (iBGP or _Interior Border Gateway Protocol_) or between different AS (eBGP or _Exterior Border Gateway Protocol_).
Two BGP routers ("peers") exchange BGP messages over a semi-permanent TCP connection. In these messages they advertise paths to different destination network prefixes (BGP is a path vector protocol).
A route message in BGP includes the prefix and some attributes. Borger routers can accept an advertisement and add it to their own table or ignore it. The can also decide if to advertise that path to other ASes.
A router can learn more that one route to a destination AS, and can pick among them base on different parameters, such as:
1. Local preference
2. Shortest AS-PATH
3. closest NEXT-HOP router (hot potato routing) - Try to reduce time of packet inside the network as mush as possible
## Difference between Intra-, Inter-AS routing
There are 3 main reasons why there have been developed different protocols for Intra- (internal) and Intra- (external) AS routing, namely:
- Policy: administrators want control over how traffic is routed internally, while externally there are no policies needed
- Scale: hierarchical routing saves table size, reducing update traffic
- Performance: in internal networks performance in key, while in inter-AS routing also policies can play a role