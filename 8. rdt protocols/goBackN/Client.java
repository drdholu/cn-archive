package goBackN;


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
    private static final int WINDOW_SIZE = 4;
    private static final int TIMEOUT = 1000;
    private static final int TOTAL_PACKETS = 10;

    public static void main(String[] args) throws Exception {
        System.out.println("Client server is running on PORT: " + PORT);

        DatagramSocket socket = new DatagramSocket();
        socket.setSoTimeout(TIMEOUT);

        int base = BASE_SEQUENCE_NUMBER;
        int nextSeqNum = BASE_SEQUENCE_NUMBER;
        int lastAckedSeq = BASE_SEQUENCE_NUMBER - 1;

        InetAddress IPAddress = InetAddress.getByName(HOSTNAME);

        while (lastAckedSeq < BASE_SEQUENCE_NUMBER + TOTAL_PACKETS - 1) {
            while (nextSeqNum < base + WINDOW_SIZE && nextSeqNum < BASE_SEQUENCE_NUMBER + TOTAL_PACKETS) {
                byte[] sendData = ByteBuffer.allocate(4).putInt(nextSeqNum).array();
                DatagramPacket packet = new DatagramPacket(sendData, sendData.length, IPAddress, PORT);
                socket.send(packet);
                System.out.println("Sent: " + nextSeqNum);
                nextSeqNum++;
            }

            try {
                byte[] rcvData = new byte[BUFFER_SIZE];
                DatagramPacket rcved = new DatagramPacket(rcvData, rcvData.length);
                socket.receive(rcved);

                int ackNum = ByteBuffer.wrap(rcved.getData()).getInt();
                System.out.println("Received ACK for: " + ackNum);

                if (ackNum >= base) {
                    base = ackNum + 1;
                    lastAckedSeq = ackNum;
                }
            } catch (SocketTimeoutException e) {
                System.out.println("Timeout, resending window from: " + base);
                nextSeqNum = base;
            }
        }

        socket.close();
        System.out.println("All packets sent and acknowledged.");
    }
}
