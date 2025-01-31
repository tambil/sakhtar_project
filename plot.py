import numpy as np
import matplotlib.pyplot as plt

# Load data from file
data = np.loadtxt('signal.txt')
time = data[:, 0]
signal = data[:, 1]

# Plot the signal
plt.figure(figsize=(10, 6))
plt.plot(time, signal, label='Signal')
plt.xlabel('Time (s)')
plt.ylabel('DC unit')
plt.title('Signal Generation')
plt.legend()
plt.grid(True)
plt.show()