#include <iostream>
#include <fstream>
using namespace std;

// Function prototype for the Banker's Algorithm
void ExecuteBankersAlgorithm(int numProcesses, int numResources, int currentAllocations[][3], int maxRequirements[][3], int availableResources[3]);

int main() {
    int numProcesses, numResources;

    // Open the input file
    ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        cerr << "Error opening the input file." << endl;
        return 1;
    }

    // Read the number of processes and resources
    inputFile >> numProcesses >> numResources;

    // Declare matrices and arrays for allocations, maximum requirements, and available resources
    int currentAllocations[5][3], maxRequirements[5][3], availableResources[3];

    // Read the current allocation matrix
    for (int i = 0; i < numProcesses; ++i)
        for (int j = 0; j < numResources; ++j)
            inputFile >> currentAllocations[i][j];

    // Read the maximum requirement matrix
    for (int i = 0; i < numProcesses; ++i)
        for (int j = 0; j < numResources; ++j)
            inputFile >> maxRequirements[i][j];

    // Read the available resources array
    for (int i = 0; i < numResources; ++i)
        inputFile >> availableResources[i];

    // Close the input file
    inputFile.close();

    // Execute the Banker's Algorithm
    ExecuteBankersAlgorithm(numProcesses, numResources, currentAllocations, maxRequirements, availableResources);

    return 0;
}

void ExecuteBankersAlgorithm(int numProcesses, int numResources, int currentAllocations[][3], int maxRequirements[][3], int availableResources[3]) {
    
    // Array to track the completion status of each process
    int processCompletionFlags[numProcesses];
    for(int i = 0; i < numProcesses; ++i){
        processCompletionFlags[i] = 0;
    }

    // Array to store the safe sequence of processes
    int safeSequence[numProcesses];
    int safeIndex = 0;

    // Calculate the need matrix by subtracting allocations from max requirements
    int needMatrix[numProcesses][numResources];
    for (int i = 0; i < numProcesses; ++i)
        for (int j = 0; j < numResources; ++j)
            needMatrix[i][j] = maxRequirements[i][j] - currentAllocations[i][j];

    // Find the safe sequence by iterating over processes
    for (int iteration = 0; iteration < 5; ++iteration) {
        for (int processIndex = 0; processIndex < numProcesses; ++processIndex) {
            // Check if the process is unfinished
            if (processCompletionFlags[processIndex] == 0) {
                int canExecute = 1;
                
                // Check if resources needed are less than or equal to available
                for (int resourceIndex = 0; resourceIndex < numResources; ++resourceIndex) {
                    if (needMatrix[processIndex][resourceIndex] > availableResources[resourceIndex]) {
                        canExecute = 0;
                        break;
                    }
                }

                // If all needed resources are available, mark the process as complete
                if (canExecute == 1) {
                    safeSequence[safeIndex++] = processIndex;
                    
                    // Release allocated resources back to the available pool
                    for (int resource = 0; resource < numResources; ++resource)
                        availableResources[resource] += currentAllocations[processIndex][resource];
                    processCompletionFlags[processIndex] = 1;
                }
            }
        }
    }

    // Check if all processes could be safely completed
    int isSafe = 1;
    for (int processIndex = 0; processIndex < numProcesses; ++processIndex) {
        if (processCompletionFlags[processIndex] == 0) {
            isSafe = 0;
            cout << "Unsafe sequence" << endl;
            break;
        }
    }

    // Print the safe sequence if it exists
    if (isSafe == 1) {
        cout << "Safe sequence order:" << endl;
        for (int processIndex = 0; processIndex < numProcesses - 1; ++processIndex)
            cout << " process:" << safeSequence[processIndex] << " ,";
        cout << " process:" << safeSequence[numProcesses - 1] << endl;
    }
}
