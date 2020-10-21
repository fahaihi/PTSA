#include <omp.h>
#include "ptsa.h"

using namespace std;

static char **mallocTwoDiArray(int row, int column) {
    char **mem = new char *[row];
    for (int i = 0; i < row; i++)
        mem[i] = new char[column];

    return mem;
}

static void deleteTwoDiArray(char **mem, int row, int column) {
    for (int i = 0; i < row; i++)
        delete[] mem[i];

    delete[] mem;
}

static inline char getHanMing(int row, int column, string &refSeq, string &readSeq, int E) {
    int readIndex = row <= E ? column - row : column + (row - E);

    if (readIndex >= 0 && readIndex < readSeq.length() && column < refSeq.length())
        return refSeq[column] != readSeq[readIndex];

    return true;
}

// window数组大小等于gridSize，contig数组长度固定为2
static inline void getContig(char *window, int gridSize, int firstColumn, int *contig) {
    int begin = 0, length = 0;

    int tmpLength = 0;
    for (int i = 0; i < gridSize; i++) {
        char value = window[(firstColumn + i) % gridSize];

        if (!value) tmpLength++;

        if (value || i == gridSize - 1) {
            if (tmpLength > length) {
                length = tmpLength;
                begin = i - tmpLength + (!value && i == gridSize - 1);
            }

            tmpLength = 0;
        }
    }

    contig[0] = begin;
    contig[1] = length;
}

int ptsaParallel(int readLength, int gridSize, int E, string &refSeq, string &readSeq, int stepValue) {
    int threadCount = omp_get_num_threads(); // 获取当前的线程数量
    int subLength = int(readLength / threadCount);
    int threadNumber = omp_get_thread_num(); // 获取当前块的线程号
    int beginPos = threadNumber * subLength;
    int endPos = beginPos + subLength;
    if (threadNumber == threadCount - 1) endPos = readLength;

    int row = 2 * E + 1;
    char **cachedHanMing = mallocTwoDiArray(row, gridSize);

    // 计算前面gridSize-1列的汉明矩阵
    for (int i = 0; i < gridSize - stepValue; i++)
        for (int j = 0; j < row; j++)
            cachedHanMing[j][i] = getHanMing(j, beginPos + i, refSeq, readSeq, E);

    int count = 0;
    int lastSelectedRow = -1;

    for (int i = beginPos; i < endPos; i += stepValue) {
        int firstColumn = (i - beginPos) % gridSize;

        // 更新最后一列汉明矩阵
        int lastColumn = (firstColumn + gridSize - stepValue) % gridSize;
        for (int j = 0; j < stepValue; j++)
            for (int k = 0; k < row; k++)
                cachedHanMing[k][(lastColumn + j) % gridSize] = getHanMing(k, i + (gridSize - stepValue) + j, refSeq,
                                                                           readSeq, E);

        // 选择一行滑动窗口
        int begin = 0, length = 0, selectedRow = 0;
        for (int j = 0; j < row; j++) {
            int contig[2];
            getContig(cachedHanMing[j], gridSize, firstColumn, contig);

            if (contig[1] > length || (contig[1] == length && contig[0] < begin)) {
                begin = contig[0];
                length = contig[1];
                selectedRow = j;
            }
        }
        // 根据选择的滑动窗口更新count
        int row = (length <= 1 && lastSelectedRow != -1) ? lastSelectedRow : selectedRow;
        for (int k = 0; k < stepValue; k++) {
            count += !cachedHanMing[row][(firstColumn + k) % gridSize];
        }

        lastSelectedRow = row;
    }

    deleteTwoDiArray(cachedHanMing, row, gridSize);

    return count;
}

int ptsa(int readLength, string &refSeq, string &readSeq, int E, int gridSize, int F, int threadCount, int stepValue) {
    // 先根据filter过滤
    int filterCount = 0;
    int maxShift = readLength - F;
# pragma omp parallel for num_threads(threadCount)
    for (int i = 0; i < 2 * maxShift + 1; i++) {
        if (filterCount > 0) continue;

        int count = 0;

        for (int j = 0; j < readSeq.length(); j++) {
            count += !getHanMing(i, j, refSeq, readSeq, maxShift);
        }

        if (count >= F) filterCount = count;
    }
    if (filterCount > 0) return filterCount;


    int count = 0;
# pragma omp parallel num_threads(threadCount) reduction(+:count)
    count += ptsaParallel(readLength, gridSize, E, refSeq, readSeq, stepValue);

    return count;
}
