//#include <iostream>
//#include <vector>
//#include <string>
//
//class UObject
//{
//public:
//    std::string Name;
//    bool bIsMarked = false;               // GC가 확인했는지 체크하는 '마킹'
//    std::vector<UObject*> References;      // 이 객체가 참조하는 다른 객체들
//
//    UObject(std::string InName) : Name(InName)
//    {
//        std::cout << "  [생성] " << Name << " 객체가 메모리에 할당되었습니다.\n";
//    }
//
//    ~UObject()
//    {
//        std::cout << "  [파괴] " << Name << " 객체가 GC에 의해 수거되었습니다.\n";
//    }
//
//    // 참조 추가 (언리얼의 UPROPERTY 역할)
//    void AddReference(UObject* Target)
//    {
//        References.push_back(Target);
//    }
//};
//
//// --- 가비지 컬렉터 시스템 ---
//class GarbageCollector
//{
//public:
//    std::vector<UObject*> AllObjects; // 메모리에 존재하는 모든 객체 명단
//    std::vector<UObject*> RootSet;    // 절대 지워지면 안 되는 '뿌리' 객체들
//
//    // 1단계: 마킹 (Mark) - 루트부터 연결된 모든 객체를 찾아감
//    void Mark(UObject* Obj)
//    {
//        if (Obj == nullptr || Obj->bIsMarked) return;
//
//        Obj->bIsMarked = true; // "살아있음" 표시
//        std::cout << "    - " << Obj->Name << " 확인됨 (살려둠)\n";
//
//        for (UObject* Ref : Obj->References)
//        {
//            Mark(Ref); // 재귀적으로 연결된 모든 친구들을 찾음
//        }
//    }
//
//    // 2단계: 소생 및 삭제 (Sweep) - 마킹 안 된 녀석들만 골라 죽임
//    void Sweep()
//    {
//        std::cout << "\n[GC Sweep 시작: 마킹 안 된 좀비 객체 수거]\n";
//
//        for (auto it = AllObjects.begin(); it != AllObjects.end(); )
//        {
//            if (!(*it)->bIsMarked)
//            {
//                UObject* Garbage = *it;
//                it = AllObjects.erase(it); // 명단에서 제거
//                delete Garbage;            // 실제 메모리 해제
//            }
//            else
//            {
//                (*it)->bIsMarked = false; // 다음 GC를 위해 마킹 초기화
//                ++it;
//            }
//        }
//    }
//
//    void Collect()
//    {
//        std::cout << "\n[GC Mark 시작: Root로부터 연결 확인]\n";
//        for (UObject* Root : RootSet)
//        {
//            Mark(Root);
//        }
//        Sweep();
//    }
//};
//
//int main()
//{
//    GarbageCollector GC;
//
//    // 1. 객체들 생성
//    UObject* Player = new UObject("Player(Root)");
//    UObject* Sword = new UObject("Excalibur(Sword)");
//    UObject* Enemy = new UObject("Goblin");
//    UObject* Gold = new UObject("100Gold");
//
//    GC.AllObjects = { Player, Sword, Enemy, Gold };
//    GC.RootSet = { Player }; // Player는 게임 세계의 중심(Root)이라 절대 안 지워짐
//
//    // 2. 관계 설정
//    Player->AddReference(Sword); // Player가 칼을 들고 있음
//    Enemy->AddReference(Gold);   // 고블린이 골드를 가지고 있음
//
//    std::cout << "\n--- 상황 1: 모든 객체가 연결되어 있을 때 ---\n";
//    // 현재: Root -> Player -> Sword  /  Enemy -> Gold (Enemy는 아무도 참조 안 함)
//    GC.Collect();
//
//    std::cout << "\n--- 상황 2: Player가 칼을 버렸을 때 ---\n";
//    Player->References.clear(); // 연결 끊김
//    GC.Collect();
//
//    return 0;
//}
//
///**
// * [ 가비지 컬렉션(GC)의 성능 대가 ]
// * -------------------------------------------------------------------------
// * 1. 시간 복잡도:
// * 객체 수가 N개일 때, 마킹 작업은 대략 O(N)의 시간이 걸립니다.
// * 즉, 게임 규모가 커질수록 GC 타임도 늘어납니다.
// * * 2. 캐시 효율 저하:
// * GC가 메모리 이곳저곳을 찌르고 다니기 때문에 CPU 캐시 적중률이 떨어집니다.
// * * 3. 프로그래머의 책임:
// * "GC가 있으니까 메모리 신경 안 써도 돼"가 아니라,
// * "GC가 덜 힘들게 하려면 어떻게 설계할까"를 고민해야 고성능 게임이 나옵니다.
// * -------------------------------------------------------------------------
// */