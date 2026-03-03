#include <iostream>
#include <fstream>
#include <string>
#include <random>
using namespace std;
const long long p = 100003;
const long long g = 2;
random_device rd;
mt19937 gen(rd());

void clrscr()
{
    cout << "\033[2J\033[1;1H";
    return;
}

string toCapital(string s)
{
    for (char &c : s)
    {
        if (c >= 'a' && c <= 'z')
        {
            c = c - 'a' + 'A';
        }
    }
    return s;
}

string encode(long long x)
{
    string code = "";
    for (int i = 0; i < 3; i++)
    {
        int temp = x % 52;
        x = x / 52;
        char c;
        if (temp < 26)
            c = 65 + temp;
        else
            c = 97 + temp - 26;
        code = c + code;
    }
    return code;
}

long long decode(string s)
{
    long long x = 0;
    for (char m : s)
    {
        int temp;
        if (m < 'a')
            temp = m - 65;
        else
            temp = m - 97 + 26;
        x = x * 52 + temp;
    }
    return x;
}

long long power(long long base, long long exp, long long mod)
{
    long long res = 1;
    base %= mod;
    while (exp > 0)
    {
        if (exp % 2 == 1)
        {
            res = (res * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return res;
}

string encrypt(string text, long long key)
{
    string ciphertext = "";
    uniform_int_distribution<long long> distrib(1, p - 2);
    for (char m : text)
    {
        long long k = distrib(gen);
        long long c1 = power(g, k, p);
        long long s = power(key, k, p);
        long long c2 = (m * s) % p;
        ciphertext += encode(c1);
        ciphertext += encode(c2);
    }
    return ciphertext;
}

string decrypt(string text, long long key)
{
    string plaintext = "";
    for (size_t i = 0; i + 5 < text.length(); i += 6)
    {
        long long c1 = decode(text.substr(i, 3));
        long long c2 = decode(text.substr(i + 3, 3));
        long long s = power(c1, key, p);
        long long sInv = power(s, p - 2, p);
        char m = (c2 * sInv) % p;
        plaintext += m;
    }
    return plaintext;
}

int main()
{
    int choice = 0;
    while (choice != 4)
    {
        clrscr();
        cout << "1. Encrypt a file" << endl;
        cout << "2. Decrypt a file" << endl;
        cout << "3. Issue a public key" << endl;
        cout << "4. Quit" << endl;
        cout << "Input your choice: ";
        cin >> choice;
        if (choice == 4)
            break;
        if (choice < 1 || choice > 4)
        {
            cout << "Invalid choice." << endl;
            continue;
        }
        long long key;
        cout << "Key number (smaller than " << p << "): ";
        cin >> key;
        if (choice == 1)
        {
            string file, ofile;
            cout << "Please input the input file name: ";
            cin >> file;
            cout << "Please input the output file name: ";
            cin >> ofile;
            ifstream fin(file);
            ofstream fout(ofile);
            string line;
            while (getline(fin, line))
            {
                fout << encrypt(line, key) << '\n';
            }
            cout << "Encryption finished. Output saved to " << ofile << endl;
            fin.close();
            fout.close();
        }
        else if (choice == 2)
        {
            string file, ofile;
            cout << "Please input the input file name: ";
            cin >> file;
            cout << "Please input the output file name: ";
            cin >> ofile;
            ifstream fin(file);
            ofstream fout(ofile);
            string line;
            while (getline(fin, line))
            {
                fout << decrypt(line, key) << '\n';
            }
            cout << "Decryption finished. Output saved to " << ofile << endl;
            fin.close();
            fout.close();
        }
        else if (choice == 3)
        {
            cout << "Public key: " << power(g, key, p) << endl;
        }
    }
    return 0;
}
