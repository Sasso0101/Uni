The main network layers functions are:
- Forwarding the packet from the router input to the router output (data plane)
- Routing packets from source to destination using routing algorithms (control plane)
## Router architecture
Routers implement the network stack up to the network layer. They are logically divided in the control plane and the data plane. The control plane deals with deciding where a packet should go and is carried out by a routing processor. The control plane deals with the physically forward packets from the input port to the output port.
![](Images/Pasted%20image%2020231114092047.png)
### Switching fabric
The switching fabric deals with transferring the packet from the input to the output buffer. The performance is measured as multiples of input/output line rate. There exist three types of switching fabrics:
- Memory based: works as in normal computers, packets get saved to memory. The speed is limited by the memory bandwidth and memory has to be accesses twice to transfer one packet
- Bus based: all input and output ports share the same bus, speed limited by bus bandwidth
- Interconnection network: uses a matrix of connections to enable extremely high speeds
### Input port functions
The input port of a router is composed of:
- Line termination: receives the bits from the wire
- Link layer: protocol from the link layer to interpret the received data from medium (ex. Ethernet, fiber)
- Queue
### Consequences of switching delays
If the switching fabric is slower than the input ports combined then we will have queueing. Head-of-line queueing happens when the first datagram in the queue  blocks all other datagrams behind because his output port is already being used.
We can have queueing also at the output ports. To deal with this we can implement some scheduling algorithms that prioritise some packets rather than others (problem for net neutrality!).
## IP protocol
The IP packet has a header organised in 32 bit blocks. Its most important fields are:
- Version: IPv4 or IPv6
- Header length: number of 32-bit blocks
- Total length: number of bytes including header and data
- Identification+fragment flag+fragment offset: used to reconstruct whole packet if it was split
- Time To Live (TTL): 8-bit field initialised by the sender which will get decremented by 1 every time it goes through a router. When it gets to 0 the packet gets dropped
- Protocol: transport protocol used in the data
- Checksum: 16-bit one's complement of the header
- Source and destination ip address: 32 bit addresses
- Additional options + padding
![300](Images/Pasted%20image%2020231114092502.png)
### IP fragmentation
Every hardware technology specifies a maximum dimension of the frame (layer 2 PDU) that is transiting on it known as Maximum Transmission Unit (MTU). Since different networks can have different MTUs, it may happen that a datagram has to be split. In this case some header fields are modified:
- the fragment offset field tells at which position in the original datagram the fragment belongs
- The Flags field [0, D, M] contains the following information:
	- D - The packet must mot be fragmented
	- M - This packet is a fragment and more fragments are expected
