import numpy as np
import matplotlib.pyplot as plt

# Load data from file
est_freq = np.loadtxt('est_freq.txt')
time_est = est_freq[:, 0]
freq_est = est_freq[:, 1]

real_freq = np.loadtxt('freq.txt')
time_real = real_freq[:, 0]
freq_real = real_freq[:, 1]

sum = 0
for i in range(len(freq_real)):
    if i>=5000 and i<95000:
        sum += (freq_real[i] - freq_est[i-5000])**2
sum = sum/90000
rmse = np.sqrt(sum)
print('RMSE:', rmse)