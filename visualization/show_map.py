import copy

import numpy as np


class Cell:
    def __init__(self, background=0, robot_color=None, path=None):
        self.background = background
        self.robot_color = robot_color
        self.path = path


def showCell(C, i, j, scen):
    if C == '@':
        background = 1
    else:
        background = 0
    for agent in scen:
        if (j, i) == agent.end:
            background = agent.i + 2
    return Cell(background=background)


def show_map(Map, Scen, Res, withPath):
    MAP = np.array([np.array([showCell(c, i, j, Scen) for j, c in enumerate(row)]) for i, row in enumerate(Map)])
    result = []
    maxt = max(map(lambda agent: len(agent.path), Res))

    for t in range(maxt):
        NMAP = copy.deepcopy(MAP)
        for agent in Res:
            if t < len(agent.path):
                pos = agent.path[t]
            else:
                pos = agent.path[-1]
            if withPath:
                if t < len(agent.path):
                    NMAP[pos[0]][pos[1]].path = agent.path[0:t + 1]
                else:
                    NMAP[pos[0]][pos[1]].path = agent.path
            NMAP[pos[0]][pos[1]].robot_color = agent.i + 2
        result.append(NMAP)
    return result
