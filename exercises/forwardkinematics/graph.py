import matplotlib.pyplot as plt

# Sample x and y values
x_values = [1, 2, 3, 4, 5]
y_values = [2, 4, 6, 8, 10]

# Create the plot
plt.figure(figsize=(8,6))
plt.plot(x_values, y_values, marker='o')

# Add labels and title
plt.xlabel('X values')
plt.ylabel('Y values')
plt.title('Plot of X vs Y')

# Show the plot
plt.grid(True)
plt.show()
