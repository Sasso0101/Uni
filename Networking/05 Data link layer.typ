= Data link layer
The data link layer deals with transferring datagrams from one node to the physically adjacent node over a link. The link may be wired or wireless. The packets at layer 2 are called frames and they encapsulate the packet from the higher layer (datagram). Frames have a header and a trailer. To identify source and destination MAC addresses are used.

This division allows a datagram to be transported over different types of mediums (ex. first Ethernet, then wifi) and over connections that offer different types of services, such as reliability.

The link layer is implemented in every host inside an "adaptor" (ex. a network interface card) or directly inside the firmware of a chip (ex. Ethernet). The component is then connected to the host's system bus to provide connectivity to the system.

The communication procedure is similar to the procedure of the other layers. The sending side encapsulates the datagram in a frame, adds error checking bits and information for flow control. The receiving side collaborates in the flow control procedures, extracts the datagram, looks for errors and passes the data to the higher layers.
== Link layer services
The services offered by the link layer are the following:
- Reliable delivery between adjacent nodes: can always be implemented, but usually ignored on links with a low probability of errors (ex. fiber)
- Flow control: transmission speed is adjusted based on the capabilities of the sender and the receiver
- Error correction: receiver can identify and correct single bit errors without the need of retransmission
- Link sharing: nodes can transmit at the same time (full-duplex) or at different times (half-duplex)
== Error detection
Error detection and correction algorithms add some redundancy bits to the message. The receiving side then checks if the ECD bits match the received data. The error detection may not be 100% reliable, but usually increasing the EDC field size improves the accuracy.
=== Parity bit
This technique can be implemented in one or two dimensions. In the first case a single bit gets added which is 1 if the number of ones in the original string is odd. In the 2D case a parity bit gets added for every row and column.
#figure(
  image("Images/Parity bit.png", width: 70%)
)
=== Redundancy
This technique add redundancy to every character and reorders them. This technique works well in the case of burst errors (errors of bits in succession).

The technique goes as follows:
1. Given a string, add some redundancy to each character: HELLO -> HHH EEE LLL OOO
2. Interleave the characters (reorder them): HEL LOH ELL OHE LLO
3. The receiver receives a modified string: HEL LOH EXX XXX LLO
4. After reordering, the received string becomes: HHX EEX LXL LXL OXO
5. By choosing the most common letter for each group, the receiver is able to reconstruct the original message
=== Cyclic Redundancy Check
CRC is a more powerful but also more involved error detection technique than the ones seen so far. The components needed for this algorithm are:
- *D*: data we want to transmit in binary
- *G*: "generator" number known both to the sender and the receiver. Its length in binary is $r+1$ bits
The technique goes as follows:
1. Add r bits to the end of *D*. Mathematically $D dot.op 2^r$ (multiplication shifts bits to the left)
2. Find the remainder *R* by dividing $D dot.op 2^r$ by *G* using modulo 2 arithmetic (instead of subtractions perform XOR operation). Mathematically $R="remainder"((D dot.op 2^r)/G)$
3. Add the remainder *R* to *D*. This can be done by the operation $(D dot.op 2^r) "XOR" R$  and is represented by the notation *$angle.l D, R angle.r$*.
4. The number we obtained is a multiple of *G*. To check the correctness after transmission we need to divide it by *G* using modulo 2 arithmetic and check that the remainder is zero.
Example:
$ D = 101110, G=1001 arrow.r.double R=011 $
#figure(
  image("Images/Division 1.png", width: 40%)
)
$ "Transmitted message": 101110underbrace(000, "r bits") "XOR" 011 = 101110underbrace(011, "CRC") $
If we receive the message without errors and we divide by the generator, we get that the remainder is zero. Otherwise it its very likely that the result is not a multiple of the generator, thus the remainder will not be zero, signalling that an error happened in the transmission.
#figure(
  image("Images/Division 2.png", width: 40%)
)
This technique is able to detect all burst errors up to r bits. If an appropriate generator number is chosen, it can detect also all single and two bit errors.
== Multiple access protocols
We can distinguish two types of connections:
- Point-to-point: nodes are connected directly (ex. PPP - Point to point protocol)
- Broadcast: nodes share the same wire or medium (ex. Ethernet, 802.11 wireless LAN)

In broadcast connections nodes have to share the same medium, therefore to avoid interference a protocol to regulate how hosts share the channel is needed. These protocols are called multiple access protocols (MAC).

