package stopandwait;

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
 * server responds
 * or if server doesnt respond -> send a packet saying dropped
 */


public class Server {
    private static final int BUFFER_SIZE = 1024;
    private static final int PORT = 3000;

    public static void main(String[] args) throws IOException {
        DatagramSocket serverSocket = new DatagramSocket(PORT);
        byte[] rcvData = new byte[BUFFER_SIZE];
        byte[] dataToSend = new byte[BUFFER_SIZE];
        System.out.println("Server is active on PORT: " + PORT);


        while(true){
            DatagramPacket rcved = new DatagramPacket(rcvData, BUFFER_SIZE);
            serverSocket.receive(rcved);
            int msg = ByteBuffer.wrap(rcved.getData()).getInt();

            Random random = new Random();
            int chance = random.nextInt(100);

            if(chance % 2 == 0){
                System.out.println("from client: " + msg);
                // get ip addr and port of the received packet
                InetAddress IPAddress = rcved.getAddress();
                int port = rcved.getPort();
                dataToSend = ByteBuffer.allocate(4).putInt(msg).array();

                // create the packet to send
                DatagramPacket packet = new DatagramPacket(dataToSend, dataToSend.length, IPAddress, port);
                serverSocket.send(packet);
                System.out.println("acked: " + msg);
                System.out.println();
            }
            else{
                System.out.println("dropped: " + msg);
            }

        }
    }
}
