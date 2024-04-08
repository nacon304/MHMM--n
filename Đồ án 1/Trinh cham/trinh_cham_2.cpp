#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <chrono>
using namespace std;

bool compareFiles(const string &file1, const string &file2)
{
    ifstream f1(file1), f2(file2);
    string line1, line2;

    if (!f1.is_open() || !f2.is_open())
    {
        cerr << "Error opening files." << endl;
        return false;
    }

    while (getline(f1, line1) && getline(f2, line2))
    {
        if (line1 != line2)
        {
            return false;
        }
    }

    if (getline(f1, line1) || getline(f2, line2))
    {
        return false;
    }

    return true;
}

int main()
{
    int nTest = 20;
    for (int i = 10; i < nTest; ++i)
    {
        stringstream inp, out_1, out_2;
        inp << "test_" << setw(2) << setfill('0') << i << ".inp";
        out_1 << "test_" << setw(2) << setfill('0') << i << ".out_1";
        out_2 << "test_" << setw(2) << setfill('0') << i << ".out_2";

        auto start1 = std::chrono::high_resolution_clock::now();
        int exitCode_1 = system(("main_1.exe " + inp.str() + " " + out_1.str()).c_str());
        auto end1 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration1 = end1 - start1;

        auto start2 = std::chrono::high_resolution_clock::now();
        int exitCode_2 = system(("main_2.exe " + inp.str() + " " + out_2.str()).c_str());
        auto end2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration2 = end2 - start2;

        if (compareFiles(out_1.str(), out_2.str()))
        {
            cout << "Test " << i << " passed: Corrert!." << endl;
            cout << "Time person 1: " << duration1.count() << "s" << endl;
            cout << "Time person 2: " << duration2.count() << "s" << endl;
        }
        else
        {
            cout << "Test " << i << " failed: Incorrect!." << endl;
            cout << "Time person 1: " << duration1.count() << "s" << endl;
            cout << "Time person 2: " << duration2.count() << "s" << endl;
        }

        // if (exitCode == 0) {
        //     cout << "Test " << i << " passed." << endl;
        // } else {
        //     cerr << "Test " << i << " failed." << endl;
        // }
    }
}