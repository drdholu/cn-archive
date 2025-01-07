import socket
import threading
import time
# Receiver
def selective_repeat_receiver(window_size=4):
    expected_seq_num = 0
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(('localhost', 9999))

    # To store received packets temporarily
    received_packets = {}
    cnt=0
    while True:
        packet, addr = sock.recvfrom(1024)
        received_packet = packet.decode('utf-8')

        # Check for termination signal
        if received_packet == "END":
            print("Received termination signal. Sending acknowledgment and closing receiver.")
            sock.sendto("END_ACK".encode('utf-8'), addr)
            break
        
        seq_num = int(received_packet.split(" ")[1])  # Extract packet number from received message
        print(f"Received {received_packet}")
        
        
        # Store the packet in the temporary buffer
        received_packets[seq_num] = received_packet

        # Send ACK for the received packet
        if seq_num==2 and cnt==0:
            cnt=1
            # time.sleep(5)
        else:
            sock.sendto(str(seq_num).encode('utf-8'), addr)

        # Process packets in order
        while expected_seq_num in received_packets:
            print(f"Processing {received_packets[expected_seq_num]}")
            del received_packets[expected_seq_num]  # Remove it from the buffer
            expected_seq_num += 1
    sock.close()

# To run both sender and receiver
selective_repeat_receiver()
