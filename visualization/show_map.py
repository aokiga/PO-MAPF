import copy

import numpy as np


class Cell:
    def __init__(self, background=0, robot_color=None, path=None):
        self.background = background
        self.robot_color = robot_color
        self.path = path


def showCell(C, i, j, scen, known):
    if C == '@':
        background = 1
    else:
        background = 0
    if not known:
        background = 2
    for agent in scen:
        if (j, i) == agent.end:
            background = agent.i + 3
    return Cell(background=background)


def show_map(Map, Scen, Res, agentNum, withPath):
    result = []
    maxt = max(map(lambda agent: len(agent.path), Res))
    known = [[False] * len(row) for row in Map]
    size = (len(Map), len(Map[0]))

    print(Res[agentNum].answer)

    for t in range(maxt):
        agent1 = Res[agentNum]
        ncells = agent1.new_visible[t]
        for i, j in ncells:
            known[i][j] = True
        NMAP = np.array([np.array([showCell(c, i, j, Scen, known[i][j]) for j, c in enumerate(row)]) for i, row in enumerate(Map)])
        for agent in Res:
            if t < len(agent.path):
                pos = agent.path[t]
            else:
                pos = agent.path[-1]
            if withPath and agentNum == agent.i:
                if t < len(agent.path):
                    NMAP[pos[0]][pos[1]].path = agent.path[0:t + 1]
                else:
                    NMAP[pos[0]][pos[1]].path = agent.path
            NMAP[pos[0]][pos[1]].robot_color = agent.i + 3

        result.append(NMAP)
    return result
