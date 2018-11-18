#!/usr/bin/env python3

import math
import numpy as np


def main():
    for t in np.arange(-100, 100, 0.1):
        a = 0.1
        c = math.atan(a * t)

        x = math.cos(t) * math.cos(c)
        y = math.sin(t) * math.cos(c)
        z = -math.sin(c)

        print("{0} {1} {2}".format(x, y, z))


if __name__ == "__main__":
    main()
