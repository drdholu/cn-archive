import heapq

class RouterOSPF:
    def __init__(self, name, neighbors):
        self.name = name
        self.neighbors = neighbors  # Neighbors and their link weights
        self.network_map = {}  # Stores the complete network topology

    def update_network_map(self, network_map):
        self.network_map = network_map

    def dijkstra(self):
        distances = {router: float('inf') for router in self.network_map}
        distances[self.name] = 0
        pq = [(0, self.name)]  # Priority queue for Dijkstra's algorithm

        while pq:
            current_dist, current_router = heapq.heappop(pq)

            if current_dist > distances[current_router]:
                continue

            for neighbor, weight in self.network_map[current_router].items():
                distance = current_dist + weight
                if distance < distances[neighbor]:
                    distances[neighbor] = distance
                    heapq.heappush(pq, (distance, neighbor))

        return distances

    def display_shortest_paths(self):
        distances = self.dijkstra()
        print(f"Shortest paths from {self.name}: {distances}")


def simulate_ospf(routers, network_map):
    for router in routers.values():
        router.update_network_map(network_map)

    for router in routers.values():
        router.display_shortest_paths()


# Example network topology for OSPF
network_map = {
    'A': {'B': 2, 'D': 5},
    'B': {'A': 2, 'E': 4},
    'C': {'B': 5, 'F': 4, 'G':3},
    'D': {'A': 3, 'E': 5},
    'E': {'D': 5, 'B': 4, 'F':2},
    'F': {'E': 2, 'C': 4, 'G':1},
    'G': {'C': 3, 'F': 1},
}

routers_ospf = {
    'A': RouterOSPF('A', {'B': 1, 'C': 5}),
    'B': RouterOSPF('B', {'A': 1, 'C': 2}),
    'C': RouterOSPF('C', {'A': 5, 'B': 2}),
    'D': RouterOSPF('D', {'A': 3, 'E': 5}),
    'E':RouterOSPF('E', {'D': 5, 'B': 4, 'F':2}),
    'F':RouterOSPF('F', {'E': 2, 'C': 4, 'G':1}),
    'G':RouterOSPF('G', {'C': 3, 'F': 1},),
}

simulate_ospf(routers_ospf, network_map)
