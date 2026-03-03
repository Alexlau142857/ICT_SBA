#include <iostream>
#include <ctime>
#include <fstream>
using namespace std;

long long p = 100003;
long long g = 2;

void clrscr() {
    cout << "\033[2J\033[1;1H";
    return;
}

string toCapital(string s) {
    for (char &c : s) {
        if (c >= 'a' && c <= 'z') {
            c = c - 'a' + 'A';
        }
    }
    return s;
}

string encode(long long x) {
    string code = "";
    for (int i = 0; i < 3; i++) {
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

long long decode(string s) {
    long long x = 0;
    for (char m : s) {
        int temp;
        if (m < 'a')
            temp = m - 65;
        else
            temp = m - 97 + 26;
        x = x * 52 + temp;
    }
    return x;
}

// Modular exponentiation to handle large powers
long long power(long long base, long long exp, long long mod) {
    long long res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (__int128(res) * base) % mod;
        base = (__int128(base) * base) % mod;
        exp /= 2;
    }
    return res;
}

// Subprogram: Encrypt Text
string encrypt(string text, long long key) {
    string ciphertext = "";
    for (char m : text) {
        long long k = rand() % (p - 2) + 1;
        long long c1 = power(g, k, p);
        long long s = power(key, k, p); // Shared secret
        long long c2 = (m * s) % p;
        ciphertext = ciphertext + encode(c1);
        ciphertext = ciphertext + encode(c2);
    }
    return ciphertext;
}

// Subprogram: Decrypt Text
string decrypt(string text, long long key) {
    string plaintext = "";
    string temp = "";
    for (char v : text) {
        temp = temp + v;
        if (temp.length() < 6)
            continue;
        long long c1 = decode(temp.substr(0,3));
        long long c2 = decode(temp.substr(3,3));
        temp = "";
        long long s = power(c1, key, p);
        long long sInv = power(s, p - 2, p);
        char m = (c2 * sInv) % p;
        plaintext = plaintext + m;
    }
    return plaintext;
}

int main() {
    srand(time(0));
    int choice = 0;
    while (choice < 4) {
        clrscr();
        string line; 
    	cout << "Please choose from the following:" << endl;
        cout << "1. Encrypt a file" << endl;
        cout << "2. Decrypt a file" << endl;
        cout << "3. Issue a public key" << endl;
        cout << "4. Quit" << endl;
        cout << "Input your choice: ";
        cin >> choice;
        getline(cin, line);
        cout << endl;
        if (choice == 4)
            break;
        long long key;
        cout << "Key number (smaller than " << p << "): ";
        cin >> key;
        getline(cin, line);
        if (choice < 3) {    
            // your code here
            if (choice == 1){
                cout << "Please input your input file name that you want to encrypt\n";
                string file;
                cin >> file;
                ifstream fin;
                fin.open(file);
                cout << "Please input your output file name after encrypt\n";
                string ofile;
                cin >> ofile;
                ofstream fout;
                fout.open(ofile);
                cout << "Please input the private / public key (smaller than 10^9+7)\n";
                int key = 0;
                cin >> key;
                cout << "Please input your input file name that you want to encrypt\n";
                string line;
                while (getline(fin, line)){
                    fout << encrypt(file, key) << '\n';
                }
                cout << "The encrypted result had been output to the file named encrypt_out.txt";
                fin.close();
                fout.close();
            } else {
                cout << "Please input your input file name that you want to decrypt\n";
                string file;
                cin >> file;
                ifstream fin;
                fin.open(file);
                cout << "Please input your output file name after encrypt\n";
                string ofile;
                cin >> ofile;
                ofstream fout;
                fout.open(ofile);
                cout << "Please input the private / public key (smaller than 10^9+7)\n";
                int key = 0;
                cin >> key;
                string line;
                while (getline(fin, line)){
                    fout << decrypt(file, key) << '\n';
                }
                cout << "The decrypted result had been output to the file named decrypt_out.txt";
                fin.close();
                fout.close();
            }
        } else{
            cout << "Public key: " << power(g, key, p) << endl;
        }
        cout << "Press <Enter> to continue.";
        getline(cin, line);
    }
    clrscr();
    return 0;
}