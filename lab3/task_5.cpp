#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

uint8_t komb(uint8_t x, const uint8_t S[16], int p) { // 1 байт x(8 бит ) для обработки, таблицу замены S, число з определяет насколько битов будет сдвигаться результат
  /*Старшие 4 бита*/
  uint8_t x1 = x >> 4 & 0x0F;
  /*Младшие 4 бита*/
  uint8_t x2 = x & 0x0F;

  /*Подстановка*/
  uint8_t y1 = S[x1]; // находит новый 4х битный блок, который заменяет x1 
  uint8_t y2 = S[x2]; // новый 4х битный блок для x2

  /*Объединение*/
  uint8_t a = (y1 << 4) | y2; // сдвигаем на 4 в старшие, y2 - это младшие биты, обьединяет 

  /*циклический сдвиг*/
  uint8_t b = (a >> p) | (a << (8 - p));

  return b;
}

int main() {
  // Вариант 3 (см. таблицу в методичке).
  uint8_t S[16] = {0x0B, 0x03, 0x05, 0x08, 0x02, 0x0F, 0x0A, 0x0D,
                   0x0E, 0x01, 0x07, 0x04, 0x0C, 0x09, 0x06, 0x00}; // массив из 16 элементов, индекс входа - 4 бита, выход 4 бита соответственно 
  int p = 5;

  cout << "Введите имя файла: ";
  string plaintext;
  cin >> plaintext;

  string output = plaintext + ".m";

  ifstream infile(plaintext, ios::binary);
  ofstream outfile(output, ios::binary);

  if (!infile.is_open() || !outfile.is_open()) {
    cerr << "Ошибка открытия файлов!" << endl;
    return 1;
  }

  uint8_t buffer = 0;
  while (infile.read(reinterpret_cast<char*>(&buffer), sizeof(buffer))) {
    buffer = komb(buffer, S, p);
    outfile.write(reinterpret_cast<char*>(&buffer), sizeof(buffer));
  }

  infile.close();
  outfile.close();

  cout << "Преобразование завершено." << endl;
  return 0;
}