An ideal multiple access protocol with bandwidth _R_ has the following characteristics:
1. When one node wants to transmit, it can transmit at rate R
2. When M nodes want to transmit, each one can transmit at an average of _R/M_
3. The protocol should be decentralised
4. The protocol should be as simple as possible

We can distinguish three types of MAC protocols:
- Channel partitioning: channel is divided into smaller pieces (time slots, frequency, code)
- Random access: nodes use the channel at random, but can detect collisions and recover
- Take turns: nodes take turns, with nodes allowed to take longer turns if they need to send more data
=== Channel partitioning protocols
In this section two techniques will be treated: TDMA and FDMA.

In TDMA (time division multiple access) access the channel in rounds. Each station is assigned a fixed time slot. If a station doesn't have to transmit, the channel stays idle during its time slot.
#figure(
  image("Images/TDMA.png", width: 70%),
  caption: "Example of a 6 station LAN using TDMA."
)
In FDMA (frequency division multiple access) the channel is dividend into frequency bands, where each station is assigned a fixed frequency band. If a station doesn't have to transmit, its assigned frequency band stays idle.
#figure(
  image("Images/FDMA.png", width: 70%)
)
=== Random access protocols
In random access protocols when a node as a packet to send it will transmit it at the full channel data rate R, without coordinating with the other nodes. If two or more nodes try to transmit at the same time, a collision will happen. These protocols define how to detect collisions and recover from them.
==== Slotted ALOHA
In slotted ALOHA time is divided into slots of the same length. All hosts are synchronised and can transmit frames only at the beginning of a time slot. If a collision is detected, every node will retransmit the frame at the next slot with probability p until success.
#figure(
  image("Images/Slotted ALOHA.png", width: 50%)
)
The advantages of this technique are that a single node can transmit at full rate and that it is simple to implement. The disadvantages are that there are frequent collisions, clients are forced to wait the beginning of a slot to transmit and clock synchronisation is needed.

Assuming that all frames have the same size, that there are infinite stations, and that each frame is transmitted independently from the others, the probability of k frames being sent at a certain instant is poisson distributed, with probability mass function
$ P[k]=(lambda^k e^(-lambda))/(k!) $where $lambda$ is the average number of frames sent by all hosts in a defined time interval. Therefore, the probability of having just one frame being transmitted in a slot is $P[k=1] = lambda e^(-lambda)$. This probability is maximised when $lambda$ is 1. Plugging this value in the PMF, we obtain $P[k]=1/(e k!)$. Therefore the probability of having no collisions is $P[k=1] = 1/e approx 0.368$. Since this probability tells us on average how many frames will be actually sent successfully this probability is called throughput.
==== Pure ALOHA (unslotted)
In unslotted ALOHA frames again have all the same size, but nodes can transmit at any time. The collision probability increases because a frame sent at $t_0$ will collide with frames sent during $[t_0-1, t_0]$.
#figure(
  image("Images/Pure ALOHA.png", width: 50%)
)
By doing the same assumptions as before, the probability of sending just one frame at time $t_0$ is $P[k=1] = lambda e^(-lambda)$. The probability that no frames have been sent in the interval $[t_0-1, t]$ is $P[k = 0]=e^(-lambda)$. The probability that the transmission is successful is therefore $P[k=1] dot.op P[k=0]=lambda e^(-2lambda)$. This probability is maximised when $lambda=1/2$. Therefore the probability of having no collisions is $P[k=1] dot.op P[k=0]=1/(2e) approx 0.184$, which is half the probability of slotted ALOHA.
#figure(
  image("Images/Comparison ALOHA.png", width: 50%)
)
==== CSMA
In carrier sense multiple access nodes check if the channel is idle before transmitting. Various implementations of CSMA exist:
- non-persistent CSMA: if channel is busy defer the transmission by a random time much larger than the transmission time
- 1-persistent CSMA: if channel is busy wait for channel to become free and then transmit the frame
- p-persistent CSMA: if channel is busy wait for channel to become free and transmit the frame with probability p. If frame is not transmitted defer the transmission by a random time much larger than the transmission time
If a collision happens wait a random time and try again.
#figure(
  image("Images/Comparison all.png", width: 70%)
)
CSMA has the following problem: if a host starts to transmit but the signal has not yet reached the other hosts, the other hosts will start transmitting at the same time because they think that the channel is idle. This period is called the vulnerability period and is given by the propagation time $tau$ and by the time $T_a$ needed for a host to sense that the channel is busy, therefore $T_v=tau + T_a$. In general CSMA works well when the propagation time is much smaller than the transmission time. When collisions occur, the entire transmission time is wasted. Therefore, two additional versions of CSMA have been developed: CSMA/CD and CSMA/CA.

