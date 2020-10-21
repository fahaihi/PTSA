#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <cstring>
#include <omp.h>

#include "ptsa/ptsa.h"
#include "shouji/shouji.h"
#include "editor/editor.h"

using namespace std;

int main(int argc, const char *argv[]) {
    int gridSize, stepValue, threadCount = 1;
    double e, filter;
    string inputFilename;

    int runFunction;

    switch (argc) {
        case 3:
            runFunction = 0; // editor
            e = atof(argv[1]);
            inputFilename = argv[2];
            break;
        case 4:
            runFunction = 1; // shouji
            gridSize = atoi(argv[1]);
            e = atof(argv[2]);
            inputFilename = argv[3];
            break;
        case 7:
            runFunction = 2; // ptsa
            gridSize = atoi(argv[1]);
            stepValue = atoi(argv[2]);
            e = atof(argv[3]);
            filter = atof(argv[4]);
            threadCount = atoi(argv[5]);
            inputFilename = argv[6];
            break;
        default:
            cerr << "参数个数不正确！" << endl;
            exit(-1);
    }

    ifstream ifs; // 创建流对象用于读文件
    ifs.open(inputFilename, ios::in);
    if (!ifs.is_open()) {
        cerr << "文件打开失败！" << endl;
        exit(-1);
    }

    cout << setiosflags(ios::fixed) << setprecision(3);
    cout << setiosflags(ios::right);

    cout << setw(6) << "Number" << setw(15) << "Length" << setw(15) << "Thread" << setw(15) << "Sim Num" << setw(15)
         << "Sim Deg" << setw(15) << "Time" << endl;

    const int countStep = 5;
    const int countLength = 100 / countStep;

    int countNumberE[countLength]; // 统计NumberE
    memset(countNumberE, 0, sizeof(countNumberE));

    double countTimeSpend[countLength]; // 统计时间花费
    memset(countTimeSpend, 0, sizeof(countTimeSpend));

    double totalTimeSpend = 0;

    string refSeq, readSeq;
    for (int i = 0; getline(ifs, refSeq); i++) {
        getline(ifs, readSeq);

        int readLength = int(max(refSeq.size(), readSeq.size()));
        int E = int(e * readLength);
        int F = int(filter * readLength);

        double begin = omp_get_wtime();
        int count = 0;
        switch (runFunction) {
            case 0:
                count = uiLevenshteinDistance(refSeq, readSeq);
                count = readLength - count;
                break;
            case 1:
                count = shouji(readLength, refSeq, readSeq, E, gridSize);
                break;
            case 2:
                count = ptsa(readLength, refSeq, readSeq, E, gridSize, F, threadCount, stepValue);
                break;
        }
        double end = omp_get_wtime();
        double timeSpend = end - begin;
        totalTimeSpend += timeSpend;

        double accuracy = double(count) / readLength;

        int pos = int(accuracy * countLength);
        if (pos == countLength) pos--;
        for (; pos >= 0; pos--) {
            countNumberE[pos]++;
            countTimeSpend[pos] += timeSpend;
        }

        cout << setw(6) << i + 1 << setw(15) << readLength << setw(15) << threadCount << setw(15) << count << setw(15)
             << accuracy << setw(15) << timeSpend << endl;
    }

    ifs.close();

    cout << endl;
    cout << "运行时间：" << totalTimeSpend << "s" << endl;
    cout << setw(5) << "Range" << setw(15) << "countNumberE" << setw(15) << "countTimeSpend" << endl;
    for (int j = 1; j < countLength; j++) {
        char tmp[10];
        sprintf(tmp, ">=%d%%", countStep * j);
        cout << setw(5) << tmp << setw(15) << countNumberE[j] << setw(15) << countTimeSpend[j] << endl;
    }

    return 0;
}
