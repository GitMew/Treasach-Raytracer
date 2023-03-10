# In the interest of not committing 1000 lines, temporarily replace the below string by the output of src/examples/Test.h.
s = """0.155939 [0.368227,0.7746,0.514202]
0.128639 [-0.881046,0.453421,0.134785]
0.158618 [0.0958877,0.735561,-0.670638]
0.157081 [0.635058,0.761877,-0.12746]
0.102615 [0.935484,0.343223,-0.0840651]
0.13663 [-0.0482449,0.869731,-0.491163]"""
import matplotlib.pyplot as plt
import matplotlib.cm as cm

fig = plt.figure()
ax = fig.add_subplot(projection='3d')

points = []
lines = s.split("\n")
for line in lines:
    pdf, vector = line.split(" ")
    x,y,z = vector[1:-1].split(",")
    # Available colour maps: https://matplotlib.org/stable/tutorials/colors/colormaps.html
    ax.scatter(float(z), float(x), float(y), color=cm.gnuplot(1 - float(pdf)/0.16), marker=".")

ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')
plt.show()