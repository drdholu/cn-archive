def tcp_slow_start():
    cwnd = 1  # Start with 1 MSS (Maximum Segment Size)
    ssthresh = 16  # Some arbitrary threshold
    total_packets = 30  # Simulate for 30 packets
    lost=[6,8,23]
    for packet in range(total_packets):
        if(packet+1 in lost):
            print("lost packet",packet+1)
            ssthresh=cwnd/2
            cwnd=1
            print(f"Packet: {packet + 1}, CWND: {cwnd}")
            if cwnd < ssthresh:
                cwnd *= 2  # Exponential growth during slow start
            else:
                cwnd += 1  # Linear growth during congestion avoidance
        else:
            print(f"Packet: {packet + 1}, CWND: {cwnd}")
            if cwnd < ssthresh:
                cwnd *= 2  # Exponential growth during slow start
            else:
                cwnd += 1  # Linear growth during congestion avoidance

tcp_slow_start()
