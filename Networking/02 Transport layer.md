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
![[UDP structure.png|300]]
### Error control
In the header of the UDP segment there is a checksum field. The sender splits the data (including header) in 16 bit integers, sums them and flips all the bits (ones' complement). The receiver just sums the data and adds that sum to the checksum. If the checksum is correct, the final result should be all ones, otherwise there has been an error in transmission. Checksum field is optional in IPv4, required in IPv6.
## Reliable data transfer
There exists a class of protocols which deal with packet losses called ARQ (Automatic Repeat reQuest). These protocols use a special packet called ACK (Acknowledgement) to notify the transmitter of the reception.
### Stop-and-wait protocol
For this protocol the transmitter sends a PDU and keeps a copy and waits to receive the ACK. If it doesn't receive the ACK before the timer runs out it resends the PDU. When an ACK is received it checks the sequence number and the checksum of the ACK and if correct sends the next PDU.
On the other side when the receiver gets a PDU it checks the sequence number and the checksum. If they are correct it sends back an ACK and forwards the SDU to the higher level, otherwise it drops the PDU.
![[Stop-And-Wait 1.png]]
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

![[Images/Stop-And-Wait.png]]
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
![[Transmission and receive windows.png]]
#### Acknowledgement packets
There actually exist multiple types of acknowledgment packets:
- Individual ACK: indicate the reception of a specific packet; $ACK(n)$ means that packet n has been received
- Cumulative ACK: indicates the correct reception up to a certain packet; $ACK(n)$ means that all packets up to n (non included) have been received
- Negative ACK: requests the transmission of a single packet; $NACK(n)$ means send packet n again
With the *piggybacking* technique we can send an acknowledgment inside a normal packet.
#### Go-back-N protocol
In this protocol the sender can have up to N unacked packets in the pipeline. The receiver uses cumulative ACK to signal that it has received the packets. If it detects a gap in the packets for every received packet it will retransmit the last ACK sent. That means that it doesn't accept any more packets until the missing packet is received. The sender keeps a timer starting from the oldest unacked packet. When this timer expires, it retransmits all packets from the expired one. This timer is reset every time an ACK is received. Because all packets get retransmitted, even those that may have been actually received after the missing one, there is an intrinsic inefficiency with this approach.
![[Go-Back-N.png]]
Note: if the sequence numbers are encoded using k bits, the maximum sender window size has to be $|W_T|\le2^k-1$, because otherwise there could be multiple packets with the same sequence number in the same window.
#### Selective repeat
As in Go-Back-N, the sender can have up to N unacked packets in the pipeline, but this time the receiver will send individual ACKs. The sender keeps a timer for every unacked packet and will retransmit that packet when the timer expires. The receiver will keep the packet in a buffer until all packets before that one have been received.
![[Selective repeat.png]]
In *selective repeat* the maximum window sizes has to be $|W_T|+|W_R|\le2^k$, so that the receiving window size doesn't wrap around past the sending window size and risk having two packets with the same number in the same window.
![[Window cycle.png|300]]
## TCP protocol
TCP is a connection-oriented (has handshaking), point-to-point protocol that allows reliable, in-order segment transmission. TCP uses pipelining, where the size of the sender and receiver windows are set by flow control (sender should not overwhelm receiver) and congestion control (network should not get clogged) algorithms. Data flow is bidirectional (full-duplex) and the protocol is connection-oriented, because there is handshaking.
### TCP segment structure
![[TCP packet structure.png]]
The number in bytes that the receiver is capable of buffering (receive window) is specified in the RWND (receiver window) field. Assuming that the sender can transmit very quickly, it will have to stop when it sends as many bytes as specified by RWND. Since it is a 16-bit field the maximum data is $2^{16}=64kB$, but we can specify a scaling factor so we can have larger windows.
The field *Sequence number* indicates the number of the first byte of that segment in that flow of data, while *Acknowledge number* indicates the sequence number of the next byte that we are expecting to receive from the other end, noting that TCP uses cumulative acknowledgments.
![[Seq and Ack numbers.png|300]]
### TCP connection setup
TCP connection is setup using a mechanism called 3-way handshake:
1. Host A initialises the connection sending a segment with the SYN flag set, source port set to A, destination port set to B and initial sequential number x
2. Host B replies with a segment with SYN and ACK flags set, source port B and destination port B, sequential number y and ACK x+1
3. Host A sends an ACK segment with source port A, destination port B and ACK y+1
![[TCP setup.png| 300]]
### TCP connection teardown
Since TCP communication is bidirectional, teardown of the connection is required in both directions.
#### Gentle teardown
The host that wants to terminate the connection sends a segment with the FIN flag set and the receiver replies with ACK. Now the connection is half closed, because the first host cannot send anything anymore, but communication in the other direction is still allowed. To terminate the connection in the other direction also the other host must send a segment with the FIN flag set.
#### Rude teardown
This method is used for connections that are in an error state (ex. an ACK gets received on a non-existing connection). To terminate the connection the host sends a segment with the RST flag set. Then both hosts can free the resources needed by the connection.
![[TCP teardown.png]]
### Maximum segment size (MSS)
Although TCP works with bytes, it doesn't send single bytes, but rather tries to send segments as big as MSS. MSS depends a parameter from the lower level called MTU (Maximum Transfer Unit), which in turn relies on the MTU of the Data link layer. To determine the size TCP proceeds by trial and error, until a segment gets lost. By default the MSS is set to 1460 bytes (ethernet has a 1500 bytes MTU - 40 bytes for the headers).
### Estimating the round trip time (RTT) and choosing timeouts
Choosing the retransmission timeout (RTO) is complicated because if it is too short, then unnecessary retransmissions might happen, while if it is too long TCP will have a slow reaction time to losses. For sure the timeout time before a packet has to be retransmitted again must be grater that the round trip time.
Also RTT estimation must be done carefully. In principle RTT is the time from when the packet is sent to when the ACK is received (ignoring retransmissions). Since this value is changing constantly, an average or among several transmissions might give us a better estimate. To compute it we use this formula:
$$ SRTT = (1-\alpha)\cdot SRTT + \alpha \cdot RTT $$
Where SRTT is the "smoothed" round trip time. This is an exponential weighted moving average, where influence of past samples decreases exponentially fast. A value of $\alpha = 1/8$ is typically used.
The retransmission timeout is then given by SRTT plus a safety margin. To decide the safety margin we can compute the deviation of the actual RTT from the SRTT in the following way:
$$ RTTVAR=(1-\beta)\cdot RTTVAR + \beta \cdot |SRTT-RTT| $$
Where RTTVAR is an exponentially weighted moving average computed over the difference between SRTT and RTT. Usually $\beta=1/4$. Then:
$$ RTO= SRTT+4\cdot RTTVAR$$
The standard defines that these values are initialised to: SRTT=RTT of first packet, RTTVAR = RTT/2, RTO = 1s.
### TCP flow control
Flow control is a functionality of the TCP protocol that allows the receiver to control the seed of transmission of the sender. To do this, the receiver indicates in the RWND field of every segment the size of its receive window. Then, the sender sets its sender window to RWND. This guarantees that the receiver will not overflow its buffer.
### TCP congestion control
Congestion happens when too many receiver are sending too much data and the network is not able to handle that traffic. In a congested network packets get dropped because the buffers in routers overflow and there are high delays because there are long queues in the router buffers.
Congestion control is a very complicated problem and many algorithms have been proposed, such as TCP Tahoe, Reno, New Reno, Vegas... Because of this, there does not exist a single TCP implementation, but many of them.
There are two possible approaches to this problem:
- End-to-end: congestion is estimated by looking at packet losses and delays
- Network-assisted: routers provide feedback on congestion of the network
#### AIMD (Additive Increase Multiplicative Decrease)
In this alogrithm the sender progressively increases the transmission rate by increasing the window size until a loss occurs. When a loss occurs the window gets shrunk.
The protocol has two phases:
- Additive increase: the window size gets increased by 1 MSS every RTT until a loss is detected (NOTE: it increases the window size every RTT, not when an ACK is received!)
- Multiplicative decrease: after a loss, the window size gets halved
![[Additive increase multiplicative decrease.png|300]]
The AIMD protocol ensures fairness in the network. That means that if K connections share the same link with bottleneck R, each connection has a bandwidth of R/K.
##### Implementation in TCP
Congestion window (CWND): number of bytes that the transmitter is allowed to inject into the network.
The sender window is therefore given by:
$$ |W_T|=min(CWND,RWND)=min(CWND,|W_R|) $$
TCP adapt the size of CWND by implementing the AIMD algorithm using two phases to reach quickly the optimal point:
- Slow start: for every ACK received by the transmitter, the window size gets increased by 1. This induces an exponential growth (fucking compound interest goes brrrr). When CWND reaches SSTHRESH (slow start threshold), TCP switches to congestion avoidance
- Congestion avoidance: for every ACK received by transmitter, CWND increased by $MSS\cdot\frac{MSS}{CWND}$. That means that if all packets in a window get ACKed, then the window size increases by one MSS. This induces a linear increase in time.
At the beginning of the transmission CWND gets initialised to 1 and SSTHRESH to RWND. When a timeout happens the algorithm always switches to slow start, retransmits the unACKed packet, SSTHRESH gets set to $max(\frac{CWND}{2}, 2)$, RTO gets doubled and CWND gets set to 1.
![[Slow start congestion avoidance.png]]
This method is very inefficient and does not consider the number of duplicated acknowledgements we get, which provide information about the working of the network (remember that TCP uses cumulative acknowledgements).
In the fast retransmit and fast recovery algorithm after three duplicated acknowledgements are received the sender retransmits the segment indicated by the duplicated acknowledgements (fast retransmit) and enters fast recovery mode. SSTHRESH gets set to $\frac{CWND}{2}$, $W_{LOW}$ still points to the first unACKed segment and CWND gets set to SSTHRESH + 3 MSS. After the sender receives another duplicated acknowledgement CWND gets increased by 1 MSS. After a valid ACK has been received (this means that the lost segment has been successfully received) CWND gets set to SSTHRESH and the sender exits fast recovery and enters congestion avoidance.
![[Fast retransmit fast recovery.png]]
### TCP fairness
The TCP protocol does not solve all fairness problems:
- TCP connections may limit their bandwidth if other UDP connections, which do not have congestion control, are using the link at the same time
- Applications can open multiple TCP connections in parallel