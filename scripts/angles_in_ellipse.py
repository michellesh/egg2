# script for finding points evenly spaced around the perimeter of an ellipse

import numpy as np
import scipy.special
import scipy.optimize


def angles_in_ellipse(num, a, b):
    '''
    source: https://stackoverflow.com/questions/6972331/
    '''
    assert (num > 0)
    assert (a < b)
    angles = 2 * np.pi * np.arange(num) / num
    if a != b:
        e2 = (1.0 - a**2.0 / b**2.0)
        tot_size = scipy.special.ellipeinc(2.0 * np.pi, e2)
        arc_size = tot_size / num
        arcs = np.arange(num) * arc_size
        res = scipy.optimize.root(
            lambda x: (scipy.special.ellipeinc(x, e2) - arcs), angles)
        angles = res.x
    return angles


num_rings = 13
n = num_rings * 2 + 2    # number of points to evenly distribute around ellipse
a = 70    # x-axis radius of ellipse
b = 100    # y-axis radius of ellipse

phi = angles_in_ellipse(n, a, b)
print(list(np.rad2deg(phi)))
print(list(b * np.sin(phi)))
print(list(a * np.cos(phi)))
