def tcp_aimd():
    cwnd = 1  # Congestion window
    ssthresh = 16
    total_packets = 30  # Simulate for 30 packets
    acked_packets = 0
    
    for packet in range(total_packets):
        print(f"Packet: {packet + 1}, CWND: {cwnd}")
        acked_packets += 1
        
        if acked_packets == 3:  # Simulate packet loss on the 4th ACK
            print("Packet loss detected!")
            ssthresh = cwnd // 2  # Halve the ssthresh
            cwnd = 1  # Reset the congestion window to 1
            acked_packets = 0
        else:
            if cwnd < ssthresh:
                cwnd *= 2  # Exponential growth during slow start
            else:
                cwnd += 1  # Linear growth in congestion avoidance

tcp_aimd()
