import matplotlib.pyplot as plt

def tcp_reno():
    cwnd = 1  # Congestion window
    ssthresh = 16
    total_packets = 30
    dup_ack_count = 0
    loss_packet = 10  # Assume packet loss at packet 10
    cwnd_over_time = []  # To store cwnd values for plotting

    for packet in range(total_packets):
        print(f"Packet: {packet + 1}, CWND: {cwnd}")
        cwnd_over_time.append(cwnd)  # Track cwnd for each packet

        if packet == loss_packet:
            print("Packet loss detected! Receiving duplicate ACKs...")
            dup_ack_count = 3
            if dup_ack_count == 3:
                print("3 Duplicate ACKs detected, entering Fast Recovery!")
                ssthresh = cwnd // 2
                cwnd = ssthresh + 3  # Fast recovery sets cwnd to ssthresh + 3
                print(f"Fast Recovery state: CWND: {cwnd}")
                dup_ack_count = 0
        else:
            if cwnd < ssthresh:
                cwnd *= 2  # Slow Start
            else:
                cwnd += 1  # Congestion Avoidance

    # Plotting the congestion window size over time
    plt.figure(figsize=(10, 6))
    plt.plot(cwnd_over_time, label="TCP Reno Congestion Window", color="green")
    plt.xlabel("Packet Number")
    plt.ylabel("Congestion Window Size (cwnd)")
    plt.title("TCP Reno Congestion Window Behavior Over Time")
    plt.legend()
    plt.grid(True)
    plt.show()

# Run the TCP Reno simulation and display the graph
tcp_reno()
