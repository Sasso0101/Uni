## Structure of the internet
Internet is made of hosts (devices that connect to the network) - network edge and ISPs - network core, which connect to each other and provide connectivity. ISPs are structured in a hierarchy:
1. Level 1 ISPs: only a handful, have global coverage, are tightly interconnected
2. Level 2 ISPs: have national coverage and connect to level 1 ISPs to provide global coverage
3. Level 3 ISPs: networks to which clients connect to (Access ISPs)
## Packet transmission
### Packet delay
There are 4 main reasons for packet delay:
1. Delay in processing at node: node requires time to verify the correctness of data and to determine the output link
2. Queuing delay: outgoing packets are put in a queue and have to wait their turn to be sent
3. Transmission delay: depends on size of transmission in bytes (L) and transmission speed in bit/s (R) => $t=L/R$
4. Propagation delay: depends on length of medium (d) and propagation time of medium (s) => $t=d/s$
Total delay:
$$ d_{nodal}=d_{proc}+d_{queue}+d_{trans}+d_{prop} $$
### Packet loss
Packets before being sent are put in a queue (buffer), which has a finite capacity. If the queue is full the packets get dropped. They can be then retransmitted by the sending node.
### Throughput
Speed at which data is transmitted between sender and receiver (bits/time unit). Can be expressed as instantaneous (rate at a given point in time) or average over a period of time.
## Protocols
Protocols define format and order of messages exchanged by the entities. More specifically they define the syntax of the messages (ex. their fields, order of bits), semantics (meaning of the messages), behaviour (ex. what to do if I lose a packet), temporisation (timing of acknowledgment).
Multiple protocols are grouped in so called *stacks*, defining a physical and logical communication architecture. There exist two main protocol stacks: the ISO/OSI stack and the TCP/IP stack. The ISO/OSI stack is just a reference model defining some fundamentals principles out of which real world models can be crafted. The TCP/IP stack is used in real world communication networks.
![[Protocol stacks.png | 300]]
### ISO/OSI structure
1. Physical: takes care of transmitting raw bits on the physical medium (electrical, electro-magnetic, light, sound)
2. Link: handles single hop communication and deals with error discovery and correction
3. Network: delivers packets from one host to another (deals with routing and congestion control)
4. Transport: performs application multiplexing/demultiplexing, segmentation/reassembling of data and deals with quality of service problems
5. Session: setups and maintains a communication between entities, therefore masks network interruptions from the transport layer
6. Presentation: data representation and encoding, encryption
7. Application: provides to applications the means to exchange data
### Layering
Layering protocols this way brings many advantages, namely:
- Change of one layer independent from other layers
- Clearly defines what each component of the system is responsible for, therefore simplifying the overall analysis of the system, maintenance and updating
Each layer provides services to the layer above and sees the lower layer as a black box out of which it gets services from.
Communication between layers is provided through a programming interface called SAP (Service access point). On the other hand exchange of information between same layers is defined by protocols.
![[SAP and protocols.png]]
#### Data units
Because of layering, we have to define how data travels through these layers. In a system with M layers, user data is packed in a M-SDU (Service data unit). Then layer M adds a M-CPI (Protocol control information). The final result is a M-PDU (Protocol data unit). The lower layer takes the M-PDU as a closed envelope and uses it as the (M-1)-SDU. On the receiving side the process is inverted, until we get the original data.
![[PDU structure.png]]
Data units can be
- segmented if data cannot be fit in a single SDU
- assembled to avoid inefficiencies, so multiple SDUs are grouped into one
- reassembled (opposite of segmentation)
Name of PDUs of the various layers:
- Application: messages
- Transport: segments
- Network: packets or datagrams
- Data link: frame