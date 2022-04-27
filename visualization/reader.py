from collections import defaultdict

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
            T = int(f.readline().strip())
            new_visible = []
            for t in range(T):
                m = int(f.readline().strip())
                nc = np.empty(m, dtype=np.dtype('int, int'))
                for j in range(m):
                    x, y = list(map(int, f.readline().strip().split()))
                    nc[j] = (x, y)
                new_visible.append(nc)
            res[i] = AgentRes(i, answer, path, new_visible)
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


def read_benchmark(file_name, algoNum):
    success = []
    ans = []
    nodes = []
    time = []
    with open(file_name, 'r') as f:
        NLAUNCH = int(f.readline().strip())
        for nlaunch in range(NLAUNCH):
            num = int(f.readline().strip())
            print(num)
            tmp = []
            for algoN in range(algoNum):
                algo = f.readline().strip()
                print(algo)
                algores = []
                while True:
                    nagents = int(f.readline().strip())
                    flag = int(f.readline().strip())
                    if flag == 0:
                        break
                    answer = int(f.readline().strip().split()[1])
                    runtime = float(f.readline().strip().split()[1])
                    ctnodes = int(f.readline().strip().split()[1])
                    algores.append((nagents, answer, runtime, ctnodes))
                tmp.append((algo, algores))
            tmpsucc = list(map(lambda xx: (xx[0], list(map(lambda yy: yy[0], xx[1]))), tmp))
            minn = min(map(lambda xx: len(xx[1]), tmp))
            x = list(map(lambda xx: (xx[0], xx[1][:minn]), tmp))
            tmpans = list(map(lambda xx: (xx[0], list(map(lambda yy: (yy[0], yy[1]), xx[1]))), x))
            tmptime = list(map(lambda xx: (xx[0], list(map(lambda yy: (yy[0], yy[2]), xx[1]))), x))
            tmpctnodes = list(map(lambda xx: (xx[0], list(map(lambda yy: (yy[0], yy[3]), xx[1]))), x))
            success.append(dict(tmpsucc))
            ans.append(dict(tmpans))
            nodes.append(dict(tmpctnodes))
            time.append(dict(tmptime))
    algos = success[0].keys()
    s = []
    a = []
    n = []
    t = []
    for algo in algos:
        tmpans = defaultdict(list)
        tmpsucc = defaultdict(int)
        tmptime = defaultdict(list)
        tmpnodes = defaultdict(list)
        for launch in range(NLAUNCH):
            for nagents, nodess in nodes[launch][algo]:
                tmpsucc[nagents] += 1 if (nodess == nagents) else 0
            #for nagents in success[launch][algo]:
            #    tmpsucc[nagents] += 1
            for nagents, answerr in ans[launch][algo]:
                tmpans[nagents].append(answerr)
            for nagents, nodess in nodes[launch][algo]:
                tmpnodes[nagents].append(nodess / nagents)
            for nagents, timme in time[launch][algo]:
                tmptime[nagents].append(timme)
        s.append((algo, tmpsucc))
        a.append((algo, tmpans))
        t.append((algo, tmptime))
        n.append((algo, tmpnodes))

    return dict(s), dict(a), dict(n), dict(t), NLAUNCH
