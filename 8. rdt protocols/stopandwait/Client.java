package stopandwait;

import java.io.*;
import java.net.*;
import java.nio.*;
import java.util.*;
import java.util.random.*;


public class Client {
    private static final int BUFFER_SIZE = 1024;
    private static final int PORT = 3000;
    private static final String HOSTNAME = "localhost";
    private static final int BASE_SEQUENCE_NUMBER = 42;
    private static final int timeout = 1000;
    private static final int totalPackets = 5;

    public static void main(String[] args) throws Exception {
        System.out.println("Client server is running on PORT: "+PORT);

        DatagramSocket socket = new DatagramSocket();
        socket.setSoTimeout(timeout);

        Integer seqNum = BASE_SEQUENCE_NUMBER;

        for(int i = 0; i<totalPackets;  i++){
            boolean timedOut = true;

            while(timedOut){
                seqNum++;

                byte[] sendData = new byte[BUFFER_SIZE];
                byte[] rcvData = new byte[BUFFER_SIZE];

                // get server address
                InetAddress IPAddress = InetAddress.getByName(HOSTNAME);
                System.out.println("Sending: " + seqNum);
                sendData = ByteBuffer.allocate(4).putInt(seqNum).array();

                try {
                    DatagramPacket packet = new DatagramPacket(sendData, sendData.length, IPAddress, PORT);
                    socket.send(packet);

                    DatagramPacket rcved = new DatagramPacket(rcvData, rcvData.length);
                    socket.receive(rcved);

                    int returnMsg = ByteBuffer.wrap(rcved.getData()).getInt();
                    
                    if(returnMsg == seqNum) {
                        System.out.println("ACK FOR: " + returnMsg);
                        timedOut = false;
                    }
                    System.out.println();

                } catch (Exception e) {
                    System.out.println("failed");
                    seqNum--;
                }
            }
        }
    }
}
