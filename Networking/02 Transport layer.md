The transport layer provides tools to enable logical communication between processes on different hosts. The transport protocols are run exclusively on the transmitting and receiving side.
## Services offered by the transport layer
There are two main transport protocols: TCP and UDP. TCP provides the ordered delivery of segments, deals with congestion control and connection setup. UDP, on the other hand, just transmits segments with the lowest possible overhead, but without guarantees of delivery or ordering.
### Multiplexing and demultiplexing
Multiplexing and demultiplexing is the operation that allows using multiple sockets on the same host. To achieve this, the sender adds information about the source port and the destination port in the header of the segment. The receiver reads this information and directs the datagram to the right socket.
The port is a 16-bit integer, which uniquely identifies a process on the machine.
Servers usually offer services on well-known ports (static), which are often defined in standards and don't change. Conversely, clients often open connections on random ports (dynamic) assigned by the operating system. Usually server and client port numbers are never the same, otherwise if a client opens multiple processes to the same server port collision would happen (ex. client open multiple browser tabs).
A flow is a group of data belonging to the same logic communication. An application can open multiple connections and therefore manage multiple flows.
## UDP protocol
The UDP (User Datagram Protocol) is a protocol offering a "best effort service", therefore it does not guarantee that the USP segments will reach the destination or that they will arrive in order. UDP is connectionless, that means that there is no handshaking between sender and receiver, so each segment is independent from the others. If we want to, we can add reliability to the protocol at the application layer.
Advantages of UDP: less delay because there is no connection establishment, no need to maintain state of communication, smaller header size, no limit on the amount of data sent because there is no congestion control.
### Error control
In the header of the UDP segment there is a checksum field. The sender splits the data (including header) in 16 bit integers, sums them and flips all the bits (ones' complement). The receiver just sums the data and adds that sum to the checksum. If the checksum is correct, the final result should be all ones, otherwise there has been an error in transmission. Checksum field is optional in IPv4, required in IPv6.
## Reliable data transfer
There exists a class of protocols which deal with packet losses called ARQ (Automatic Repeat reQuest). These protocols use a special packet called ACK (Acknowledgement) to notify the transmitter of the reception.
### Stop-and-wait protocol
For this protocol the transmitter sends a PDU and keeps a copy and waits to receive the ACK. If it doesn't receive the ACK before the timer runs out it resends the PDU. When an ACK is received it checks the sequence number and the checksum of the ACK and if correct sends the next PDU.
On the other side when the receiver gets a PDU it checks the sequence number and the checksum. If they are correct it sends back an ACK and forwards the SDU to the higher level, otherwise it drops the PDU.
#### Efficiency of the protocol
Assuming R = 1Gbps link, 15ms propagation delay (RTT 30ms), L = 8000 bit packet, then the transmission delay (time to send 8000 bits) is:
$$ d_{trans} = L/R = 8000bit/10^9bps = 8\micro s $$
This is the speed at which the router can push data onto the wire, but it's not the only factor involved in computing the delay. We must also consider the propagation delay (time the data needs to travel through the wire) and the processing delay at the other end to create the ACK packet.
Therefore the final perceived throughput is:
$$ \frac{L}{d_{trans}+RTT} = \frac{8000bit}{8\micro s+30ms}=33kB/s $$
(we need to add to the link speed the propagation delay to get the ACK back)
The efficiency of the link is:
$$ \frac{d_{trans}}{d_{trans}+RTT}=\frac{8\micro s}{30.008ms}=0.00027=0.027\% $$
We can see from the computations that this method is extremely inefficient!

![[Stop-And-Wait.png]]
### Pipelined protocols
In pipelined protocols the sender sends multiple packets at once and keeps track of their sequence number. Using pipelining we can increase the throughput of the Stop-And-Wait protocol. For example, if we send three packets at once, we triple the throughput.
$$ \frac{3L}{d_{trans}+RTT} = \frac{24000bit}{8\micro s+30ms}=100kB/s $$
![[Pipelined protocols.png]]
Note that the time start when the first packet is transmitted and ends when the first acknowledgment arrives.
#### Terminology of pipelined protocols
Transmission window $W_T$: set of PDUs that the transmitter is allowed to transmit without receiving an acknowledgement. The maximum dimension of the window is limited by the allocated memory at the transmitter side and is denoted by $|W_T|$.
Receive window $W_R$: set of PDUs that the receiver is capable to accept and memorise. The maximum dimension of the window is limited by the allocated memory at the receiver side.
Low pointer $W_{LOW}$: pointer to the first packet in the transmission window $W_T$
Up pointer $W_{UP}$: pointer to the last packet that has already been transmitted
![[Pasted image 20231107172636.png]]