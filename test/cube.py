#!/usr/bin/env python3

import numpy as np


c_side = 1
c_step = 0.414241


def dim(index):
    it = [0, 0, 0]
    ar = np.arange(-c_side / 2, c_side / 2 + c_step, c_step)

    for it[index[0]] in ar:
        for it[index[1]] in ar:
            for it[index[2]] in ar:
                print("{0} {1} {2}".format(it[0], it[1], it[2]))
            print("+")


def main():
    dim((0, 1, 2))
    dim((0, 2, 1))
    dim((1, 2, 0))


if __name__ == "__main__":
    main()

