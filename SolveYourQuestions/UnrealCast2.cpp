//#include <iostream>
//#include <vector>
//#include <string>
//
//// --- 언리얼 엔진의 타입 정보 시스템 (UClass 흉내) ---
//struct UClass
//{
//    std::string Name;
//    size_t TypeID;                   // 클래스별 고유 숫자 번호
//    std::vector<size_t> ParentIDs;   // 부모들의 숫자 번호 리스트 (족보)
//};
//
//class UObject
//{
//public:
//    virtual ~UObject() {}
//    virtual UClass* GetClass() const = 0;
//
//    // 언리얼의 IsA 기능을 시각화한 함수
//    template <typename T>
//    bool IsA() const
//    {
//        UClass* MyType = this->GetClass();
//        UClass* TargetType = T::StaticClass();
//
//        std::cout << "\n[Cast 검사 시작]" << std::endl;
//        std::cout << "▶ 내 실제 타입: " << MyType->Name << " (ID: " << MyType->TypeID << ")" << std::endl;
//        std::cout << "▶ 목표 타입: " << TargetType->Name << " (ID: " << TargetType->TypeID << ")" << std::endl;
//
//        // 1단계: 직계 타입 대조
//        std::cout << "STEP 1: 직계 ID 대조 중... ";
//        if (MyType->TypeID == TargetType->TypeID)
//        {
//            std::cout << "✅ 일치!" << std::endl;
//            return true;
//        }
//        std::cout << "❌ 불일치 (다음 단계로)" << std::endl;
//
//        // 2단계: 족보(부모 리스트) 전수 조사
//        std::cout << "STEP 2: 부모 족보 리스트 검색 시작 (총 " << MyType->ParentIDs.size() << "명)" << std::endl;
//        for (size_t pid : MyType->ParentIDs)
//        {
//            std::cout << "   - 족보 내 ID [" << pid << "] 확인 중... ";
//            if (pid == TargetType->TypeID)
//            {
//                std::cout << "🎯 찾았다! (상속 관계 확인)" << std::endl;
//                return true;
//            }
//            std::cout << "아님" << std::endl;
//        }
//
//        std::cout << "결과: 이 객체는 " << TargetType->Name << " 계열이 아닙니다." << std::endl;
//        return false;
//    }
//};
//
//// --- 언리얼의 글로벌 Cast 함수 ---
//template <typename T, typename From>
//T* UnrealCast(From* Src)
//{
//    if (Src && Src->template IsA<T>())
//    {
//        std::cout << ">>> 캐스팅 최종 승인: static_cast 실행" << std::endl;
//        return static_cast<T*>(Src);
//    }
//    std::cout << ">>> 캐스팅 최종 거부: nullptr 반환" << std::endl;
//    return nullptr;
//}
//
//// --- 테스트용 클래스들 ---
//
//class ACharacter : public UObject
//{
//public:
//    static UClass* StaticClass() { static UClass Ins = { "ACharacter", 100, {} }; return &Ins; }
//    UClass* GetClass() const override { return StaticClass(); }
//};
//
//class APawn : public ACharacter
//{
//public:
//    static UClass* StaticClass() { static UClass Ins = { "APawn", 200, {100} }; return &Ins; }
//    UClass* GetClass() const override { return StaticClass(); }
//};
//
//class AEnemy : public APawn
//{
//public:
//    static UClass* StaticClass() { static UClass Ins = { "AEnemy", 300, {200, 100} }; return &Ins; }
//    UClass* GetClass() const override { return StaticClass(); }
//};
//
//int main()
//{
//    // 진짜 몸통은 Enemy인 객체 생성
//    UObject* MyObj = new AEnemy();
//
//    // 1. 성공 케이스: Enemy를 Pawn으로 캐스팅 (부모로 올라가기)
//    APawn* CastedPawn = UnrealCast<APawn>(MyObj);
//
//    std::cout << "\n" << std::string(50, '=') << "\n";
//
//    // 2. 실패 케이스: Enemy와 상관없는 클래스로 캐스팅
//    struct AStranger : public UObject {
//        static UClass* StaticClass() { static UClass Ins = { "AStranger", 999, {} }; return &Ins; }
//        UClass* GetClass() const override { return StaticClass(); }
//    };
//    AStranger* CastedStranger = UnrealCast<AStranger>(MyObj);
//
//    delete MyObj;
//    return 0;
//}