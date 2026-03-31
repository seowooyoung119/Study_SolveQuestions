//#include <iostream>
//#include <vector>
//#include <string>
//
//// 1. 언리얼의 UObject처럼 모든 객체의 최상위 부모 클래스입니다.
//class UObject
//{
//public:
//    virtual ~UObject() {}
//
//    // 모든 언리얼 객체는 자신의 타입 정보를 담은 UClass*를 가지고 있습니다.
//    // 여기서는 심플하게 클래스 이름과 고유 해시값만 저장하는 구조체로 대체합니다.
//    struct UClass
//    {
//        std::string Name;
//        size_t TypeHash; // 클래스별 고유한 숫자 ID (핵심!)
//        std::vector<size_t> SuperClassHashes; // 부모 클래스들의 해시 목록 (조상 족보)
//    };
//
//    // 실제 객체가 자신의 UClass 정보를 반환하는 가상 함수입니다.
//    virtual UClass* GetClass() const = 0;
//
//    // 언리얼의 Cast<T> 함수를 흉내 낸 템플릿 함수입니다.
//    template <typename T>
//    bool IsA() const
//    {
//        UClass* targetClass = T::StaticClass(); // 우리가 바꾸려는 타입의 공식 정보
//        UClass* myClass = GetClass();           // 내 실제 몸통의 정보
//
//        // [핵심 비교 로직]
//        // 1. 내 타입이 타겟 타입과 완전히 일치하는지 해시값 하나만 틱! 비교합니다.
//        if (myClass->TypeHash == targetClass->TypeHash)
//        {
//            return true;
//        }
//
//        // 2. 일치하지 않는다면, 내 조상 족보(SuperClassHashes)를 뒤집니다.
//        // 이 리스트도 숫자(Hash) 배열이라 문자열 비교보다 훨씬 빠릅니다.
//        for (size_t superHash : myClass->SuperClassHashes)
//        {
//            if (superHash == targetClass->TypeHash)
//            {
//                return true;
//            }
//        }
//
//        return false; // 족보에도 없으면 캐스팅 실패
//    }
//};
//
//// 언리얼의 Cast<T>와 사용법을 맞추기 위한 글로벌 템플릿 함수
//template <typename T, typename From>
//T* Cast(From* Src)
//{
//    // 소스가 유효하고, 타겟 타입이 맞는지 IsA()로 먼저 확인합니다.
//    if (Src && Src->template IsA<T>())
//    {
//        // 안전이 확인되었으므로, 아주 빠른 static_cast로 주소만 넘겨줍니다.
//        return static_cast<T*>(Src);
//    }
//    return nullptr; // 타입이 안 맞으면 안전하게 nullptr 반환
//}
//
//
//// --- 실제 클래스 구현 (언리얼 매크로가 해주는 일들을 수동으로 구현) ---
//
//class ACharacter : public UObject
//{
//public:
//
//    // 클래스별 고유 해시값 (실제 언리얼은 컴파일 시 자동으로 생성함)
//    static UClass* StaticClass()
//    {
//        static UClass Ins = { "ACharacter", 1001, {} }; // 최상위라 부모 족보 없음
//        return &Ins;
//    }
//    UClass* GetClass() const override { return StaticClass(); }
//};
//
//class APawn : public ACharacter
//{
//public:
//    static UClass* StaticClass()
//    {
//        // 부모인 ACharacter의 해시(1001)를 족보에 넣습니다.
//        static UClass Ins = { "APawn", 1002, { 1001 } };
//        return &Ins;
//    }
//    UClass* GetClass() const override { return StaticClass(); }
//};
//
//class AEnemy : public APawn
//{
//public:
//    static UClass* StaticClass()
//    {
//        // 조상인 APawn(1002)과 ACharacter(1001)의 해시를 족보에 넣습니다.
//        static UClass Ins = { "AEnemy", 1003, { 1002, 1001 } };
//        return &Ins;
//    }
//    UClass* GetClass() const override { return StaticClass(); }
//};
//
//
//int main()
//{
//    // [현장 테스트]
//    UObject* myObject = new AEnemy(); // 진짜 몸통은 Enemy
//
//    std::cout << "[ 언리얼 식 캐스팅 시뮬레이션 시작 ]" << std::endl;
//
//    // 1. 성공하는 캐스팅 (Enemy -> Pawn으로 업캐스팅)
//    APawn* pawnPtr = Cast<APawn>(myObject);
//    if (pawnPtr)
//    {
//        std::cout << "✅ Cast<APawn> 성공! (Enemy는 Pawn의 자손입니다.)" << std::endl;
//    }
//
//    // 2. 실패하는 캐스팅 (관계없는 클래스로 캐스팅 시도)
//    // 실제 언리얼 구조라면 이런 코드는 컴파일 에러가 나겠지만, 
//    // 여기서는 런타임 실패를 보여주기 위해 Stranger 클래스를 가정합니다.
//    class AStranger : public UObject
//    {
//    public:
//        static UClass* StaticClass() { static UClass Ins = { "AStranger", 9999, {} }; return &Ins; }
//        UClass* GetClass() const override { return StaticClass(); }
//    };
//
//    AStranger* strangerPtr = Cast<AStranger>(myObject);
//    if (strangerPtr == nullptr)
//    {
//        std::cout << "❌ Cast<AStranger> 실패! (Enemy와 Stranger는 남남입니다.)" << std::endl;
//    }
//
//    delete myObject;
//    return 0;
//}