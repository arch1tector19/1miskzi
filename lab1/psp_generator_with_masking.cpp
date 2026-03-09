#include <cstdint>
#include <iostream>
#include <bitset>
#include <fstream>
#include <string>

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
    std::string input_file_name;
    std::string output_file_name;

    std::cout << "Введите имя входного файла: ";
    std::getline(std::cin >> std::ws, input_file_name); // std::ws удалаяет пробелы передчтением строки 

    std::cout << "Введите имя выходного файла: ";
    std::getline(std::cin >> std::ws, output_file_name);

    std::ifstream input_file(input_file_name, std::ios::binary); // открытие файла для чтения
    if (!input_file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть входной файл\n";
        return 1;
    }

    std::ofstream output_file(output_file_name, std::ios::binary); // открытие файла для записи
    if (!output_file.is_open()) { // если не открылся - ошибка!
        std::cerr << "Ошибка: не удалось создать выходной файл\n";
        return 1;
    }

    Rlz2 = Rlz2 << 64;
    Rlz2 = Rlz2 | Rlz2_1;

    char input_byte;
    uint8_t gamma_byte;

    while (input_file.get(input_byte)) { // чтение по одному байту из выходного файла
        gamma_byte = 0; // обнуляем байт гаммы

        for (i = 0; i < 8; i++) {
            feedback_bit_1 = (Rlz1 & 0x1) ^ ((Rlz1 >> 2) & 0x1);
            Rlz1 = (Rlz1 >> 1) | (feedback_bit_1 << n); // бит вставаляется в позицию i, то есть старшего разряда

            feedback_bit_2 = Rlz2[0] ^ Rlz2[21]; // xor
            Rlz2 = (Rlz2 >> 1);
            Rlz2.set(134, feedback_bit_2);

            bit = (feedback_bit_1) ^ (feedback_bit_2);
            gamma_byte = gamma_byte | (static_cast<uint8_t>(bit) << i);
        }

        output_file.put(static_cast<char>(static_cast<uint8_t>(input_byte) ^ gamma_byte));
    }

    if (!input_file.eof()) { // если чтение завершилось не из-за конца файла, то ошибка!
        std::cerr << "Ошибка чтения входного файла\n";
        return 1;
    }

    std::cout << "Маскирование завершено. Результат записан в файл: " << output_file_name << "\n";

    return 0;
}
