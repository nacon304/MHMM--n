#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <bitset>
#include <vector>
using namespace std;

const int MAX_BIT = 512;
typedef bitset<MAX_BIT> nBit;
typedef bitset<MAX_BIT * 2 + 1> nBit2;

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
    for (int i = 1; i < MAX_BIT * 2 + 1; i += 4)
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
nBit2 addBs(nBit2 &x, nBit2 &y)
{
    bool carry = 0;
    nBit2 res;
    for (int i = 0; i < MAX_BIT * 2 + 1; i++)
    {
        res[i] = (x[i] ^ y[i]) ^ carry;
        carry = (x[i] & y[i]) | (carry & (x[i] ^ y[i]));
    }
    return res;
}
nBit2 addBit(nBit2 &x, nBit2 &y)
{
    if (x == 0)
        return y;
    if (y == 0)
        return x;

    nBit2 res = addBs(x, y);
    return res;
}
nBit2 subBit(nBit2 x, nBit2 y)
{
    nBit2 _y = y.flip();
    for (int i = 0; i < MAX_BIT * 2 + 1; i++)
        if (_y[i] == 1)
            _y[i] = 0;
        else
        {
            _y[i] = 1;
            break;
        }

    nBit2 res = addBit(x, _y);
    return res;
}
nBit2 div2(nBit2 x)
{
    nBit2 res = x;
    if (x[MAX_BIT * 2] == 0)
        res >>= 1;
    else
    {
        res >>= 1;
        res[MAX_BIT * 2] = 1;

        if (x[0] == 1)
        {
            for (int i = 0; i < MAX_BIT * 2 + 1; i++)
                if (res[i] == 1)
                    res[i] = 0;
                else
                {
                    res[i] = 1;
                    break;
                }
        }
    }

    return res;
}
nBit2 mulBit(nBit2 &x, nBit2 &y)
{
    nBit2 res = 0;
    if (y == 0 || x == 0)
        return res;
    if (x == 1)
        return y;
    if (y == 1)
        return x;

    nBit2 temp = nBit2(x.to_string());
    for (int i = 0; i < MAX_BIT * 2 + 1; i++)
    {
        if (y[i] == 1)
            res = addBit(res, temp);
        temp <<= 1;
    }
    return res;
}
nBit2 phiBit(string p, string q)
{
    nBit2 res = 0;
    nBit2 pn = nBit2(p);
    nBit2 qn = nBit2(q);

    if (p.size() == 2 && p == "10")
        pn = nBit2(1);
    else
        pn = (pn ^ nBit2(1));

    if (q.size() == 2 && q == "10")
        qn = nBit2(1);
    else
        qn = (qn ^ nBit2(1));

    res = mulBit(pn, qn);

    return res;
}
void BezoutBit(nBit2 x, nBit2 y, nBit2 &a, nBit2 &b, nBit2 &g, nBit2 &mod)
{
    g = nBit2(1);

    while (x[0] == 0 && y[0] == 0)
    {
        x >>= 1;
        y >>= 1;
        g <<= 1;
    }

    nBit2 u, v, A, B, C, D;
    u = x;
    v = y;
    A = nBit2(1);
    B = 0;
    C = 0;
    D = nBit2(1);

    while (u != 0)
    {
        while (u[0] == 0)
        {
            u >>= 1;
            if (A[0] == 0 && B[0] == 0)
            {
                A = div2(A);
                B = div2(B);
            }
            else
            {
                A = div2(addBit(A, y));
                B = div2(subBit(B, x));
            }
        }
        while (v[0] == 0)
        {
            v >>= 1;
            if (C[0] == 0 && D[0] == 0)
            {
                C = div2(C);
                D = div2(D);
            }
            else
            {
                C = div2(addBit(C, y));
                D = div2(subBit(D, x));
            }
        }

        if (u.to_string() < v.to_string())
        {
            v = subBit(v, u);
            C = subBit(C, A);
            D = subBit(D, B);
        }
        else
        {
            u = subBit(u, v);
            A = subBit(A, C);
            B = subBit(B, D);
        }
    }
    a = C;
    b = D;
    g = mulBit(g, v);
}
nBit2 keyGen(string p, string q, string e, nBit2 &g)
{
    nBit2 pn = nBit2(p);
    nBit2 qn = nBit2(q);
    nBit2 ph = phiBit(p, q);
    nBit2 en = nBit2(e);

    nBit2 d, n_1;
    BezoutBit(en, ph, d, n_1, g, ph);
    cout << binToHex(ph.to_string()) << endl;
    cout << binToHex(d.to_string()) << endl;
    cout << binToHex(n_1.to_string()) << endl;

    while (d[MAX_BIT * 2] == 1)
        d = addBit(d, ph);

    return d;
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

    string p, q;
    string e;
    string pbn, qbn;
    string ebn;

    inp >> p >> q >> e;

    pbn = hexToBin(p);
    qbn = hexToBin(q);
    ebn = hexToBin(e);

    nBit2 g;
    nBit2 d = keyGen(pbn, qbn, ebn, g);

    if (g == nBit2(1))
    {
        string res = binToHex(d.to_string());
        out << res;
    }
    else
    {
        out << -1;
    }

    inp.close();
    out.close();
    return 0;
}
