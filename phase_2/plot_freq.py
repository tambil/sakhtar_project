import numpy as np
import matplotlib.pyplot as plt

# Load data from file
real_file = np.loadtxt('phase_2/real_freq.txt')
est_file = np.loadtxt('phase_2/est_freq.txt')
real_freq_data = real_file
est_freq_data = est_file
sample = list(range(0, len(real_freq_data),1))


# Plot the signal
plt.figure(figsize=(10, 6))
plt.plot(sample, 2*real_freq_data - real_freq_data[0],color='r' ,  label='real')
plt.plot(sample, est_freq_data,color='g' ,  label='estimate')
plt.xlabel('sample')
plt.ylabel('frequency(Hz)')
plt.title('frequency')
plt.legend() 
plt.grid(True)
plt.show()