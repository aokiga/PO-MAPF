from collections import defaultdict


class Benchmark:
    def __init__(self):
        self.nlaunch = 0
        self.correct = defaultdict(list)
        self.runtime = defaultdict(list)
        self.ctnodes = defaultdict(list)
        self.answer = defaultdict(list)
        self.agentNums = []
