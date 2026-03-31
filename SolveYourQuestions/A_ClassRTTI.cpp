//#include <iostream>
//#include <typeinfo> // RTTI(typeid)를 사용하기 위해 필요
//
//class Parent
//{
//
//public:
//    virtual ~Parent() {} // virtual이 있어야 vtable이 생기고 dynamic_cast가 작동함
//    virtual void sayHello() { std::cout << "안녕, 나는 부모야!\n"; }
//};
//
//class Child : public Parent 
//{
//
//public:
//    int childData = 777;
//    void sayHello() override { std::cout << "안녕, 나는 자식이야!\n"; }
//    void play() { std::cout << "자식만 가진 play 함수 실행 중!\n"; }
//};
//
//// 부모-자식 관계가 전혀 없는 다른 클래스
//class Another 
//{ 
//public:
//    virtual ~Another() {}
//};
//
//int main() 
//{
//
//    // 1. 자식 객체 생성
//    Child* realChild = new Child();
//    Parent* pPtr = realChild; // 업캐스팅 (언제나 안전)
//
//    std::cout << "--- [ 주소 및 타입 확인 ] ---" << std::endl;
//    std::cout << "실제 객체의 주소 (realChild): " << realChild << std::endl;
//    std::cout << "부모 포인터가 가리키는 주소 (pPtr): " << pPtr << std::endl;
//    std::cout << "pPtr이 가리키는 실제 클래스 이름: " << typeid(*pPtr).name() << std::endl; // RTTI를 이용해 런타임에 실제 정체 확인
//
//
//
//    std::cout << "\n--- [ dynamic_cast 테스트 ] ---" << std::endl;
//
//    // 성공하는 케이스: pPtr은 실제 Child를 가리키고 있음
//    Child* castedChild = dynamic_cast<Child*>(pPtr);
//    if (castedChild) 
//    {
//        std::cout << "✅ dynamic_cast 성공!" << std::endl;
//        std::cout << "캐스팅된 주소: " << castedChild << std::endl;
//        std::cout << "자식 멤버 변수 접근: " << castedChild->childData << std::endl;
//        castedChild->play();
//    }
//
//    // 실패하는 케이스: Child를 Another 타입으로 바꾸려고 시도
//    Another* castedAnother = dynamic_cast<Another*>(pPtr);
//    if (castedAnother == nullptr) 
//    {
//        std::cout << "❌ dynamic_cast 실패! (nullptr 반환)" << std::endl;
//        std::cout << "이유: pPtr은 Another 클래스와 아무 상관이 없음." << std::endl;
//    }
//
//    delete realChild;
//    return 0;
//}