#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>

uint64_t bit;

// K1(x)=x^35+x^2+1
int n = 35;
int i;

uint64_t Rlz1 = 0x1A3F5C9AB;
uint64_t feedback_bit_1; // переменная для бита обратной последовательности первого регистра

// K2(x)=x^135+x^22+1
uint8_t Rlz2[135]; // переменная имеет точно 8 бит
uint64_t feedback_bit_2;

int main() {

    std::string input_file_name;
    std::string output_file_name;

    std::cout << "Введите имя входного файла: ";
    std::getline(std::cin >> std::ws, input_file_name);

    std::cout << "Введите имя выходного файла: ";
    std::getline(std::cin >> std::ws, output_file_name);

    std::ifstream input_file(input_file_name, std::ios::binary);
    if (!input_file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть входной файл\n";
        return 1;
    }

    std::ofstream output_file(output_file_name, std::ios::binary);
    if (!output_file.is_open()) {
        std::cerr << "Ошибка: не удалось создать выходной файл\n";
        return 1;
    }

    // начальное состояние регистра
    uint64_t high = 0xABFCBBAEA;
    uint64_t low  = 0xAAFEBBECFEBAAFCA;
    // заполнение младших бит регистра 
    for (i = 0; i < 64; i++)
        Rlz2[i] = (low >> i) & 1; // побитовый сдвиг вправо, остается младший бит 
    // заполнение старших бит регистра 
    for (i = 0; i < 71; i++)
        Rlz2[i + 64] = (high >> i) & 1;

    char input_byte; // переменная для чтения байта из входного файла
    uint8_t gamma_byte; // переменная для хранения байта гаммы, который будет xorиться с байтом из входного файла

    while (input_file.get(input_byte)) { // читает файл по байтам, пока не достигнет конца файла

        gamma_byte = 0;

        for (i = 0; i < 8; i++) {

            // первый регистр
            feedback_bit_1 = (Rlz1 & 1) ^ ((Rlz1 >> 1) & 1); // xor младшего бита и второго бита регистра
            Rlz1 = (Rlz1 >> 1) | (feedback_bit_1 << (n - 1)); // сдвиг регистра вправо, новый бит вставляется в старший разряд, и объединяет 

            // второй регистр
            feedback_bit_2 = Rlz2[0] ^ Rlz2[21];

            for (int j = 0; j < 134; j++) // биты регистра смещаются на одну позицию, после сдвига записываются старший разряд новый бит обратной связи
                Rlz2[j] = Rlz2[j + 1]; // значение следующего бита перемещается на одну позицию влево по массиву 

            Rlz2[134] = feedback_bit_2; // новый бит записывается в старший разряд

            // выходной бит
            bit = feedback_bit_1 ^ feedback_bit_2; // комбинирование двух регистров

            gamma_byte |= (bit << i); // бит сдвигается влево на i
        }

        output_file.put(static_cast<char>(
            static_cast<uint8_t>(input_byte) ^ gamma_byte));
    }

    if (!input_file.eof()) {
        std::cerr << "Ошибка чтения входного файла\n";
        return 1;
    }

    std::cout << "Маскирование завершено. Результат записан в файл: "
              << output_file_name << "\n";

    return 0;
}