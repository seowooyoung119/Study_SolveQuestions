//#include <iostream>
//#include <string>
//
//class UObject
//{
//public:
//    enum class EObjectFlags
//    {
//        None = 0,
//        PendingKill = 1 << 0,  // 삭제 대기 중 (가비지 컬렉션 대상)
//        Garbage = 1 << 1       // 이미 쓰레기임
//    };
//
//    UObject(std::string Name) : Name(Name), Flags(EObjectFlags::None) {}
//    virtual ~UObject() {}
//
//    std::string Name;
//    EObjectFlags Flags; // 객체의 상태를 나타내는 플래그
//
//    // 객체를 "삭제 대기" 상태로 만듭니다. (언리얼의 Destroy/MarkAsGarbage)
//    void MarkPendingKill()
//    {
//        std::cout << "--- [" << Name << "] 객체에 '삭제 대기' 마킹을 합니다. ---" << std::endl;
//        Flags = EObjectFlags::PendingKill;
//    }
//};
//
//// --- 언리얼의 IsValid 작동 방식 시뮬레이션 ---
//bool IsValid(const UObject* Test)
//{
//    std::cout << "\n[IsValid 검사 수행 중...]" << std::endl;
//
//    // 1단계: 포인터 자체가 null인지 확인 (C++ 표준 방식)
//    std::cout << "STEP 1: Null 포인터 체크... ";
//    if (Test == nullptr)
//    {
//        std::cout << "❌ 포인터가 nullptr입니다!" << std::endl;
//        return false;
//    }
//    std::cout << "✅ OK" << std::endl;
//
//    // 2단계: 객체 내부에 '삭제 대기' 플래그가 있는지 확인 (언리얼 방식)
//    std::cout << "STEP 2: 객체 상태 플래그 체크... ";
//    if (Test->Flags == UObject::EObjectFlags::PendingKill)
//    {
//        std::cout << "❌ 삭제 대기 중(PendingKill)인 객체입니다!" << std::endl;
//        return false;
//    }
//    std::cout << "✅ 정상 작동 중인 객체입니다." << std::endl;
//
//    return true;
//}
//
//int main()
//{
//    // 1. 정상적인 객체 생성
//    UObject* MyActor = new UObject("PlayerCharacter");
//
//    std::cout << "--- 상황 A: 객체가 쌩쌩할 때 ---";
//    if (IsValid(MyActor))
//    {
//        std::cout << ">>> 결과: 게임 로직 실행 가능!" << std::endl;
//    }
//
//    std::cout << "\n" << std::string(50, '-') << "\n";
//
//    // 2. 객체를 삭제 대기 상태로 만듦
//    MyActor->MarkPendingKill();
//
//    std::cout << "--- 상황 B: 삭제 버튼 눌렀는데 메모리엔 아직 남아있을 때 ---";
//    if (!IsValid(MyActor))
//    {
//        std::cout << ">>> 결과: 유효하지 않음! (안전하게 로직 건너뜀)" << std::endl;
//    }
//
//    delete MyActor;
//    return 0;
//}
//
///**
// * [ 언리얼 엔진 IsValid()의 이중 보안 메커니즘 ]
// * -------------------------------------------------------------------------
// * 
// * 1. 왜 nullptr 체크만으로는 부족한가?
// * - 언리얼은 성능을 위해 객체를 삭제할 때 메모리에서 즉시 날리지 않습니다.
// * - 포인터 주소는 여전히 유효하지만(Dangling Pointer),
// * 객체 내용은 죽어있는 '좀비 상태'가 존재할 수 있기 때문입니다.
// * * 2. IsValid의 2단계 검사 (Two-Step Verification)
// * 
// * -------------------------------------------------------------------------
// * 
// * Step 1 [물리 검사]: "주소값이 있는가?" (ptr != nullptr)
// * - 가장 기본적인 C++ 수준의 포인터 검사입니다.
// * * Step 2 [논리 검사]: "살아있는 상태인가?" (!PendingKill)
// * - 객체 내부 플래그를 확인합니다.
// * - 가비지 컬렉터(GC)가 아직 수거해가지 않았더라도,
// * 이미 파괴 로직이 돌아간 객체라면 false를 반환합니다.
// * 
// * -------------------------------------------------------------------------
// * 3. 핵심 요약
// * - IsValid는 "메모리가 잡혀 있는가?"가 아니라
// * "게임을 계속 진행해도 안전한 객체인가?"를 묻는 함수입니다.
// * 
// * -------------------------------------------------------------------------
// */