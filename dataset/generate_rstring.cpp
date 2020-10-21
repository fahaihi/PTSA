#include <iostream>
#include <fstream>
#include <vector>

#define SEQ_NUMBER 1000 // 字符串组数
#define SEQ_LENGTH 300000 // 想生成字符串长度
#define OUTPUT_FILENAME "300000.txt"

using namespace std;

char randomChar()
{
	char metachar[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	return metachar[rand() % (sizeof(metachar) - 1)];
}

void  randomSeq(vector<char> &buff)
{
	for (int i = 0; i < buff.size(); i++)
	{
		buff[i] = randomChar();
	}
}

void randomSeqPair(vector<char> &buff1, vector<char> &buff2, int length, int diff)
{
	randomSeq(buff1);
	buff2.assign(buff1.begin(), buff1.end());

	for (int i = 0; i < diff; i++)
	{
		if (rand() % 2 == 0)
		{
			buff2[rand() % length] = randomChar();
		}
		else
		{
			int shiftIndex = rand() % (length - 5);
			int shiftLength = rand() % 5 + 1;
			for (int j = 0; j < shiftLength; j++)
			{
				buff2.push_back(buff2[shiftIndex + j]);
				buff2.erase(buff2.begin() + shiftIndex + j);
			}
		}
	}
}

void printVector(vector<char> &vec, ostream &out)
{
	for (auto iter = vec.begin(); iter != vec.end(); iter++)
	{
		out << (char)*iter;
	}
	out << endl;
}

int main() {
	int number = SEQ_NUMBER / 10;
	vector<char> buff1(SEQ_LENGTH), buff2(SEQ_LENGTH);

	ofstream ofs; // 创建流对象用于写文件
	ofs.open(OUTPUT_FILENAME, ios::out); // 以追加的方式打开

	for (int i = 0; i < 10; i++)
	{
		// 生成十分之一相似度极低的数据
		for (int j = 0; j < number; j++)
		{
			if (i != 9)
			{
				randomSeqPair(buff1, buff2, SEQ_LENGTH, 0.05 * i * SEQ_LENGTH);
			}
			else
			{
				randomSeq(buff1);
				randomSeq(buff2);
			}

			//ofs << str1 << endl << str2 << endl;
			printVector(buff1, ofs);
			printVector(buff2, ofs);
		}
	}

	ofs.close();

	return 0;
}