CSMA/CD deals with collision detection: if a collision is detected, the transmission is stopped immediately, reducing the time wasted. This is easy in wired LANs because it is possible to measure the signal strength on the channel and compare it to the transmitted signal strength. In wireless LANs this is much more difficult.

CSMA/CA deals instead with collision avoidance: in channels where is hard to detect collisions (such as wireless LANs and networks where $T lt.double tau + T_a$) p-persistent CSMA is used, where p is adapted to network conditions (p decreases at each retransmission attempt).
=== "Taking turns" protocols
Polling is a technique where a master node tells to slave nodes when to transmit. It it used when slaves have low computation power, but it introduces a single point of failure, polling messages generate overhead and some latency is introduces.
Token passing is a technique where the host holding the token is allowed to transmit. Whet its done transmitting the host gives the token to the next host. Again, the passing of the token generates overhead, latency and introduces a single point of failure (the token itself).
== IEEE 802 standards
In IEEE 802 standards the data link is divided in two layers: logical link control and the medium access control (MAC) of the medium. The LLC deals with multiplexing the packets it receivers from the network layer to the various mediums, such as 802.3 Ethernet or 802.11 Wireless LAN.
#figure(
  image("Images/IEEE 802.png", width: 70%)
)
=== Ethernet (IEEE 802.3)
Ethernet is the dominant wired LAN technology: it is simple and cheap and nowadays it can reach speeds of 10Gbps and more.
==== Ethernet frame
The Ethernet frame is structured in the following way:
- Preamble: sequence of predefined bits used to synchronise the sender and receiver
- Source and destination MAC address
- Type: higher level protocol
- CRC: used for error detection
#figure(
  image("Images/Ethernet frame.png", width: 70%)
)
==== Ethernet characteristics
Ethernet is connectionless (does not do handshaking) and unreliable (no acks or nacks). Ethernet uses unslotted CSMA/CD. This means that if the network card (NIC) has a frame to transmit it will check the status of the channel: if it is free, it will transmit the frame immediately, otherwise it will transmit it as soon as the channel becomes free. If a collision is detected during the transmission, the NIC terminate the transmission, send a jam signal and will choose randomly a backoff value K from the set $\{0, 1, 2,..., 2^{n-1}\}$, when n is the number of collisions. The NIC will then wait $K dot.op "512 bit"$. Therefore the time grows exponentially.
==== Ethernet switches
Initially Ethernet was designed using the bus topology: that means that all hosts were connected to the same wire. Nowadays, the star topology is used.
#figure(
  image("Images/Topology.png", width: 70%)
)
There exists two types of devices to connect multiple hosts over Ethernet using this topology: hubs and switches. If hosts are connected through a hub, the hub simply retransmits everything it receives on all the other ports, thus the collision domain is the same (as in the bus topology).

The switch is more active device: it examines the incoming frames and retransmits them only to the appropriate outgoing links. This process is transparent, which means that hosts are unaware of the presence of the switch. This means that each host is on a different collision domain, therefore, if the connections are full-duplex, no collisions are possible. The switch uses internal buffers if it is receiving too many frames.

