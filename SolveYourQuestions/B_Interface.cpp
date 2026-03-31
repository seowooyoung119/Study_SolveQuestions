/**
 * [ C++ 인터페이스 및 크로스 캐스팅(Cross Cast) 정리 ]
 * * 1. C++ 인터페이스의 실체
 * 
 * -------------------------------------------------------------------------
 * - C++은 'interface' 키워드가 없는 대신, 모든 함수가 'virtual ... = 0;'인
 * 순수 가상 클래스를 인터페이스로 취급합니다.
 * - 클래스가 여러 인터페이스를 상속받으면(다중 상속),
 * 각 인터페이스마다 고유한 [vptr]과 [vtable]이 객체 메모리에 생성됩니다.
 * 
 * -------------------------------------------------------------------------
 * * 2. 인터페이스 캐스팅의 작동 메커니즘
 * -------------------------------------------------------------------------
 * 
 * Step 1. [검색] dynamic_cast가 실행되면 Locator를 통해 상속 계보(Hierarchy)를 뒤집니다.
 * Step 2. [확인] 내가 찾으려는 인터페이스 이름이 계보 리스트에 있는지 확인합니다.
 * Step 3. [좌표 획득] 리스트에서 해당 인터페이스의 'where_mdisp(오프셋)' 정보를 읽습니다.
 * Step 4. [주소 점프] '현재 주소 + 오프셋'을 계산하여, 해당 인터페이스의 vtable이
 * 시작되는 정확한 위치로 포인터를 이동시킵니다. (위 코드의 주소 차이 발생 원인)
 * 
 * -------------------------------------------------------------------------
 * * 3. 왜 인터페이스를 쓰는가? (유연성)
 * -------------------------------------------------------------------------
 * 
 * - 수직적 상속(Parent-Child) 관계가 아니더라도,
 * "특정 기능을 할 수 있는가?"라는 수평적 기준으로 객체를 다룰 수 있습니다.
 * - RTTI는 객체가 어떤 '기능 명찰'들을 달고 있는지 런타임에 완벽하게
 * 파헤칠 수 있는 근거를 제공합니다.
 * 
 * -------------------------------------------------------------------------
 */
//
//#include <iostream>
//#include <typeinfo>
//
// // 1. C++ 식 인터페이스 정의 (순수 가상 함수만 포함)
//class IDamageable
//{
//public:
//    virtual ~IDamageable() {}
//    virtual void TakeDamage(float amount) = 0; // 순수 가상 함수
//};
//
//class IInteractable
//{
//public:
//    virtual ~IInteractable() {}
//    virtual void Interact() = 0; // 순수 가상 함수
//};
//
//// 2. 여러 인터페이스를 상속받는 실제 클래스
//class Player : public IDamageable, public IInteractable
//{
//public:
//    void TakeDamage(float amount) override
//    {
//        std::cout << "Player가 " << amount << "만큼 데미지를 입음!\n";
//    }
//
//    void Interact() override
//    {
//        std::cout << "Player가 오브젝트와 상호작용함!\n";
//    }
//};
//
//int main()
//{
//    // [현장 검증]
//    Player* myPlayer = new Player();
//
//    std::cout << "[ 객체 주소 확인 ]" << std::endl;
//    std::cout << "1. Player 원본 주소:    " << myPlayer << std::endl;
//
//    // 3. 첫 번째 인터페이스로 캐스팅
//    IDamageable* dmg = dynamic_cast<IDamageable*>(myPlayer);
//    std::cout << "2. IDamageable 주소:   " << dmg << " (첫 번째 상속이라 원본과 같을 수 있음)" << std::endl;
//
//    // 4. 두 번째 인터페이스로 캐스팅 (여기서 주소가 바뀝니다!)
//    IInteractable* inter = dynamic_cast<IInteractable*>(myPlayer);
//    std::cout << "3. IInteractable 주소:  " << inter << " <-- 주소가 점프했습니다!" << std::endl;
//
//    // [왜 바뀌었을까?]
//    std::cout << "\n[ 원리 분석 ]" << std::endl;
//    std::cout << "두 주소의 차이(Offset): " << (char*)inter - (char*)myPlayer << " 바이트" << std::endl;
//    std::cout << "이 차이가 바로 'where_mdisp' 정보에 의해 계산된 결과입니다." << std::endl;
//
//    // 5. 바뀐 주소로도 기능은 정상 작동
//    inter->Interact();
//
//    delete myPlayer;
//    return 0;
//}