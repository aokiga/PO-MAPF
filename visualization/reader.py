import numpy as np
from agent import Agent


def read_res(file_name):
    with open(file_name, 'r') as f:
        n = int(f.readline().strip())
        res = np.empty(n, dtype=Agent)
        for i in range(n):
            index = int(f.readline().strip())
            answer, true_answer = list(map(int, f.readline().strip().split()))
            m = int(f.readline().strip())
            path = np.empty(m, dtype=np.dtype('int, int'))
            for j in range(m):
                x, y = list(map(int, f.readline().strip().split()))
                path[j] = (x, y)
            res[i] = Agent(i, answer, true_answer, path)
    return res


def read_map(file_name):
    with open(file_name, 'r') as f:
        f.readline()
        height = f.readline().split()[1]
        width = f.readline().split()[1]
        f.readline()
        data = np.array(list(map(lambda x: x.strip(), f.readlines())))
    return np.array(data)
