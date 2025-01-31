import numpy as np
import matplotlib.pyplot as plt

# Load data from file
real_file = np.loadtxt('real_freq.txt')
est_file = np.loadtxt('est_freq.txt')
real_freq_data = real_file[:, 0]
est_freq_data = est_file[:, 0]
sample = list(range(0, len(real_freq_data),1))


# Plot the signal
plt.figure(figsize=(10, 6))
plt.plot(sample, real_freq_data, label='Signal')
plt.xlabel('Time (s)')
plt.ylabel('DC unit')
plt.title('Signal Generation')
plt.legend() 
plt.grid(True)
plt.show()