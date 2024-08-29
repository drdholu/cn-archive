import java.util.Scanner;

public class SubnetMaskCalculator {

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.print("Enter subnet mask in CIDR notation (24-30): /");
        int CIDR = scanner.nextInt();
        scanner.nextLine();
        System.out.print("Enter valid Class C IP address: ");
        String ipAddress = scanner.nextLine();  

        if (CIDR < 24 || CIDR > 30) {
            System.out.println("Invalid subnet mask. Must be between 24 and 30.");
            scanner.close();
            return;
        }

        int numberOfSubnets = (int) Math.pow(2, CIDR - 24);
        int numberOfIpAddresses = (int) Math.pow(2,32 - CIDR);
        int numberOfHostsPerSubnet = (int) Math.pow(2, 32 - CIDR) - 2;

        StringBuilder binSubnetMask = new StringBuilder("00000000.00000000.00000000.00000000");
        int x = 0;
        int n = CIDR;
        while (n > 0) {
            if (binSubnetMask.charAt(x) == '.') {
                x++;
                continue;
            }
            binSubnetMask.setCharAt(x, '1');
            x++;
            n--;
        }
        System.out.println("Binary Subnet Mask: " + binSubnetMask.toString());
        String binSubnetMaskStr = binSubnetMask.toString().replace(".", "");

        StringBuilder subnetMask = new StringBuilder();
        int t1 = 0;
        int t2 = 7;
        for (int j = 0; j < 4; j++) {
            String temp = binSubnetMaskStr.substring(t1, t2 + 1);
            subnetMask.append(Integer.parseInt(temp, 2));
            if (j < 3) subnetMask.append(".");
            t1 = t2 + 1;
            t2 = t2 + 8;
        }

        System.out.println("Subnet mask in decimal: " + subnetMask.toString());
        System.out.println("Number of subnets: " + numberOfSubnets);
        System.out.println("Number of hosts per subnet: " + numberOfHostsPerSubnet);
        System.out.println("Number of IP Addresses: " + numberOfIpAddresses);

        String[] ipParts = ipAddress.split("\\.");
        int[] ip = new int[4];
        for (int i = 0; i < 4; i++) {
            ip[i] = Integer.parseInt(ipParts[i]);
        }

        int subnetIncrement = 256 / numberOfSubnets;
        for (int i = 0; i < numberOfSubnets; i++) {
            int subnetNetworkId = i * subnetIncrement;
            int subnetBroadcastId = subnetNetworkId + subnetIncrement - 1;

            System.out.println("Subnet " + (i + 1) + ":");
            System.out.println("  Network ID: " + ip[0] + "." + ip[1] + "." + ip[2] + "." + subnetNetworkId);
            System.out.println("  Broadcast Address: " + ip[0] + "." + ip[1] + "." + ip[2] + "." + subnetBroadcastId);
            System.out.println("  Usable Host Address Range: " + ip[0] + "." + ip[1] + "." + ip[2] + "." + (subnetNetworkId + 1) + " - " + ip[0] + "." + ip[1] + "." + ip[2] + "." + (subnetBroadcastId - 1));
        }

        scanner.close();
    }
}