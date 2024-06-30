# script for figuring out how many LEDs to put in each ring
# prints some a few options for # of LEDs, and the resulting distance and angle
# between each LED for each

import math
from numpy import linspace


def poc(radius, origin, d):
    '''get a point on the circumference of a circle
    d : degrees along the circumference of the cricle
    radius : radius of the circle
    origin : (x, y, z) center coordinates of the circle
    '''
    degrees = d + 270 if d - 90 < 0 else d - 90
    radians = (degrees * math.pi) / 180
    x = origin[0] + radius * math.cos(radians)
    y = origin[1] + radius * math.sin(radians)
    return (x, y, origin[2])


def distance(p1, p2):
    return math.sqrt((p2[0] - p1[0])**2 + (p2[1] - p1[1])**2 +
                     (p2[2] - p1[2])**2)


# number of vertices in each circle
circle_num_vertices = [
    # Distance between LEDs in each ring:
    6,    # Ring 0: [5, 6, 7] [19.42, 16.52, 14.34]
    11,    # Ring 1: [10, 11, 12] [18.94, 17.27, 15.86]
    15,    # Ring 2: [14, 15, 16] [18.77, 17.54, 16.46]
    19,    # Ring 3: [18, 19, 20] [17.88, 16.95, 16.11]
    21,    # Ring 4: [20, 21, 22] [18.42, 17.55, 16.76]
    23,    # Ring 5: [22, 23, 24] [18.35, 17.55, 16.83]
    25,    # Ring 6: [24, 25, 26] [17.78, 17.07, 16.42]
    25,    # Ring 7: [24, 25, 26] [18.23, 17.5, 16.83]
    25,    # Ring 8: [24, 25, 26] [18.1, 17.38, 16.71]
    24,    # Ring 9: [23, 24, 25] [18.05, 17.31, 16.62]
    22,    # Ring 10: [21, 22, 23] [17.98, 17.17, 16.43]
    18,    # Ring 11: [17, 18, 19] [18.8, 17.76, 16.84]
    14,    # Ring 12: [13, 14, 15] [18.61, 17.31, 16.17]
]

# z coordinate of each circle
circle_z = [
    119.978, 110.52, 98.0201, 83.8016, 68.496, 52.4351, 35.8279, 18.9098,
    1.91173, -14.8137, -30.7232, -45.036, -56.8221
]

# diameter of each circle
circle_diameter = [
    33.0401, 61.2911, 84.3618, 102.988, 117.7806, 128.9127, 136.2106, 139.6347,
    138.668, 132.5939, 120.6395, 102.3022, 77.7783
]

stats = ['[Ring #] [# LEDs] [distance b/t each LED] [angle b/t each LED]']
for i in range(len(circle_num_vertices)):
    num_vertices_opts = [
        circle_num_vertices[i] - 1, circle_num_vertices[i],
        circle_num_vertices[i] + 1
    ]
    distance_opts = []
    degree_step_opts = []
    radius = circle_diameter[i] / 2
    origin = (0, 0, circle_z[i])

    # Option 1
    circle_degrees = linspace(0, 360, num_vertices_opts[0], endpoint=False)
    p1 = poc(radius, origin, circle_degrees[0])
    p2 = poc(radius, origin, circle_degrees[1])
    distance_opts.append(round(distance(p1, p2), 2))
    degree_step_opts.append((circle_degrees[1] - circle_degrees[0]) / 2)

    # Option 2
    circle_degrees = linspace(0, 360, num_vertices_opts[1], endpoint=False)
    p1 = poc(radius, origin, circle_degrees[0])
    p2 = poc(radius, origin, circle_degrees[1])
    distance_opts.append(round(distance(p1, p2), 2))
    degree_step_opts.append((circle_degrees[1] - circle_degrees[0]) / 2)

    # Option 3
    circle_degrees = linspace(0, 360, num_vertices_opts[2], endpoint=False)
    p1 = poc(radius, origin, circle_degrees[0])
    p2 = poc(radius, origin, circle_degrees[1])
    distance_opts.append(round(distance(p1, p2), 2))
    degree_step_opts.append((circle_degrees[1] - circle_degrees[0]) / 2)

    stats.append('Ring ' + str(i) + ': ' + str(num_vertices_opts) + ' ' +
                 str(distance_opts) + ' ' + str(degree_step_opts))

print('\n'.join(stats))
