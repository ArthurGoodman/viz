#!/usr/bin/env python3

import random
import time


c_range = 1
c_dt = 1e-2


def rand():
    return random.uniform(-c_range, c_range)


def main():
    pos = [0.0, 0.0, 0.0]
    vel = [0.0, 0.0, 0.0]

    while True:
        print("p {0} {1} {2}".format(pos[0], pos[1], pos[2]))
        print("r {0} {1} {2} {3}".format(pos[0], pos[1], pos[2], 0))
        print("#comment")

        delta = (rand(), rand(), rand())

        pos[0] += vel[0] * c_dt
        pos[1] += vel[1] * c_dt
        pos[2] += vel[2] * c_dt
        vel[0] += delta[0]
        vel[1] += delta[1]
        vel[2] += delta[2]

        time.sleep(c_dt)


if __name__ == "__main__":
    main()
