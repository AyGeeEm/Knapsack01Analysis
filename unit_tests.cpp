#include "algorithm.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>

//Function to generate random test cases
void generateTestCase(int n, int maxWeight, int maxValue, 
                     vector<int>& weights, vector<int>& values) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> weightDist(1, maxWeight);
    uniform_int_distribution<> valueDist(1, maxValue);
    
    weights.resize(n);
    values.resize(n);
    
    for (int i = 0; i < n; i++) {
        weights[i] = weightDist(gen);
        values[i] = valueDist(gen);
    }
}

//Function to measure execution time
double measureExecutionTime(const vector<int>& weights, 
                           const vector<int>& values, 
                           int capacity) {
    Knapsack knapsack;
    
    auto start = chrono::high_resolution_clock::now();
    knapsack.compute(weights, values, capacity);  //No need to store result
    auto end = chrono::high_resolution_clock::now();
    
    chrono::duration<double, milli> duration = end - start;
    return duration.count();
}

//Function to run a small verification test
void verificationTest() {
    cout << "Starting algorithm verification test -" << endl;
    
    vector<int> weights = {8, 8, 1};
    vector<int> values = {3, 2, 9};
    int capacity = 10;
    
    Knapsack knapsack;
    int maxValue = knapsack.compute(weights, values, capacity);
    
    //(Obtained test case from online dataset KNAPSACK_01 from John Burkardt)
    cout << "Test case: Weights = {8, 8, 1}, Values = {3, 2, 9}, Capacity = 10" << endl;
    cout << "Expected result: 12" << endl;
    cout << "Actual result: " << maxValue << endl;
    
    if (maxValue == 12) {
        cout << "Verification passed!" << endl;
    } else {
        cout << "Verification failed!" << endl;
    }
    cout << endl;
}

int main() {
    //Running the small verification test first
    verificationTest();
    
    //Experimental analysis for running time with different input sizes
    cout << "Starting running time performance tests -" << endl;
    
    ofstream resultsFile("analysis_results.csv");
    resultsFile << "Size,Time(ms)" << endl;
    
    vector<int> testSizes = {5, 10, 20, 50, 100, 200, 500, 1000, 1500, 2000};
    for (int size : testSizes) {
        vector<int> testWeights, testValues;
        generateTestCase(size, 100, 100, testWeights, testValues);
        int testCapacity = 0.7 * size * 70; //Using this to set the capacity at 70% of max possible total weight
        
        double time = measureExecutionTime(testWeights, testValues, testCapacity);
        
        cout << "Size: " << size << ", Time: " << time << " ms" << endl;
        resultsFile << size << "," << time << endl;
    }
    
    resultsFile.close();
    cout << "Results saved to analysis_results.csv" << endl;
    
    return 0;
}
