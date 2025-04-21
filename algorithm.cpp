#include <iostream>
#include <algorithm>
#include "algorithm.h"

Knapsack::Knapsack() {}

int Knapsack::compute(const vector<int>& weights, const vector<int>& values, int capacity) {
    int n = weights.size();
    
    //Store weights for later use (getSelectedItems)
    weightsArr = weights;
    
    //Initialize DP table with dimensions [n+1][capacity+1]
    dpTable.resize(n + 1, vector<int>(capacity + 1, 0));
    selected.resize(n + 1, vector<bool>(capacity + 1, false));
    
    //Fill DP table using bottom up approach
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            //If current item doesn't fit, take the value without including it
            if (weights[i-1] > w) {
                dpTable[i][w] = dpTable[i-1][w];
                selected[i][w] = false;
            } else {
                //Compare value with and without including current item
                int valueWithout = dpTable[i-1][w];
                int valueWith = values[i-1] + dpTable[i-1][w - weights[i-1]];
                
                //Take the max value
                if (valueWith > valueWithout) {
                    dpTable[i][w] = valueWith;
                    selected[i][w] = true;  //Marking item as selected
                } else {
                    dpTable[i][w] = valueWithout;
                    selected[i][w] = false;
                }
            }
        }
    }
    
    return dpTable[n][capacity];
}

vector<int> Knapsack::getSelectedItems() const {
    vector<int> items;
    int n = dpTable.size() - 1;
    int w = dpTable[0].size() - 1;
    
    //Backtracking through DP table
    int i = n;
    int currentCap = w;
    
    while (i > 0 && currentCap > 0) {
        if (selected[i][currentCap]) {
            //This item is part of the solution
            items.push_back(i - 1);  //Convert for index
            currentCap -= weightsArr[i-1];
        }
        i--;
    }
    
    //Reverse to get items in original order (optional)
    reverse(items.begin(), items.end());
    
    return items;
}

//STILL TESTING THIS FUNCTION
void Knapsack::printDPTable() const {
    cout << "DP Table:" << endl;
    for (size_t i = 0; i < dpTable.size(); i++) {
        for (size_t w = 0; w < dpTable[i].size(); w++) {
            cout << dpTable[i][w] << " ";
        }
        cout << endl;
    }
    
    cout << "\nSelection Table:" << endl;
    for (size_t i = 0; i < selected.size(); i++) {
        for (size_t w = 0; w < selected[i].size(); w++) {
            cout << (selected[i][w] ? "1" : "0") << " ";
        }
        cout << endl;
    }
}
