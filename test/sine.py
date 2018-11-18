#!/usr/bin/env python3

import math
import numpy as np


def main():
    for x in np.arange(-6, 6, 0.1):
        y = math.sin(x)
        print("{0} {1}".format(x, y))


if __name__ == "__main__":
    main()
