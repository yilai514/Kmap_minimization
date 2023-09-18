# K-map-based logic minimization
## Introduction
* Objective: Implement the k-map-based logic minimization approach
* Description
  * The program reads a Boolean function (with 4 variables) in the k-map format
  * Then, it applies the k-map-based logic minimization approach to optimize the Boolean function
  * Finally, it outputs the minimized Boolean function in the SOP form
## Input format
* The first integer indicates the number of Boolean functions
* Each line represents a Boolean function in the k-map format
  * 16 integers separated with a space: 0 for 0, 1 for 1, and 2 for X
  * These 16 integers correspond to the squares in the k map from top to down and left to right in order

    
        2
        1 0 1 1 0 1 1 0 0 1 1 0 1 1 1 1
        2 1 1 2 0 2 1 0 0 0 1 0 0 0 1 0

