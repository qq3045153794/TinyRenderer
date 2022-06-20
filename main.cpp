#include <iostream>
using namespace std;

class Data{
public:
    Data()
        : nums(-1) {std::cout<<"create Data"<<std::endl;};
    //Data(const Data&) = delete;
    //Data operator&=(const Data&) = delete;
    ~Data(){
        std::cout<<"delete Data"<<std::endl;
    }
    int nums;
};

class A {
public:
    A():
        a(10),b(20),data(){ std::cout<<"create A"<<std::endl; }
    virtual const int get_c() = 0;
    int a;
    int b;
    Data data;
};

class B : public A{
public:
    B():
        A(),
        c(30){std::cout<<"create B"<<std::endl;}
    virtual const int get_c() override {
        return c ;
    }
    int c;
};
void func(Data& data){
    std::cout<<"func"<<std::endl;
} 
int main() {
/*
    A* c_a = new B;
    std::cout<< c_a->get_c() <<std::endl;   
    B c_b;
    std::cout<<(c_b.A::a)<<" "<<(c_b.B::a)<<std::endl;
    std::cout<<&(c_b.A::a)<<" "<<&(c_b.B::a)<<std::endl;

    c_b.A::a = 1;
    std::cout<<(c_b.A::a)<<" "<<(c_b.B::a)<<std::endl;
    std::cout<<ccc<<"\n";
*/

    Data a ;
    func(a);
}