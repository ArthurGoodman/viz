#!/usr/bin/env python3

import random

c_range = 1e-3
c_dt = 1e-4

def main():
    pos = [0.0, 0.0, 0.0]
    vel = [0.0, 0.0, 0.0]

    while True:
        print("{0} {1} {2}".format(pos[0], pos[1], pos[2]))
        delta = (random.uniform(-c_range, c_range),
                 random.uniform(-c_range, c_range),
                 random.uniform(-c_range, c_range))
        pos[0] += vel[0] * c_dt
        pos[1] += vel[1] * c_dt
        pos[2] += vel[2] * c_dt
        vel[0] += delta[0]
        vel[1] += delta[1]
        vel[2] += delta[2]


if __name__ == "__main__":
    main()
