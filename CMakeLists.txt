cmake_minimum_required(VERSION 3.20)
project(PO_MAPF)

set(CMAKE_CXX_STANDARD 17)

add_executable(PO_MAPF main.cpp src/map/Graph.cpp src/map/Graph.h src/map/Node.cpp src/map/Node.h src/scen/Scenario.h src/scen/Agent.h src/scen/AgentResult.h src/algorithms/MDSTAR-Lite.cpp src/algorithms/MDSTAR_Lite.h src/scen/ScenarioResult.h src/io/Reader.cpp src/io/Reader.h src/algorithms/MAPFAlgorithm.h src/io/Writer.cpp src/io/Writer.h src/map/Map.cpp src/map/Map.h src/algorithms/PPAStar.cpp src/algorithms/PPAStar.h src/algorithms/CBS.cpp src/algorithms/CBS.h src/algorithms/PathPlanningWithConstraints.cpp src/algorithms/PathPlanningWithConstraints.h)
