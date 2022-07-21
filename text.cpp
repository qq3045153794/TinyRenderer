#include <iostream>
#include <string>
using namespace std;



std::string fun(){
 std::string str("1234");
 return std::move(str);
}
int main() {
    std::string t = fun();
    std::cout<< t <<(&t)<<"\n";
    std::cout<< t <<(&t)<<"\n";
}