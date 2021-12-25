#include <bitset>
#include <iostream>
#include <stdint.h>
using namespace std;
int main(int argc, const char **argv) {
  unsigned int A = 4095;
  unsigned int B = 1248;
  unsigned long long test_b = 4095 * 4095;

  cout << A << endl;
  uint8_t *tc = (uint8_t *)malloc(sizeof(uint8_t) * 3);
  uint32_t inconvert = A << 20;

  inconvert |= B;
  tc = (uint8_t *)&(inconvert);
  cout << ((*(uint32_t *)tc) >> 20) << endl;
  cout << (((*(uint32_t *)tc) << 20) >> 20) << endl;

  cout << "Normal = " << bitset<sizeof(unsigned int) * 8>((4095 * 4095))
       << endl;
  cout << "Decal_Casted = " << bitset<sizeof(unsigned int) * 8>(test_b << 8)
       << endl;
  cout << "Reduced = " << bitset<24>(test_b) << endl;
  cout << endl;
  cout << "Decal_Casted 4095 = " << bitset<32>(4095 << 20) << endl;
  cout << "Reduced 4095 = " << bitset<12>(4095) << endl;
  cout << "Normal 4095 = " << bitset<sizeof(unsigned int) * 8>(4095) << endl;

  int n = 1;

  if (*(char *)&n == 1) {
    cout << "litle endian" << endl;
  }
  return 0;
}