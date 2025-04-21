#include "algorithm.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>
#include <iomanip>

//In the case that unit tests are being run on a non Windows system, there's a fallback!!
#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <psapi.h>
    
    //Gets current memory usage in KB
    size_t getMemoryUsage() {
        PROCESS_MEMORY_COUNTERS_EX pmc;
        GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
        return pmc.WorkingSetSize / 1024; // Convert bytes to KB
    }
#else
    size_t getMemoryUsage() {
        return 0;
    }
#endif

//Struct to hold runtime and memory usage metrics
struct PerformanceMetrics {
    double runTime;
    size_t peakMemory;
};

//Modified Knapsack class with memory tracking
class KnapsackMemTracking : public Knapsack {
private:
    size_t peakMemory;

public:
    KnapsackMemTracking() : peakMemory(0) {}
    
    int solve(const vector<int>& weights, const vector<int>& values, int capacity) {
        //Measure memory before allocation
        size_t memBefore = getMemoryUsage();

        int result = Knapsack::compute(weights, values, capacity);
        
        //Measure memory after allocation
        size_t memAfter = getMemoryUsage();

        //Check if memory usage increased and calculate the additional memory consumed
        if (memAfter > memBefore) {
            peakMemory = memAfter - memBefore;
        } else {
            peakMemory = 0; //No increase in memory usage
        }
        
        return result;
    }
    
    size_t getPeakMemory() const {
        return peakMemory;
    }
    
    //Theoretical memory usage
    size_t getTheoreticalMemory(int n, int capacity) const {
        //Formula considers the size of DP table (sizeof(int) * (n+1) * (capacity+1)) and the size of selected table (sizeof(bool) * (n+1) * (capacity+1))
        return (4 * (n+1) * (capacity+1) + 1 * (n+1) * (capacity+1)) / 1024; //(Assuming int is 4 bytes and bool is 1 byte)
    }
};

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

//Function to measure execution time and mem usage
PerformanceMetrics getMetrics(const vector<int>& weights, const vector<int>& values, int capacity) {
    KnapsackMemTracking knapsack;
    PerformanceMetrics metrics;
    
    auto start = chrono::high_resolution_clock::now();
    
    //Execute algo
    knapsack.solve(weights, values, capacity);
    
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;
    
    metrics.runTime = duration.count();
    metrics.peakMemory = knapsack.getPeakMemory();
    
    return metrics;
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
    cout << "Starting performance tests -" << endl;
    
    ofstream resultsFile("analysis_results.csv");
    resultsFile << "Size,Capacity,Time(ms),Memory(KB),TheoreticalMemory(KB)" << endl;
    
    vector<int> testSizes = {5, 10, 20, 50, 100, 200, 500, 1000, 1500, 2000, 3000, 5000};

    for (int size : testSizes) {
        vector<int> testWeights, testValues;
        generateTestCase(size, 100, 100, testWeights, testValues);
        int testCapacity = 0.7 * size * 70; //Using this to set the capacity at 70% of max possible total weight
        
        //Measure time and memory
        PerformanceMetrics metrics = getMetrics(testWeights, testValues, testCapacity);

        //Calculate theoretical memory usage
        KnapsackMemTracking knapsack;
        size_t theoreticalMemory = knapsack.getTheoreticalMemory(size, testCapacity);
        
        cout << "Size: " << size << ", Capacity: " << testCapacity << ", Time: " << metrics.runTime << " ms" << ", Memory: " << metrics.peakMemory << " KB" << endl;
        resultsFile << size << "," << testCapacity << "," << metrics.runTime << ","  << metrics.peakMemory << "," << theoreticalMemory << endl;

        resultsFile.flush(); //In case of data loss, flush file buffer
    }
    
    resultsFile.close();
    cout << "\nResults saved to analysis_results.csv" << endl;
    
    return 0;
}
