import matplotlib.pyplot as plt

def tcp_tahoe():
    cwnd = 1  # Congestion window
    ssthresh = 16
    total_packets = 30
    timeout_threshold = 10  # Assume packet loss at the 10th packet
    cwnd_over_time = []  # To store cwnd values for plotting

    for packet in range(total_packets):
        print(f"Packet: {packet + 1}, CWND: {cwnd}")
        cwnd_over_time.append(cwnd)  # Track cwnd for each packet

        if packet+1 == timeout_threshold:
            print("Timeout detected, switching to Slow Start!")
            ssthresh = cwnd // 2
            cwnd = 1  # Reset CWND to 1 (Slow Start restart)
            print(f"Packet: {packet + 1}, CWND: {cwnd}")
            cwnd*=2
        else:
            if cwnd < ssthresh:
                cwnd *= 2  # Exponential growth in Slow Start
            else:
                cwnd += 1  # Linear growth in Congestion Avoidance

    plt.figure(figsize=(10, 6))
    plt.plot(cwnd_over_time, label="TCP Tahoe Congestion Window", color="blue")
    plt.xlabel("Packet Number")
    plt.ylabel("Congestion Window Size (cwnd)")
    plt.title("TCP Tahoe Congestion Window Behavior Over Time")
    plt.legend()
    plt.grid(True)
    plt.show()

# Run the TCP Tahoe simulation and display the graph
tcp_tahoe()
