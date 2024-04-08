#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <bitset>

#define nhat "bai1"

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

vector<ll> binToLongLong(const string &a)
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

int msbPosition(ll x)
{
    int res = 0;
    while (x > 0)
    {
        x >>= 1;
        res++;
    }
    return res;
}

ll getFirstKBits(ll x, int k)
{
    ll mask = (1LL << k) - 1;
    ll res = x & mask;

    return res;
}

vector<ll> leftShift(const vector<ll> &x, int shift)
{
    if (shift == 0)
    {
        return x;
    }

    int shiftUnits = shift / LONG_LONG_BITS;
    int shiftBits = shift % LONG_LONG_BITS;
    int shiftNe = LONG_LONG_BITS - shiftBits;

    vector<ll> res(shiftUnits + 1, 0);
    for (ll v : x)
    {
        ll temp = getFirstKBits(v, shiftNe);
        res[res.size() - 1] |= (temp << shiftBits);

        temp = (v >> shiftNe);
        res.push_back(temp);
    }

    popZero(res);

    return res;
}

vector<ll> divLongLong(const vector<ll> &x, const vector<ll> &y)
{
    if (y.size() == 1 && y[0] == 0)
        return vector<ll>();

    vector<ll> q, r = x;
    int shift = MAX_BIT + 1;

    while (compare(r, y) >= 0)
    {
        if (checkTime())
            return vector<ll>();

        int msbR = msbPosition(r.back());
        int msbY = msbPosition(y.back());
        int cShift = msbR - msbY + (int)(r.size() - y.size()) * LONG_LONG_BITS;
        shift = min(shift - 1, cShift);

        vector<ll> shiftedY = leftShift(y, shift);
        while (compare(r, shiftedY) < 0)
            shift--, shiftedY = leftShift(y, shift);

        r = subLongLong(r, shiftedY);
        q = addLongLong(q, leftShift(vector<ll>{1}, shift));
    }

    return q;
}

bool check_primitive_root_mod_p(vector<ll> &p, vector<ll> &g, vector<vector<ll>> &U)
{
    vector<ll> p_1 = p;
    p_1[0]--;

    for (vector<ll> k : U)
    {
        vector<ll> tempInd = divLongLong(p_1, k);
        if (powerModLongLong(g, tempInd, p) == vector<ll>{1})
            return 0;
    }

    return 1;
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

    string p;
    string n;
    vector<vector<ll>> U;
    string g;

    inp >> p;
    string pn = hexToBin(p);
    vector<ll> pv = binToLongLong(pn);

    inp >> n;
    string nn = hexToBin(n);
    vector<ll> nv = binToLongLong(nn);

    for (int i = 0; i < nv[0]; i++)
    {
        string s;
        inp >> s;
        string sn = hexToBin(s);
        vector<ll> sv = binToLongLong(sn);
        U.push_back(sv);
    }

    inp >> g;
    string gn = hexToBin(g);
    vector<ll> gv = binToLongLong(gn);

    out << check_primitive_root_mod_p(pv, gv, U);

    inp.close();
    out.close();

    return 0;
}
