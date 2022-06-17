#include <iostream>
using namespace std;
int main() {
    int x = 0x12345678;
    int* p = &x;
    char* pc = (char*) p;
    printf("%x\n", *pc);
}