from collections import deque
import matplotlib.pyplot as plt

def bfs_with_path(grid, start, end):
    rows, cols = len(grid), len(grid[0])
    directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]
    queue = deque([(start[0], start[1], [])])  # (x, y, path)
    visited = set()
    visited.add((start[0], start[1]))

    while queue:
        x, y, path = queue.popleft()

        # If we reach the end, return the path
        if (x, y) == end:
            return path + [(x, y)]

        for dx, dy in directions:
            nx, ny = x + dx, y + dy

            if 0 <= nx < rows and 0 <= ny < cols and grid[nx][ny] == 0 and (nx, ny) not in visited:
                visited.add((nx, ny))
                queue.append((nx, ny, path + [(x, y)]))

    # If no path is found
    return []

def visualize_grid_with_path(grid, path, waypoints):
    rows, cols = len(grid), len(grid[0])
    plt.figure(figsize=(10, 5))

    # Plot the grid
    for x in range(rows):
        for y in range(cols):
            if grid[x][y] == 1:
                plt.scatter(x, y, color='black', s=100)  # Walls
            else:
                plt.scatter(x, y, color='white', s=100, edgecolor='black')  # Open spaces

    # Plot the path
    if path:
        path_x, path_y = zip(*path)
        plt.plot(path_x, path_y, color='red', linewidth=2, label='Path')

    # Plot the waypoints
    if waypoints:
        waypoint_x, waypoint_y = zip(*waypoints)
        plt.scatter(waypoint_x, waypoint_y, color='blue', s=100, label='Waypoints')

    plt.legend()
    plt.title("Grid with Path")
    plt.gca().invert_yaxis()  # Flip the y-axis so (0,0) is at the bottom-left
    plt.gca().set_aspect('equal', adjustable='box')
    plt.show()

if __name__ == '__main__':
    # Define the grid (all open spaces for simplicity)
    grid = [
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    ]

    start = (0, 10)
    end = (21, 11)

    # Define the waypoints
    waypoints = [(2, 2), (11, 2), (21, 2), (21, 8), end]
    curr = start
    full_path = []

    for waypoint in waypoints:
        segment_path = bfs_with_path(grid, curr, waypoint)
        full_path.extend(segment_path[:-1])  # Avoid duplicating the endpoint
        curr = waypoint

    full_path.append(end)

    # Visualize the grid with path and waypoints
    visualize_grid_with_path(grid, full_path, waypoints)
