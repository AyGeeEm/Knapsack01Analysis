#include "algorithm.h"
#include <iostream>
#include <vector>

int main() {
    int n, capacity;
    
    //Get user input
    cout << "Enter the number of items: ";
    cin >> n;
    
    vector<int> weights(n);
    vector<int> values(n);
    
    cout << "\nEnter the weights of each item:\n";
    for (int i = 0; i < n; i++) {
        cout << "Item " << i+1 << ": ";
        cin >> weights[i];
    }
    
    cout << "\nEnter values of each item:\n";
    for (int i = 0; i < n; i++) {
        cout << "Item " << i+1 << ": ";
        cin >> values[i];
    }
    
    cout << "\nEnter knapsack capacity: ";
    cin >> capacity;
    
    //Solve knapsack problem
    Knapsack knapsack;
    int maxValue = knapsack.compute(weights, values, capacity);
    
    //Output results
    cout << "\nResults -\n";
    cout << "Maximum value: " << maxValue << endl;
    
    vector<int> selectedItems = knapsack.getSelectedItems();
    cout << "Selected items:" << endl;
    
    if (selectedItems.empty()) {
        cout << "No items selected! This could be due to either capacity = 0, or no items fit" << endl;
    } else {
        for (int item : selectedItems) {
            //Adding +1 to item for correct item order
            cout << "- Item " << item+1 << ": Value = " << values[item] << ", Weight = " << weights[item] << endl;
        }
    }
    
    return 0;
}
