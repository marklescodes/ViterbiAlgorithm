import matplotlib.pyplot as plt
import csv

x, y = [], []

with open("results.csv") as f:
    reader = csv.reader(f, delimiter=";")
    for row in reader:
        x.append(float(row[0]))
        y.append(float(row[1]))

plt.plot(x, y, marker='o')
plt.xlabel("Channel Error Probability")
plt.ylabel("Bit Error Rate (BER)")
plt.title("BER vs Channel Error Probability")
plt.grid(True)
plt.show()
