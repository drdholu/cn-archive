package goBackN;


import java.io.*;
import java.net.*;
import java.nio.*;
import java.util.*;
import java.util.random.*;


/*
 * initialization of server
 *  create a datagram socket
 *  set up byte arrays for receiving and sending data
 * 
 * main loop of the server
 *  receives packet and extract message from it
 *  simulate the randomness of sending a packet (simulating a packet loss)
 * 
 * 
 */

public class Server {
    private static final int BUFFER_SIZE = 1024;
    private static final int PORT = 3000;
    private static final int WINDOW_SIZE = 4;

    public static void main(String[] args) throws IOException {
        DatagramSocket serverSocket = new DatagramSocket(PORT);
        byte[] rcvData = new byte[BUFFER_SIZE];
        byte[] dataToSend = new byte[BUFFER_SIZE];
        System.out.println("Server is active on PORT: " + PORT);

        int expectedSeqNum = 0;
        int lastAckSent = -1;


        while(true){
            DatagramPacket rcved = new DatagramPacket(rcvData, BUFFER_SIZE);
            serverSocket.receive(rcved);
            int seqNum = ByteBuffer.wrap(rcved.getData()).getInt();

            Random random = new Random();
            int chance = random.nextInt(100);

            if (chance % 2 == 0) {
                if (seqNum == expectedSeqNum) {
                    System.out.println("from client: " + seqNum);
                    lastAckSent = expectedSeqNum;
                    expectedSeqNum = (expectedSeqNum + 1) % WINDOW_SIZE;
                } else {
                    System.out.println("out of order packet: " + seqNum);
                }

                InetAddress IPAddress = rcved.getAddress();
                int port = rcved.getPort();
                
                // Send ACK for the last correctly received packet
                dataToSend = ByteBuffer.allocate(4).putInt(lastAckSent).array();

                // Create the packet to send
                DatagramPacket ackPacket = new DatagramPacket(dataToSend, dataToSend.length, IPAddress, port);
                serverSocket.send(ackPacket);
                System.out.println("ACK sent for packet: " + lastAckSent);
                System.out.println();
            } else {
                System.out.println("dropped: " + seqNum);
            }
        }
    }
}
