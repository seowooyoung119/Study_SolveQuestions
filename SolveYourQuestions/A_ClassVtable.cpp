//#include <iostream>
//
//class Base
//{
//public:
//    virtual void func1() { std::cout << "Base::func1 호출됨!" << std::endl; }
//    virtual void func2() { std::cout << "Base::func2 호출됨!" << std::endl; }
//};
//
//class Derived : public Base 
//{
//public:
//    void func1() override { std::cout << "Derived::func1 호출됨! (오버라이딩)" << std::endl; }
//    void func2() override { std::cout << "Derived::func2 호출됨! (오버라이딩)" << std::endl; }
//};
//
//// 함수 포인터 타입 정의 (인자 없고 반환값 없는 함수)
//typedef void (*FuncPtr)();
//
//int main() 
//{
//    Derived* d = new Derived();
//
//    std::cout << "1. 객체의 시작 주소: " << d << std::endl;
//
//    // 2. vptr 추출 (객체의 첫 8바이트가 vptr임)
//    // long long*로 캐스팅해서 첫 8바이트 값을 읽어옴
//    long long* vptr = (long long*)*(long long*)d;
//    std::cout << "2. vtable의 시작 주소 (vptr의 값): " << vptr << std::endl;
//
//    // 3. vtable의 첫 번째 칸(func1)과 두 번째 칸(func2)의 함수 주소 확인
//    FuncPtr f1 = (FuncPtr)vptr[0];
//    FuncPtr f2 = (FuncPtr)vptr[1];
//
//    std::cout << "3. vtable[0] (func1) 주소: " << (void*)f1 << std::endl;
//    std::cout << "4. vtable[1] (func2) 주소: " << (void*)f2 << std::endl;
//
//    // 4. 추출한 주소로 함수 직접 실행!
//    std::cout << "\n--- [ vtable에서 꺼낸 주소로 함수 호출 ] ---" << std::endl;
//    f1(); // Derived::func1 실행
//    f2(); // Derived::func2 실행
//
//    delete d;
//    return 0;
//}