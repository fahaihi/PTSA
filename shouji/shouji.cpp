#include <iostream>
#include <vector>

using namespace std;

int count = 0;

int shouji(int readLength, string &refSeq, string &readSeq, int E, int gridSize) {
    // int HammingMask[(2 * E + 1) * readLength];
    // int ShoujiMask[readLength];
    vector<char> HammingMask((2 * E + 1) * readLength);
    vector<char> ShoujiMask(readLength);

    count = 0;
    for (int i = 0; i < readLength; i++) {
        HammingMask[i] = readSeq[i] != refSeq[i];
        count += HammingMask[i];
    }
    if (count <= E || E == 0) return readLength - count;


    // Shifted Hamming Masks
    for (int i = 1; i <= E; i++) {
        int deleteStart = i * readLength;
        int insertStart = deleteStart + E * readLength;

        // fill the shifted chars with Zeros
        for (int j = 0; j < i; j++) {
            // Deletion
            HammingMask[deleteStart + j] = 1;
            // Insertion
            HammingMask[insertStart + (readLength - 1) - j] = 1;
        }

        count = 0;
        //  shift the Read chars and compare
        for (int j = 0; j < readLength - i; j++) {
            //  Incremental-step right shifted Hamming Masks (Deletion)
            if (readSeq[j] != refSeq[j + i]) {
                HammingMask[deleteStart + j + i] = 1;
                count++;
            } else {
                HammingMask[deleteStart + j + i] = 0;
            }
        }
        if (count <= E) return readLength - count;

        count = 0;
        for (int j = 0; j < readLength - i; j++) {
            // Incremental-step left shifted Hamming Masks (Insertion)
            if (readSeq[j + i] != refSeq[j]) {
                HammingMask[insertStart + j] = 1;
                count++;
            }
            else {
                HammingMask[insertStart + j] = 0;
            }
        }
        if (count <= E) return readLength - count;
    }

    /*  Search for Longest Consecutive Zeros in a sliding window fashion*/
    for (int i = 0; i < readLength; i++) {
        ShoujiMask[i] = HammingMask[i];
    }

    count = 0;
    for (int gridIndex = 0; gridIndex <= readLength - gridSize; gridIndex++) {
        int maxZeros = 0, maxZerosIndex = 0;

        for (int rowIndex = 0; rowIndex < 2 * E + 1; rowIndex++) {
            int startIndex = rowIndex * readLength + gridIndex;

            count = 0;
            for (int i = 0; i < gridSize; i++) {
                if (HammingMask[startIndex + i] == 0) {
                    count++;
                }
            }
            if (count > maxZeros) {
                maxZeros = count;
                maxZerosIndex = rowIndex;
            } else if (count == maxZeros) {
                if (gridIndex < readLength - gridSize) {
                    if (HammingMask[startIndex] == 0)
                        maxZerosIndex = rowIndex;
                } else {
                    if (HammingMask[startIndex] == 0 && HammingMask[maxZerosIndex * readLength + gridIndex] == 1)
                        maxZerosIndex = rowIndex;
                }
            }
        }

        int magnetCount = 0;
        for (int i = 0; i < gridSize; i++) {
            if (ShoujiMask[gridIndex + i] == 0)
                magnetCount++;
        }
        if (maxZeros > magnetCount) {
            for (int i = 0; i < gridSize; i++) {
                ShoujiMask[gridIndex + i] = HammingMask[maxZerosIndex * readLength + gridIndex + i];
            }
        }
    }
    /*  END of Search for Longest Consecutive Zeros */

    count = 0;
    for (int i = 0; i < readLength; i++) {
        if (ShoujiMask[i] == 1) {
            count++;
            if (i + 1 < readLength && ShoujiMask[i + 1] == 1) {
                i++;
            }
        }
    }

    return readLength - count;
}
