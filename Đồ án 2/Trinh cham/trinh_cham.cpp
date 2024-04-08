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
    int nTest = 10;
    for (int i = 0; i < nTest; ++i)
    {
        stringstream inp, ans, out;
        inp << "test_" << setw(2) << setfill('0') << i << ".inp";
        ans << "test_" << setw(2) << setfill('0') << i << ".ans";
        out << "test_" << setw(2) << setfill('0') << i << ".out";

        auto start = std::chrono::high_resolution_clock::now();
        int exitCode = system(("main.exe " + inp.str() + " " + ans.str()).c_str());
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        if (compareFiles(out.str(), ans.str()))
        {
            cout << "Test " << i << " passed: Corrert!." << endl;
            cout << "Time: " << duration.count() << "s" << endl;
        }
        else
        {
            cout << "Test " << i << " failed: Incorrect!." << endl;
            cout << "Time: " << duration.count() << "s" << endl;
        }

        // if (exitCode == 0) {
        //     cout << "Test " << i << " passed." << endl;
        // } else {
        //     cerr << "Test " << i << " failed." << endl;
        // }
    }
}