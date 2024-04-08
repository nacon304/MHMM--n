#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <bitset>
#include <vector>
using namespace std;

const int MAX_BIT = 1024;
typedef bitset<MAX_BIT> nBit;

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
string binToHex(string a)
{
    bool flag1 = false;
    string res = "";
    for (int i = 0; i < MAX_BIT; i += 4)
    {
        string sub = a.substr(i, 4);
        bitset<4> tam(sub);
        int decimal = tam.to_ulong();

        if (decimal)
            flag1 = true;
        if (!flag1)
            continue;

        if (decimal < 10)
            res = char(decimal + '0') + res;
        else
            res = char(decimal - 10 + 'A') + res;
    }
    return res;
}
bitset<MAX_BIT + 1> addBs(nBit &x, nBit &y)
{
    bool carry = 0;
    int x_1 = x.count();
    int y_1 = y.count();
    bitset<MAX_BIT + 1> res;
    for (int i = 0; i < MAX_BIT; i++)
    {
        res[i] = (x[i] ^ y[i]) ^ carry;
        carry = (x[i] & y[i]) | (carry & (x[i] ^ y[i]));

        x_1 -= x[i];
        y_1 -= y[i];

        if (carry == 0 && x_1 == 0 && y_1 == 0)
            break;
    }
    res[MAX_BIT] = carry;
    return res;
}
bool subBit(bool b1, bool b2, bool &borrow)
{
    bool diff;
    if (borrow)
    {
        diff = !(b1 ^ b2);
        borrow = !b1 || (b1 && b2);
    }
    else
    {
        diff = b1 ^ b2;
        borrow = !b1 && b2;
    }
    return diff;
}
bitset<MAX_BIT + 1> subBs(bitset<MAX_BIT + 1> &x, nBit &mod)
{
    bool borrow = 0;
    bitset<MAX_BIT + 1> res;
    for (int i = 0; i < MAX_BIT; i++)
    {
        res[i] = subBit(x[i], mod[i], borrow);
    }

    if (borrow)
        res[MAX_BIT] = 0;
    else
        res[MAX_BIT] = x[MAX_BIT];
    return res;
}
nBit modBs(bitset<MAX_BIT + 1> x, nBit &mod)
{
    while (x[MAX_BIT] == 1 || x.to_string().substr(1, MAX_BIT) >= mod.to_string())
        x = subBs(x, mod);
    string temp = x.to_string().substr(1, MAX_BIT);
    nBit res = nBit(temp);
    return res;
}
nBit addMod(nBit &x, nBit &y, nBit &mod)
{
    if (x == 0)
        return modBs(bitset<MAX_BIT + 1>(y.to_string()), mod);
    if (y == 0)
        return modBs(bitset<MAX_BIT + 1>(x.to_string()), mod);

    bitset<MAX_BIT + 1> res = addBs(x, y);
    return modBs(res, mod);
}
nBit mulMod(nBit &x, nBit &y, nBit &mod)
{
    nBit res = nBit(0);
    if (y == 0 || x == 0)
        return res;
    if (x == 1)
        return modBs(bitset<MAX_BIT + 1>(y.to_string()), mod);
    if (y == 1)
        return modBs(bitset<MAX_BIT + 1>(x.to_string()), mod);

    nBit temp = x;
    int y_1 = y.count();
    for (int i = 0; i < MAX_BIT; i++)
    {
        if (y[i] == 1)
            res = addMod(res, temp, mod), y_1--;
        if (y_1 == 0)
            break;

        bitset<MAX_BIT + 1> tt = bitset<MAX_BIT + 1>(temp.to_string());
        tt <<= 1;
        temp = modBs(tt, mod);
    }
    return res;
}
nBit powerMod(nBit &x, nBit &y, nBit &mod)
{
    nBit res = nBit(1);
    if (y == 0)
        return res;
    if (y == 1)
        return modBs(bitset<MAX_BIT + 1>(x.to_string()), mod);
    for (int i = MAX_BIT - 1; i >= 0; i--)
    {
        if (res != nBit(1))
            res = mulMod(res, res, mod);
        if (y[i] == 1)
            res = mulMod(res, x, mod);
    }
    return res;
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

    string n;
    string x, k;
    string nbn;
    string xbn, kbn;
    nBit nbs;
    nBit xbs, kbs;

    inp >> n >> k >> x;

    nbn = hexToBin(n);
    xbn = hexToBin(x);
    kbn = hexToBin(k);

    nbs = nBit(nbn);
    xbs = nBit(xbn);
    kbs = nBit(kbn);

    nBit res = powerMod(xbs, kbs, nbs);
    out << binToHex(res.to_string());

    inp.close();
    out.close();
    return 0;
}
