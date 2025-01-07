def calculate_fragments(total_packet_size, mtu, header_size=20):
    # Check if fragmentation is needed
    if total_packet_size <= mtu:
        print("No fragmentation needed.")
        return [{
            'Fragment Number': 1,
            'Offset': 0,
            'Data Size': total_packet_size - header_size,
            'Header Size': header_size,
            'Total Fragment Size': total_packet_size
        }]

    # Data size (excluding header)
    data_size = total_packet_size - header_size

    # Size of each fragment's data portion
    fragment_data_size = mtu - header_size

    # Ensure fragment data size is a multiple of 8
    fragment_data_size = (fragment_data_size // 8) * 8

    # Calculate the number of fragments
    num_fragments = (data_size + fragment_data_size - 1) // fragment_data_size

    fragments = []
    offset = 0

    for i in range(num_fragments):
        # Last fragment may have less data
        if i == num_fragments - 1:
            fragment_size = data_size - offset
            more_fragments_flag = 0  # Last fragment
        else:
            fragment_size = fragment_data_size
            more_fragments_flag = 1  # More fragments to follow

        # Fragment details
        fragment = {
            'Fragment Number': i + 1,
            'Offset': offset // 8,
            'Data Size': fragment_size,
            'Header Size': header_size,
            'Total Fragment Size': fragment_size + header_size,
            'More Fragments': more_fragments_flag
        }

        fragments.append(fragment)
        offset += fragment_size

    return fragments

# Example usage

total_packet_size = int(input("ENTER TOTAL PACKET SIZE: "))  # Size of the IP packet (header + data)
mtu = int(input("ENTER MTU: "))  # Maximum Transmission Unit (MTU)
header_size = 20  # IP header size (standard size without options)

fragments = calculate_fragments(total_packet_size, mtu, header_size)

for fragment in fragments:
    print(f"Fragment {fragment['Fragment Number']}:")
    print(f"  Offset: {fragment['Offset']}")
    print(f"  Data Size: {fragment['Data Size']}")
    print(f"  Header Size: {fragment['Header Size']}")
    print(f"  Total Fragment Size: {fragment['Total Fragment Size']}")
    print(f"  More Fragments Flag: {fragment['More Fragments']}")
    print()
