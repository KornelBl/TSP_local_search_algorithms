# TSP_local_search_algorithms
Implementation of Simulated Annealing and Tabu Search for Travelling Salesman Problem<br />

Input data is taken from a text file<br />
Exemplary file could look like this (First number is the size of the matrix):<br />

6<br />
0 20 30 31 28 40<br />
30 0 10 14 20 44<br />
40 20 0 10 22 50<br />
41 24 20 0 14 42<br />
38 30 32 24 0 28<br />
50 54 60 52 38 0<br />

Program allows the user to change neighbour types, first path and few other parameters unique for either Simulated Annealing or Tabu Search.

### Shared parameters
- first TSP path
  - random
  - NN based algorithm
- neighbour type
  - swap
  - insert
  - asymmetric invert
  - symmetric invert (faster than asymmetric invert, but works only for symmetric TSP)


### Simulated Annealing exclusive parameters:
- cooling schedule:
  - geometric
  - linear
  - logarithmic

### Tabu Search exclusive parameters:
- aspiration on/off
- diversification on/off (critical event based)
- tabu list implementation
  - matrix
  - list


## Author
Kornel Błąkała