- the identifier field uniquely identifies fragments of the same datagram
Fragments get reassembled only at the destination, so the routers don't have to deal with waiting for all fragments to arrive and fragments don't need to take the same route. The receiver waits for all fragments to arrive by buffering them. If the maximum waiting time is exceeded, fragments that have arrived are dropped and the sender has to send them again.
Fragments is nowadays disabled on the public internet, therefore datagrams that are too large get automatically discarded by routes. This is due to security risks:
- firewalls may be deceived by checking a fragment, allowing it through and then not checking new fragments that are sent if the initial ones get lost
- Memory exhaustion attacks by intentionally omitting fragments
- Buffer overflows due to a bad implementation of the reassembling procedure
### IPv4 addressing
Interface: connection between host/router and physical link
IP addresses are 32 bit numbers assigned to host and router interfaces. IP addresses are represented in dotted decimal notation, where each 8-bit segment is represented in decimal value, with periods as separators. IP addresses are divided in two parts:
- Network ID, which identifies the physical network to which the host is attached
- Host ID, which identifies the network interface
For the network to work there must be the guarantee that each host has a unique IP address and that network ID are coordinated globally (host IDs are coordinated locally instead).
#### Addressing classes
Initially to distinguish between the network part and the host part of an IP address static classes of IP addresses were defined. Since everyone wanted the bigger classes, they run out of them quickly, so a new method of assigning Ip addresses had to be found.
#### Classless addressing
A central authority called ICANN (Internet Corporation for Assigned Names and Numbers) to handle address assignment has been established. The authority gives to registrars the authority to assign IP blocks to ISPs, which in turn sell them to organisations and customers.
In classless addressing the number of bits that denote the network is customisable. This is done by specifying the network mask. The network mask is a 32-bit number, where the bits are set to 1 if there are part of the network ID. Therefore, to identify the Network ID of an IP address it is sufficient to do a bitwise AND between the IP address and the network mask. The network mask can be also represented in CIDR (Classless Inter-Domain Routing) notation, which is a decimal number denoting the number of bits denoting the Network ID.
##### Forwarding table
Routers internally use a forwarding table, which indicates on which interface the incoming packet should be routed. To choose the right interface, they perform a bitwise and between the destination IP address and the network mask of every entry in the table. If the result matches an entry in the table, the packet is forwarded on that interface. If multiple entries match, the entry with the longest Network ID is chosen. The last entry is the table is usually 0.0.0.0/0, which matches all IP addresses, and packets get forwarded to this interface if no packet matches.
If there are multiple routers in the same network, the router must save in the forwarding table also the IP address of the router which should handle the packet we are forwarding. This field is called *next hop*.
### Private IP addresses and NAT
Not all Ip addresses are public: some are reserved for private networks. These IP addresses can be used only in local networks and are not reachable by devices outside that specific network. There are three blocks of IP addresses assigned to private networks:
- 10.0.0.0/8: from 10.0.0.0 to 10.255.255.255
- 172.16.0.0/12: from 172.16.0.0 to 172.31.255.255
- 192.168.0.0/16: from 192.1168.0.0 to 192.168.255.255
Since they are not accessible from external networks, all local networks can reuse the same IP address blocks.
Since it is not possible to communicate from the network to devices with private IP addresses, two solutions have been developed to address this problem:
- NAT (network address translator): device which swaps the source IP address and port of the packets it receives with the public IP address and a random port
- Proxy: a computer with a public and a private IP address which receives the requests from private hosts and executes them on their behalf on the public network. This is done at the application layer
The NAT has an internal translation table, which keeps track of the internal private IP address and port which has sent the packet and the corresponding public destination IP address and port, so when the NAT receives a response back from the public side, knows to which host to forward the packet to. The advantages of using NAT are that we can have $2^{16}\approx 60000$ connections with a single IP address and that local hosts are unaccessible from the external network, except when they are the ones establishing the connection. Although is has the benefit of addressing the IP shortage, the implementation of NAT is controversial because routers modify packets at level 3 and 4, therefore applications at higher levels have to manage also this cases (ex. P2P applications). Moreover, special techniques have to be adopted to make a local client accessible from the outside world (ex. port forwarding or hole punching).
### Special IP addresses
#### Network addresses
They are addresses that refer to the prefix of the network and have all host bits set to 0.
$$ \underbrace{10000000.11010011.00000000.0001}_\text{NetID}\underbrace{0000}_{\text{HostID}} $$
#### Direct broadcast addresses
When a packet reaches the network specified in the NetID, the packet gets sent to all the hosts in the network. This address has all host bits set to 1.
$$ \underbrace{10000000.11010011.00000000.0001}_\text{NetID}\underbrace{1111}_{\text{HostID}} $$
#### Limited broadcast addresses
Packets with this broadcast address never get forwarded by routers, so they are directed only to hosts in the same network.
$$ 255.255.255.255 \Rightarrow 11111111.11111111.11111111.11111111 $$
#### This computer address
When a host does not yet know its IP address, it can use this special IP address to identify itself.
$$ 0.0.0.0 \Rightarrow 00000000.00000000.00000000.00000000 $$
#### Loopback address
Loopback addresses are all addresses in the 127.0.0.0/8 subnet and are used to test applications running in the same host. When an application sends a packet with this address, the protocol stack travels down to the network layer and then directly travels up to forward the packet to the right application without ever leaving the host.
#### Multicast
There are various ways of achieving a multicast communication (sending the same packet to a subset of host).
##### Multicast addresses
Multicast addresses are used to send a packet to all hosts. Then hosts can decide to keep them or drop them if not interested. Packets should be duplicated and sent to all hosts by all routers, but their usage is normally blocked on the internet. These special addresses start with 1110 and therefore are those between 224.0.0.0 and 239.255.255.255.
##### Multiple unicast
The source send one copy of the packet for each host interested. This method is suboptimal because it may require sending the same exact packet in the same link multiple times.
##### Multicast routing
Special protocol have been developed to achieve multicast while minimising network overhead.
#### Link-local addresses
These exists a subnet reserved for hosts which cannot find an IP address. Hosts choose a random IP address from the 169.254.0.0/16 and use that IP address to communicate with each other.
#### IP addresses for routers
Each router can have two or more IP addresses, one for each logical network to which is connected.
For example, a router can act as a bridge for multiple subnets, but all these subnets are connected to the router on the same network interface.
![](Images/Multiple%20IP%20addresses.png)
### ARP protocol
Every IP packet gets encapsulated inside a level 2 frame and to send a level 2 frame the source and destination address of that layer are needed. These addresses are called MAC addresses and are 48-bit long numbers, which uniquely identify every network interface. While source and destination IP addresses remain unchanged during the whole trip of the packet, MAC addresses change at every hop. To go from level 3 to level 2, it is therefore needed to translate the IP address of the next hop to a MAC address. A host can discover the MAC address of another host in the network by using a level 2 protocol called ARP (Address Resolution Protocol). To do this, the host sends a broadcast request asking for the MAC address of host with IP address X. When host X receives the ARP request, it replies with its MAC address.  When it travels through a physical network an ARP message is encapsulated in a hardware frame.
#### ARP message
![](Images/ARP%20message.png)
The Hardware type field specifies the network link protocol type, therefore the type of address in PADDR. The message structure is the same for both request and response, so request field tells if this is a request or response (1 request, 2 response).
To send an ARP request use the command arping.
ARP responses are cached in an ARP table and updated periodically (every 30s).