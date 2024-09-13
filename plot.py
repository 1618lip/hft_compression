import pandas as pd
import matplotlib.pyplot as plt

# Load the original and important tick data CSV files
original_data = pd.read_csv('tick_data.csv')
important_data = pd.read_csv('important_tick_data.csv')

# Create a figure and four subplots (2 rows, 2 columns)
fig, axs = plt.subplots(2, 2, figsize=(24, 16))

# Subplot 1: Original Price
axs[0, 0].plot(original_data['Timestamp'], original_data['Price'], label='Original Price', color='blue', alpha=0.5)
axs[0, 0].set_title('Original Price')
axs[0, 0].set_xlabel('Timestamp')
axs[0, 0].set_ylabel('Price')
axs[0, 0].tick_params(axis='x', rotation=45)
axs[0, 0].grid(True)

# Subplot 2: Filtered Price
axs[0, 1].plot(important_data['Timestamp'], important_data['Price'], label='Filtered Price', color='red', linewidth=2)
axs[0, 1].set_title('Filtered Price')
axs[0, 1].set_xlabel('Timestamp')
axs[0, 1].set_ylabel('Price')
axs[0, 1].tick_params(axis='x', rotation=45)
axs[0, 1].grid(True)

# Subplot 3: Original Volume
axs[1, 0].plot(original_data['Timestamp'], original_data['Volume'], label='Original Volume', color='green', alpha=0.5)
axs[1, 0].set_title('Original Volume')
axs[1, 0].set_xlabel('Timestamp')
axs[1, 0].set_ylabel('Volume')
axs[1, 0].tick_params(axis='x', rotation=45)
axs[1, 0].grid(True)

# Subplot 4: Filtered Volume
axs[1, 1].plot(important_data['Timestamp'], important_data['Volume'], label='Filtered Volume', color='orange', linewidth=2)
axs[1, 1].set_title('Filtered Volume')
axs[1, 1].set_xlabel('Timestamp')
axs[1, 1].set_ylabel('Volume')
axs[1, 1].tick_params(axis='x', rotation=45)
axs[1, 1].grid(True)

# Adjust layout to prevent overlap
plt.tight_layout()

# Save the plot as an image
plt.savefig('price_volume_comparison_side_by_side.png')

# Show the plot
plt.show()




