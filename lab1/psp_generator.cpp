#include <cstdint>
#include <iostream>
#include <bitset>

uint64_t bit;

//K1(x)=x^35+x^2+1
int n = 35;
int i;
int counter;

uint64_t Rlz1 = 0xD4B; //0000110101001011
uint64_t feedback_bit_1;

//K2(x)=x^135+x^22+1 точки съема 135 и 22
uint64_t feedback_bit_2;

std::bitset<135> Rlz2(0xABFCBBAEA); // старшие биты
std::bitset<135> Rlz2_1(0xAAFEBBECFEBAAFCA); //младшие биты

int main() {
    std::cout << "Количество бит: ";
    std::cin >> counter;

    Rlz2 = Rlz2<<64;
    Rlz2 = Rlz2|Rlz2_1;

    for (i = 0; i < counter; i++) {
        feedback_bit_1 = (Rlz1&0x1)^((Rlz1>>2)&0x1);
        Rlz1 = (Rlz1>>1)|(feedback_bit_1<<n);

        feedback_bit_2 = Rlz2[0]^Rlz2[21];
        Rlz2 = (Rlz2>>1);
        Rlz2.set(134, feedback_bit_2);

        bit = ((feedback_bit_1)^(feedback_bit_2));
        std::cout << bit;
    }
}
