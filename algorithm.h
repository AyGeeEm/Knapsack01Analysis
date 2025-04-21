#pragma once
#include <vector>
#include <utility>

using namespace std;

//DP algorithm for solving the 0/1 Knapsack problem
class Knapsack {
private:
    vector<vector<int>> dpTable; //Stores DP table
    vector<vector<bool>> selected; //Vector for backtracking solution
    vector<int> weightsArr; //Stored weights for reconstruction

public:
    Knapsack();
    int compute(const vector<int>& weights, const vector<int>& values, int capacity);
    vector<int> getSelectedItems() const;
    void printDPTable() const; //Added for testing output values on DP table
};
