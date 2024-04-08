#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <bitset>

using namespace std;
#define ll long long

const int MAX_BIT = 512;
const int MAX_ADD = 60;
const int LONG_LONG_BITS = 61;

std::chrono::time_point<std::chrono::high_resolution_clock> start;
std::chrono::duration<double> duration;

int resPrime = 0;
string hexToBin(string a)
{
    string res;
    for (char c : a)
    {
        int decimal;
        if (c >= '0' && c <= '9')
            decimal = c - '0';
        else
            decimal = c - 'A' + 10;

        res = bitset<4>(decimal).to_string() + res;
    }
    return res;
}

void popZero(vector<ll> &x)
{
    while (x.size() && x[x.size() - 1] == 0)
        x.pop_back();
}

vector<ll> hexToLongLong(const string &a)
{
    vector<ll> res;
    int len = a.length();
    for (int i = len - 1; i >= 0; i -= LONG_LONG_BITS)
    {
        ll value = 0;
        int bits = min(LONG_LONG_BITS, i + 1);
        for (int j = 0; j < bits; ++j)
        {
            int index = i - j;
            value |= (static_cast<ll>(a[index] - '0') << j);
        }
        res.push_back(value);
    }
    popZero(res);
    return res;
}

vector<ll> primeLongLong()
{
    int pr[168] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997};
    vector<ll> res;
    for (int i = 0; i < 168; i++)
        res.push_back(pr[i]);
    return res;
}

bool checkTime()
{
    auto end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    return duration.count() > 59;
}

bool checkAns()
{
    return resPrime >= 3;
}
void print(const vector<ll> &x)
{
    for (ll v : x)
        cout << v << endl;
}

vector<ll> addLongLong(const vector<ll> &x, const vector<ll> &y)
{
    vector<ll> res;
    ll carry = 0;
    for (size_t i = 0; i < max(x.size(), y.size()) || carry; ++i)
    {
        ll sum = carry;
        if (i < x.size())
            sum += x[i];
        if (i < y.size())
            sum += y[i];
        res.push_back(sum & ((1ll << LONG_LONG_BITS) - 1));
        carry = sum >> LONG_LONG_BITS;
    }
    return res;
}
vector<ll> subLongLong(const vector<ll> &x, const vector<ll> &y)
{
    vector<ll> res;
    ll borrow = 0;
    for (size_t i = 0; i < x.size(); ++i)
    {
        ll diff = x[i] - borrow;
        borrow = 0;
        if (i < y.size())
            diff -= y[i];
        if (diff < 0)
            diff += (1ll << LONG_LONG_BITS), borrow = 1;
        res.push_back(diff);
    }
    popZero(res);
    return res;
}

int compare(const vector<ll> &x, const vector<ll> &y)
{
    if (x.size() > y.size())
        return 1;
    if (x.size() < y.size())
        return -1;
    for (int i = x.size() - 1; i >= 0; i--)
    {
        if (x[i] > y[i])
            return 1;
        if (x[i] < y[i])
            return -1;
    }
    return 0;
}

vector<ll> modLongLong(const vector<ll> &x, const vector<ll> &mod)
{
    vector<ll> temp = x;
    while (compare(temp, mod) >= 0)
    {
        if (checkTime())
            return vector<ll>();
        temp = subLongLong(temp, mod);
    }
    return temp;
}

vector<ll> addModLongLong(const vector<ll> &x, const vector<ll> &y, const vector<ll> &mod)
{
    if (x.empty())
        return modLongLong(y, mod);
    if (y.empty())
        return modLongLong(x, mod);

    vector<ll> res = addLongLong(x, y);
    return modLongLong(res, mod);
}

vector<ll> mulModLongLong(const vector<ll> &x, const vector<ll> &y, const vector<ll> &mod)
{
    vector<ll> res;
    if (y.empty() || x.empty())
        return res;

    vector<ll> temp = x;
    for (size_t i = 0; i < y.size() * LONG_LONG_BITS; ++i)
    {
        if (checkTime())
            return vector<ll>();
        if (y[i / LONG_LONG_BITS] & (1ll << (i % LONG_LONG_BITS)))
        {
            res = addModLongLong(res, temp, mod);
        }
        temp = addModLongLong(temp, temp, mod);
    }
    return res;
}

vector<ll> powerModLongLong(const vector<ll> &x, const vector<ll> &y, const vector<ll> &mod)
{
    vector<ll> res = {1};
    if (y.empty())
        return res;
    for (int i = y.size() * LONG_LONG_BITS - 1; i >= 0; --i)
    {
        if (checkTime())
            return vector<ll>();
        if (res.size() != 1 || res[0] != 1)
        {
            res = mulModLongLong(res, res, mod);
        }
        if (y[i / LONG_LONG_BITS] & (1ll << (i % LONG_LONG_BITS)))
        {
            res = mulModLongLong(res, x, mod);
        }
    }
    return res;
}

bool checkPrime(const vector<ll> &s, const vector<ll> &p, vector<ll> m, int r)
{
    vector<ll> fbs = subLongLong(s, {1});
    if (s == vector<ll>{1})
        return false;
    if (s == vector<ll>{2})
        return true;

    vector<int> kt(200, 0);
    if (s.size() == 1)
    {
        for (size_t i = 0; i < p.size(); i++)
            if (p[i] == s[0])
                return true;
    }

    for (int i = 0; i < 20; i++)
    {
        int randPrime = std::rand() % 100;
        while (kt[randPrime] == 1)
            randPrime = std::rand() % 100;

        vector<ll> p_i = {p[randPrime]};
        //        vector<ll> p_i = {3};
        kt[randPrime] = 1;

        vector<ll> temp = fbs;

        vector<ll> ans = powerModLongLong(p_i, m, s);

        if (checkTime())
            return checkAns();

        if (ans == vector<ll>{1} || ans == fbs)
        {
            ans = mulModLongLong(ans, ans, s);
            if (ans == vector<ll>{1})
            {
                resPrime++;
                continue;
            }
            else
                return false;
        }

        bool check = false;
        for (int i = 1; i <= r; i++)
        {
            ans = mulModLongLong(ans, ans, s);
            if (ans == fbs && i != r)
            {
                resPrime += 2;
                check = true;
                break;
            }
            if (checkTime())
                return checkAns();
        }
        if (!check)
            return false;
    }
    return true;
}
vector<ll> find_m_r(string s, int &r)
{
    s[s.size() - 1] = 0;
    int i;
    for (i = s.size() - 1; i >= 0; i--)
    {
        if (s[i] == '1')
            break;
        r++;
    }
    s = s.substr(0, i + 1);
    vector<ll> m = hexToLongLong(s);
    return m;
}
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " input_file output_file" << endl;
        return 1;
    }

    ifstream inp(argv[1]);
    if (!inp.is_open())
    {
        cerr << "Could not open input file: " << argv[1] << endl;
        return 1;
    }
    ofstream out(argv[2]);
    if (!out.is_open())
    {
        cerr << "Could not open output file: " << argv[2] << endl;
        return 1;
    }
    std::srand(std::time(0));

    start = std::chrono::high_resolution_clock::now();
    string s;
    string sbn;
    vector<ll> p_bs;

    inp >> s;

    sbn = hexToBin(s);
    int r = 0;
    vector<ll> m = find_m_r(sbn, r);

    p_bs = primeLongLong();

    out << checkPrime(hexToLongLong(sbn), p_bs, m, r);

    inp.close();
    out.close();
    return 0;
}
