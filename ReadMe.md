![](https://github.com/fahaihi/PTSA/blob/master/ptsa.PNG)

# **A parallel algorithm for computing text similarity with low time and storage overhead**


Text similarity calculation has a wide range of applications of text editing, personalized recommendation,
biological information processing and other fields. There are many limitations on text similarity algorithms through
dynamic programming or hardware acceleration. This paper designs and implements a parallel algorithm of text
similarity with low time and storage overhead. This algorithm divides the logical edit distance matrix into multiple
matrix sub-blocks, and allocates these matrix sub-blocks to multiple threads for parallel processing to speed up the
calculation of the edit distance element value; a sliding window strategy is used to dynamically store the logic for
each matrix sub-block Editing distance matrix elements, with significant storage space overhead required by the
algorithm; by counting the eigenvalues of all non-overlapping common subsequences in the matrix to obtain the edit
distance value between the sequences, so as to realize the parallel calculation of the similarity of two text strings.
This algorithm is applied to the filtering scenario. The experimental results show that in the case of linear space
complexity, this algorithm can greatly reduce the computing time, and the maximum speedup ratio of 64 threads can
reach 63X, while maintaining high comparison accuracy, higher filtering accuracy and lower filtering false positive
rate, and this algorithm performs better for long texts with high similarity

**more info： https://github.com/fahaihi/PTSA/wiki/USer-Guide**
