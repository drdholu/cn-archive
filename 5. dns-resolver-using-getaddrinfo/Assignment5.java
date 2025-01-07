import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.Scanner;

public class Assignment5 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        // Prompt the user to enter a domain name
        System.out.print("Enter domain name to resolve: ");
        String domainName = scanner.nextLine();

        try {
            // Use getAllByName to resolve the domain to all associated IP addresses
            InetAddress[] inetAddresses = InetAddress.getAllByName(domainName);

            // Print the IP addresses
            System.out.println("IP addresses for domain '" + domainName + "':");
            for (InetAddress inetAddress : inetAddresses) {
                System.out.println(inetAddress.getHostAddress());
            }
        } catch (UnknownHostException e) {
            // Handle the case where the domain name cannot be resolved
            System.out.println("Error: Could not resolve the domain name '" + domainName + "'");
        } finally {
            scanner.close();
        }
    }
}