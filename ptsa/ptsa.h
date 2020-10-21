#ifndef SEQUENCE_ALIGNMENT_PTSA_H
#define SEQUENCE_ALIGNMENT_PTSA_H

#include <string>

int ptsa(int readLength, std::string &refSeq, std::string &readSeq, int E, int gridSize, int F, int threadCount,
         int stepValue);

#endif //SEQUENCE_ALIGNMENT_PTSA_H
