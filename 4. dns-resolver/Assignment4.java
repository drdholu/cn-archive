import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.Scanner;


public class Assignment4 {
    public static void main(String[] args) {

        Scanner scanner = new Scanner(System.in);

        System.out.print("Enter domain name to resolve: ");
        String hostname = scanner.nextLine();

        // String hostname = args[0];

        try {
            InetAddress inetAddress = InetAddress.getByName(hostname);
            String ipAddress = inetAddress.getHostAddress();
            System.out.println("The IP address for '" + hostname + "' is: " + ipAddress);
        } catch (UnknownHostException e) {
            System.out.println("Error: Could not resolve the hostname '" + hostname + "'");
        }
    }
}