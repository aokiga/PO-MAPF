import numpy as np

from agent import Agent
from agentRes import AgentRes
from benchmark import Benchmark


def read_res(file_name):
    with open(file_name, 'r') as f:
        n = int(f.readline().strip())
        res = np.empty(n, dtype=AgentRes)
        for i in range(n):
            index = int(f.readline().strip())
            answer = int(f.readline().strip())
            m = int(f.readline().strip())
            path = np.empty(m, dtype=np.dtype('int, int'))
            for j in range(m):
                x, y = list(map(int, f.readline().strip().split()))
                path[j] = (x, y)
            res[i] = AgentRes(i, answer, path)
    return res


def read_scen(file_name):
    with open(file_name, 'r') as f:
        f.readline()
        data = list(map(lambda x: x.strip().split(), f.readlines()))
        n = len(data)
        res = np.empty(n, dtype=Agent)
        for i in range(n):
            start = (int(data[i][4]), int(data[i][5]))
            end = (int(data[i][6]), int(data[i][7]))
            res[i] = Agent(i, start, end)
    return res


def read_map(file_name):
    with open(file_name, 'r') as f:
        f.readline()
        height = f.readline().split()[1]
        width = f.readline().split()[1]
        f.readline()
        data = np.array(list(map(lambda x: x.strip(), f.readlines())))
    return np.array(data)


def is_number(x):
    return x.is_digit()


def read_benchmark_tmp(file_name):
    z = []
    with open(file_name, 'r') as f:
        for i in range(15):
            lst = []
            algo = f.readline().strip()
            for j in range(7):
                n, passed = list(map(int, f.readline().strip().split()))
                lst.append((n, passed))
            z.append((algo, lst))
    return z


def read_benchmark(file_name):
    x = Benchmark()
    with open(file_name, 'r') as f:
        NAGENTS = int(f.readline().strip())
        NLAUNCH = int(f.readline().strip())
        NALGO = int(f.readline().strip())
        for i in range(NAGENTS):
            agentNum = int(f.readline().split()[1])
            x.agentNums.append(agentNum)
            for j in range(NLAUNCH):
                for k in range(NALGO):
                    name, correct = f.readline().split()
                    x.correct[name].append((agentNum, correct))
                    if correct:
                        answer = int(f.readline().split()[1])
                        x.answer[name].append((agentNum, answer))
                        runtime = float(f.readline().split()[1])
                        x.runtime[name].append((agentNum, runtime))
                        ctnodes = int(f.readline().split()[1])
                        x.ctnodes[name].append((agentNum, ctnodes))
    return x