The switch a plug-and-play device and is able to automatically learn which hosts are reachable through which interfaces and build the switch table accordingly. If the switch does not know where the destination is, it will send the frame to all interfaces except from where the frame arrived (flooding). If the destination is the same as the source, the frame will be dropped. Entries in the switching table have a TTL value, similarly to the routing table.
==== Switches vs. routers vs. hubs
Routers are network-layer devices (examine network-layer headers), switches are link-layer devices (examine link-layer headers) and hubs simply retransmit frames on all interfaces. The forwarding table of routers is assembled using routing algorithms and contains IP addresses, switches assemble their table by flooding and contain MAC addresses, while hubs have no routing table.
==== Broadcast vs collision domains
The collision domain is the portion of the network in which, if two hosts transmit simultaneously, a collision will occur. The broadcast domain is the portion of the network that can be reached by a layer-2 broadcast.
#figure(
  image("Images/Domains.png", width: 70%)
)
=== WiFi (IEEE 802.11)
A wireless network is composed of:
- Wireless hosts: devices that connect to the network, can be stationary or fixed (ex. smartphones, laptops)
- Base station: devices that are typically connected to a wired network and relay packets from the wired network and the wireless hosts in their area (ex. cell towers, 802.11 access points)
- Wireless link: connect wireless hosts and base stations to other base stations. Access to the medium is managed by MAC protocols
#figure(
  image("Images/Wireless network.png", width: 50%)
)
A wireless network can be structured in two ways:
- Infrastructure mode: base station connects hosts to the wired network, and hosts with a procedure named handoff can change base station
- Ad hoc mode: there are no base stations, but hosts communicate directly to their neighbours
#figure(
  image("Images/Wireless structure.png", width: 20%)
)
==== Reference architecture of WLANs
Usually WLANs are based on a standard architecture model. In this architecture all hosts and base stations are referred to as STA (stations). All stations sharing the same MAC protocol and accessing the same medium are referred to as BSS (Basic service set). In a BSS an access point can be present, which connects the BSS to the distribution system. The distribution system is a wired backbone LAN which connects two or more BSSs. A group of connected BSSs is referred to as ESS (Extended service set). ESS appears as a single logical LAN at the logical link control (LLC) level (ex. a WiFi network is made of multiple BSSs, but if appears at the higher level as a single network).
#figure(
  image("Images/Reference architecture.png", width: 40%)
)
==== Protocols for wireless communications
Wireless communications are regulated by the 802.11 standard, which divides the spectrum into different intervals of frequencies called channels. If two neighbouring access points use the same channel, interference may happen.

Hosts connect to access points by scanning the channels and listening for beacon frames containing the AP's name (SSID) and the MAC address. Then the hosts chooses the AP to associate with, authenticates and typically uses DHCP to obtain an IP address. There also exists an active scanning mode: the host sends a probe request using broadcast, to which the access points reply with a probe response.

There are some key differences between wireless and wired communications: in wireless communications the signal strength is attenuated as it propagates through matter, the signal propagates in different ways over different paths (ex. reflections) and there is a lot more interference from other sources (ex. motors, lights and other access points). Moreover, collision detection in wireless network is impossible to achieve, because a transmitting antenna cannot act as a receiver at the same time. Even if we had an access point with two antennas (one transmitting and one receiving), the power of the transmitted signal will be of many order of magnitude higher than any other signal received by the received antenna (note that radio waves are attenuated quadratically). Therefore, the only option to avoid collisions is to reduce the probability of interference by using CSMA/CA.
==== Collision avoidance in wireless networks
The MAC 802.11 protocol is based on CSMA with collision avoidance. The procedure goes as follows:
1. If the channel is free for a certain amount of time (DIFS - Distributed Inter-frame space) the the sender will transmit the entire frame. If the channel is busy, the sender will pick a random backoff time, wait for the channel to be free again and additionally wait for the backoff time.
2. If the receiver has received the frame successfully, it will send back an ACK after a short interval than DIFS (otherwise other frames would be transmitted before the ACK). The ACK is needed because of possible backoff collisions, channel errors (signal was to weak to reach the receiver) or hidden terminal problems. If no ACK has been received, double the backoff interval and retry sending the frame
Example:
#figure(
  image("Images/CA wireless 1.png", width: 100%)
)
#figure(
  image("Images/CA wireless 2.png", width: 100%)
)
#figure(
  image("Images/CA wireless 3.png", width: 50%)
)
==== Hidden terminal problem
The hidden terminal problem arises when two stations cannot hear each other (A and C), but a third station can hear both (B). This will cause deterministic collisions, which could be avoided by using CSMA/CA with handshaking.
#figure(
  image("Images/Hidden terminal.png", width: 70%)
)
The handshaking requires that before transmitting the station has to send a short RTS (Request to send), to which the receiver has to reply with CTS (Clear to send). Therefore, even if a station could not hear the RTS, it will hear the CTS, thus it will know that the channel is busy for a certain amount of time. If two RTSs collide there will be no CTS, but not much time has been wasted because these packets are very short.
Example:
#figure(
  image("Images/Hidden terminal collision.png", width: 70%)
)
==== 802.11 frame
In an 802.11 frame there are four MAC addresses:
- address of the sender
- address of the destination
- address of router to which the access point is connected to
- used only when access point is connected to another access point: address of second access point
#figure(
  image("Images/WiFi frame 1.png", width: 70%)
)
#figure(
  image("Images/WiFi frame 2.png", width: 60%)
)