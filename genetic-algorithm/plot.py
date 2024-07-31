import matplotlib.pyplot as plt

# open file and read data
a = open("output.txt").readlines()[:]

for i in range(len(a)):
    a[i] = int(a[i].strip("\n").split(" ")[-1])

x = list(range(len(a)))
y = a

plt.plot(x, y)
plt.xlabel("nth generation")
plt.ylabel("average score of best chromosome")
plt.show()