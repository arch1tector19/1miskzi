#include <iostream>
#include <bitset>
#include <cstdint>
#include <fstream>
#include <vector>

using namespace std;

uint8_t rlz1_step(uint64_t &reg)
{
    uint8_t bit = (reg & 0x1) ^ ((reg >> 33) & 0x1);
    reg = (reg >> 1) | (uint64_t(bit) << 34);
    return bit;
}

uint8_t rlz2_step(bitset<135> &reg)
{
    uint8_t bit = reg[0] ^ reg.test(113);
    reg >>= 1;
    reg.set(134, bit);
    return bit;
}

int main()
{
    int L;
    cout << "Введите количество бит: ";
    cin >> L;

    uint64_t R1 = 0x1ABCDEF;
    bitset<135> R2(0x1ABCDEF1234567ULL);

    int zeros = 0, ones = 0;
    cout << "\nПреобразованная последовательность:\n";

    for (int i = 0; i < L; ++i)
    {
        uint8_t b1 = rlz1_step(R1);
        uint8_t b2 = rlz2_step(R2);
        uint8_t gamma = b1 ^ b2;

        cout << int(gamma);

        if (gamma) ones++;
        else zeros++;

        if ((i + 1) % 64 == 0)
            cout << endl;
    }

    cout << "\nКол-во 0 и 1:\n";
    cout << "0: " << zeros << endl;
    cout << "1: " << ones << endl;

    string filename;
    cout << "\nИмя файла для маскировки: ";
    cin >> filename;

    ifstream infile(filename, ios::binary);
    if (!infile)
    {
        cout << "\nOшибка открытия файла\n";
        return 1;
    }

    vector<uint8_t> data(
        (istreambuf_iterator<char>(infile)),
        istreambuf_iterator<char>()
    );
    infile.close();

    for (size_t i = 0; i < data.size(); ++i)
    {
        uint8_t gamma_byte = 0;
        for (int j = 0; j < 8; ++j)
        {
            uint8_t bit = rlz1_step(R1) ^ rlz2_step(R2);
            gamma_byte = (gamma_byte << 1) | bit;
        }
        data[i] ^= gamma_byte;
    }
	
    ofstream outfile(filename, ios::binary | ios::trunc);
    outfile.write(reinterpret_cast<char*>(data.data()), data.size());
    outfile.close();

    cout << "\nФайл успешно замаскирован\n";
    return 0;
}
