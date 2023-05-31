#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>
using namespace std;
using namespace std::chrono;

int calculateHash(const vector<vector<int>> &matrix, int K) {
    int hashValue = 0;
    int mask = (1 << K) - 1;
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < K; j++) {
            hashValue = (hashValue << 1) | (matrix[i][j] & 1);
        }
    }
    return hashValue & mask;
}

bool isTopRightCornerDuplicated(const vector<vector<int>> &picture, int M, int N, int K) {
    if (M < K || N < K) {
        return false;
    }

    unordered_map<int, int> hashTable;

    // Calculate the hash value of the top-right corner of size K by K
    vector<vector<int>> pattern(K, vector<int>(K));
    for (int i = 0; i < K; i++) {
        for (int j = N - K; j < N; j++) {
            pattern[i][j - (N - K)] = picture[i][j];
        }
    }
    int patternHash = calculateHash(pattern, K);

    // Calculate the hash value of the first window
    vector<vector<int>> window(K, vector<int>(K));
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < K; j++) {
            window[i][j] = picture[i][j];
        }
    }
    int windowHash = calculateHash(window, K);

    hashTable[windowHash]++;

    for (int i = 0; i < M - K + 1; i++) {
        for (int j = K; j < N; j++) {
            for (int k = 0; k < K; k++) {
                window[k][0] = picture[i + k][j - K];
            }

            windowHash = calculateHash(window, K);

            if (windowHash == patternHash) {
                bool match = true;
                for (int x = 0; x < K; x++) {
                    for (int y = 0; y < K; y++) {
                        if (window[x][y] != pattern[x][y]) {
                            match = false;
                            break;
                        }
                    }
                    if (!match) {
                        break;
                    }
                }
                if (match) {
                    return true;
                }
            }

            hashTable[windowHash]++;
        }
    }

    return false;
}

// void printPicture(const vector<vector<int>> &picture) {
//     int M = picture.size();
//     int N = picture[0].size();

//     for (int i = 0; i < M; i++) {
//         for (int j = 0; j < N; j++) {
//             cout << picture[i][j] << " ";
//         }
//         cout << endl;
//     }
// }

int main() {
    vector<double> duplicatedExecutionTimes;
    vector<double> nonDuplicatedExecutionTimes;
    vector<int> inputSizes = {100, 200, 300, 400, 500};

    for (int size : inputSizes) {
        vector<vector<int>> picture(size, vector<int>(size));
        vector<vector<int>> duplicatedPattern(2, vector<int>(2));

        duplicatedPattern = {{1, 1}, {1, 1}};

        for (int i = 0; i < 2; ++i) {
            for (int j = size - 2; j < size; ++j) {
                picture[i][j] = duplicatedPattern[i][j - (size - 2)];
            }
        }

        auto duplicatedStartTime = high_resolution_clock::now();
        bool duplicated = isTopRightCornerDuplicated(picture, size, size, 2);
        auto duplicatedEndTime = high_resolution_clock::now();

        double duplicatedExecutionTime = duration_cast<milliseconds>(duplicatedEndTime - duplicatedStartTime).count();
        duplicatedExecutionTimes.push_back(duplicatedExecutionTime);

        // Set the top-right corner as non-duplicatedPattern
        for (int i = 0; i < 2; ++i) {
            for (int j = size - 2; j < size; ++j) {
                picture[i][j] = 0; // Set to a different value
            }
        }

        auto nonDuplicatedStartTime = high_resolution_clock::now();
        bool nonDuplicated = isTopRightCornerDuplicated(picture, size, size, 2);
        auto nonDuplicatedEndTime = high_resolution_clock::now();

        double nonDuplicatedExecutionTime = duration_cast<nanoseconds>(nonDuplicatedEndTime - nonDuplicatedStartTime).count();
        nonDuplicatedExecutionTimes.push_back(nonDuplicatedExecutionTime);

    }
    
    cout << "Execution Times:" << endl;
    for (int i = 0; i < inputSizes.size(); ++i) {
        cout << "Size: " << inputSizes[i] << " x " << inputSizes[i] << endl;
        cout << "Duplicated - Time: " << duplicatedExecutionTimes[i] << " milliseconds" << endl;
        cout << "Non-duplicated - Time: " << nonDuplicatedExecutionTimes[i] << " nanoseconds" << endl;
        cout << endl;
    }

    return 0;
}
