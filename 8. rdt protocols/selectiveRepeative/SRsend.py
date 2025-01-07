import socket
import time
import threading

# Sender
def selective_repeat_sender(window_size=4):
    server_address = ('localhost', 9999)
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # Data packets to send
    data_packets = [f"Packet {i}" for i in range(6)]  # Six packets
    base = 0
    next_seq_num = 0
    total_packets = len(data_packets)
    ack_received = [False] * total_packets  # Track which packets have been acknowledged
    sock.settimeout(5)  # Timeout set for resending packets
    
    while base < total_packets:
        # Send packets in the window
        while next_seq_num < base + window_size and next_seq_num < total_packets:
            if(ack_received[next_seq_num]==False):
                print(f"Sending {data_packets[next_seq_num]}")
                sock.sendto(data_packets[next_seq_num].encode('utf-8'), server_address)
            next_seq_num += 1
        
        # Wait for ACKs
        try:
            while True:
                ack, _ = sock.recvfrom(1024)
                ack = int(ack.decode('utf-8'))
                if not ack_received[ack]:  # Only process unacknowledged packets
                    print(f"Received ACK for {data_packets[ack]}")
                    ack_received[ack] = True  # Mark this packet as acknowledged
                
                # Slide the window
                while base < total_packets and ack_received[base]:
                    base += 1  # Move the base forward
                
                if base >= total_packets or base==next_seq_num:
                    break

        except socket.timeout:
            if not all(ack_received[base:base+window_size]):
                print("Timeout! Resending packets...")
                next_seq_num = base  # Reset next_seq_num to resend unacknowledged packets in the current window
            else:
                # No need to resend if all packets in the window are acknowledged
                print("All packets in the current window acknowledged, no need to resend.")

    # Send termination signal
    print("All packets sent. Sending termination signal.")
    sock.sendto("END".encode('utf-8'), server_address)

    # Wait for the acknowledgment of the END signal
    try:
        end_ack, _ = sock.recvfrom(1024)
        if end_ack.decode('utf-8') == "END_ACK":
            print("Termination signal acknowledged. Closing sender.")
    except socket.timeout:
        print("Timeout while waiting for END acknowledgment.")

    sock.close()
selective_repeat_sender()