#include "task1.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cstdlib>

using namespace std;

void convertFile(const char* filename)
{
    char outFilename[80] = {0};
    strcat(outFilename,"_");
    strcat(outFilename,filename);
    std::filebuf outFile;
    std::filebuf inFile;
    if(!outFile.open (outFilename,std::ios::out)) return;
    std::ostream os(&outFile);
    if (inFile.open (filename, std::ios::in))
    {
        std::istream is(&inFile);
        char ch;
        while (is)
        {
            ch=char(is.get());
            os << char(toupper(ch));
        }
        inFile.close();
    }
}

void fileConvertion(const char **filenameArray, const unsigned int filesCount, const unsigned int threadCount)
{
    std::vector<thread> v;
    v.reserve(threadCount);

    unsigned int iterCount = filesCount / threadCount;
    unsigned int iterRest;

    if(filesCount < threadCount)
    {
        iterRest = 0;
    }
    else
    {
        iterRest = filesCount % threadCount;
    }

    unsigned int fileNum = 0;

    for(unsigned int it = 0; it<iterCount; it++)
    {
        for(unsigned int i = 0; i < threadCount; i++)
        {
            v.emplace_back(convertFile,filenameArray[fileNum++]);
        }
        for(auto& v_it : v)
        {
            v_it.join();
        }
        v.clear();
    }

    for(unsigned int i = 0; i < iterRest; i++)
    {
        v.emplace_back(convertFile,filenameArray[fileNum++]);
    }
    for(auto& v_it : v)
    {
        v_it.join();
    }
}

void showTime(chrono::high_resolution_clock::time_point start, chrono::high_resolution_clock::time_point stop)
{
    chrono::duration<double> time_elapsed = stop - start;
    cout << chrono::duration_cast<chrono::milliseconds>(time_elapsed).count() << " ms" << endl;
}

void Task1()
{
    const char *filespec[] = {
        "01.txt",
        "02.txt",
        "03.txt",
        "04.txt",
        "05.txt",
        "06.txt",
        "07.txt",
        "08.txt",
        "09.txt",
        "10.txt",
        "11.txt",
        "12.txt",
        "13.txt",
        "14.txt",
        "15.txt",
        "16.txt"};
    chrono::high_resolution_clock::time_point start, stop;

    start = chrono::high_resolution_clock::now();
    fileConvertion(filespec,16,1);
    stop = chrono::high_resolution_clock::now();
    cout << "Sequentially, in one thread: ";
    showTime(start,stop);

    start = chrono::high_resolution_clock::now();
    fileConvertion(filespec,16,16);
    stop = chrono::high_resolution_clock::now();
    cout << "Thread count is equal to files count: ";
    showTime(start,stop);

    start = chrono::high_resolution_clock::now();
    fileConvertion(filespec,16,std::thread::hardware_concurrency());
    stop = chrono::high_resolution_clock::now();
    cout << "Thread count is equal to cores in system: ";
    showTime(start,stop);
}
