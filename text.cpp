#include <iostream>

#include <memory>

class foo {
 public:
  std::unique_ptr<int> ptr;
};

int main() {

  foo a;
  if(a.ptr){
    std::cout << "not null" << std::endl;
  } else {
    std::cout << "null" << std::endl;
  }

  std::cout << (*a.ptr) << std::endl;

  a.ptr = std::make_unique<int>(10);



  if(a.ptr){
    std::cout << "not null" << std::endl;
  } else {
    std::cout << "null" << std::endl;
  }

  //std::cout << a.ptr << std::endl;


}
