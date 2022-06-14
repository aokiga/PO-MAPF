# PO-MAPF
This repository contains the POHCA* algorithm for solving the PO-MAPF problem.

##### Table of Contents
[Setting parameters](#setting-parameters)   
[POHCA parameters](#pohca-parameters)
[Visualization](#visualization)    
[Benchmarks](#benchmarks)


<a name="params"/>

##Setting parameters
The format of input data for maps/scenarios is taken from the resource [Movingai.com](https://movingai.com/benchmarks/mapf.html).

To set the startup parameter, params.conf is used. A specific format was chosen to represent the parameters for ease of reading.

Example:
```
LAUNCH {
    MAP_FILE_NAME empty-16-16.map
    SCEN_FILE_NAME empty-16-16.scen
    MOVE_LIMIT 100
    TMP_PATH ../data/empty-16-16/

    <ALGORITHM params>
}
```
###Possible parameters:
- `MAP_FILE_NAME <NAME>` - File name of file containing a map
- `SCEN_FILE_NAME <NAME>` - File name of file containing a scenario
- `MOVE_LIMIT <N>` - maximum number of steps in the algorithm
- `TMP_PATH` - the path from the current folder to the folder with the map and scenario files

After the algorithm is started, it will leave behind a file with the `.ans` extension. It can be used for visualization in a notebook.

##POHCA parameters
The keyword `ALGORITHM` is used to set parameter algorithms.  

Usage:
```
ALGORITHM POHCAStar {
    Param1
    Param2
}
```

###Possible parameters:

- `NAME <NAME>` - the name of the algorithm in the resulting file
- `PRIORITY <PRIORIY_STRATEGY>` - priority allocation strategy
- - `RANDOM` - Distribute priorities randomly
- - `LONGEST_FIRST` - Distribute priorities first to agents who have the maximum Manhattan distance
- - `SHORTEST_FIRST` - Distribute priorities first to agents who have the minimum Manhattan distance
- `RADIUS <N>` - Radius of vision
- `EXCHANGE_MODE <MODE>` - A parameter that specifies which agents can be exchanged with
- - `ALL` - With everyone
- - `ONLY_VISIBLE` - Only with visible
- - `VISIBLE_CHAIN` - With visible + through an other agents
- `FAST_REPLANNING`- Flag for using planning optimization. it is recommended to always put it
- `SEND_PATH` - Flag to indicate that we can exchange the paths.
- `SEND_MAP` - Flag to indicate that we can exchange the map.
- `NO_PATH_STRATEGY <STRATEGY>` - Choosing a strategy for a fully decentralized version


<a name="visualization"/>

##Visualization
To visualize the result of the algorithm, it is proposed to use Jupyter Notebook `visualization/visualization.ipynb`.    
Inside this notebook you can find examples of interaction and visualization of benchmarks or the result of the algorithm.

<a name="visualization"/>

##Benchmarks
The benchmark mode is also supported in the configuration file.   
Each algorithm will be called `N_LAUNCH` times for a different number of agents. The number of agents is initially 1, but after each launch it increases by `ADD_AGENTS` to the `MAX_AGENT_NUM` limit.    
Also, the benchmark mode does not have to submit a scenario file, then a random scenario will be automatically generated.
The benchmark mode will output a file `.bin`, which can then be used in the jupyter notebook to visualize the results.   
All algorithms specified in the file will be used in experimental testing.   

Example:
```
BENCHMARK {
    MAP_FILE_NAME house.map
    MOVE_LIMIT 100
    TMP_PATH ../data/house/
    NLAUNCH 10
    MAX_AGENT_NUM 50
    ADD_AGENTS 5
    
    ALGORITHM POHCAStar {
        NAME CENTRALIZED
        PRIORITY RANDOM
        RADIUS 5
        EXCHANGE_MODE ALL
        FAST_REPLANNING
        SEND_PATH
        SEND_MAP
        CONFLICT_RESOLUTION_MODE RANDOM
        NO_PATH_STRATEGY POSITION
    }
    
    ALGORITHM POHCAStar {
        NAME PARTIALLY-CENTRALIZED
        PRIORITY RANDOM
        RADIUS 5
        EXCHANGE_MODE ONLY_VISIBLE
        FAST_REPLANNING
        SEND_PATH
        SEND_MAP
        CONFLICT_RESOLUTION_MODE RANDOM
        NO_PATH_STRATEGY POSITION
    }
}
```