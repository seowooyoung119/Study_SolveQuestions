//#include <iostream>
//#include <typeinfo>
//#include <windows.h>
//
//// --- MSVC 64비트 전용 RTTI 구조체 (오프셋 방식) ---
//
//
//// 1. 클래스의 '진짜 이름'이 담긴 명찰입니다.
//struct TypeDescriptor 
//{
//    void* pVFTable;                          // 이 타입용 가상 함수 테이블
//    void* spare;                             // 예비용 공간
//    char name[1];                            // 클래스 이름 (실제로는 뒤로 더 길게 이어짐)
//};
//
//
//// 2. 계보에 있는 부모들 하나하나의 정보를 담은 '가족 기록부'입니다.
//struct BaseClassDescriptor
//{
//    int pTypeDescriptor;                    // 부모의 TypeDescriptor 주소(ImageBase 기준 오프셋)
//    unsigned int numAccumulatedBases;       // 상속 계보상에서 이 클래스 아래에 몇 개의 클래스가 더 있는지 (계층 깊이)
//
//    // --- [중요] 다중 상속 시 주소 보정을 위한 필드들 ---
//    int where_mdisp;                        // 멤버 변수 오프셋 (Member displacement): 부모 객체가 자식 메모리 내에서 시작점으로부터 얼마나 떨어져 있는지
//    int where_pdisp;                        // 가상 함수 테이블 안의 vbtable(가상 기본 테이블) 오프셋
//    int where_vdisp;                        // vbtable 내에서의 오프셋
//
//    unsigned int attributes;                // 클래스의 특성 (예: 가상 상속 여부 등)을 비트 플래그로 저장
//};
//
//// 3. 이 클래스가 어떤 부모들로부터 상속받았는지 총괄하는 '상속 계보 총람'입니다.
//struct ClassHierarchyDescriptor
//{
//    unsigned int signature;        // 이 구조체의 버전을 나타내는 고유 값 (보통 0)
//    unsigned int attributes;       // 다중 상속인지, 가상 상속인지 등을 나타내는 플래그
//    unsigned int numBaseClasses;   // 나 자신을 포함하여 상속 계보에 엮인 모든 클래스의 총 개수
//    int pBaseClassArray;           // BaseClassDescriptor들의 주소가 모여있는 배열의 위치 (오프셋)
//};
//// 4. vtable 바로 앞에 위치하며, 위 모든 정보들을 하나로 묶어주는 '중앙 관리 센터'입니다.
//struct CompleteObjectLocator
//{
//    unsigned int signature;        // RTTI 데이터의 버전을 나타내는 서명입니다. (64비트 MSVC는 보통 1)
//
//    unsigned int offset;           // [매우 중요] 현재 vtable을 사용하는 '부분 객체'가 
//                                   // 전체 객체의 시작점에서 얼마나 떨어져 있는지 나타내는 값입니다.
//                                   // 다중 상속 시 이 값을 보고 '진짜 객체 시작점'을 찾아냅니다.
//
//    unsigned int cdOffset;         // 생성자/소멸자 호출 시 사용되는 오프셋 정보입니다.
//
//    int pTypeDescriptor;           // 이 클래스의 '이름 명찰(TypeDescriptor)'이 
//                                   // 프로그램 시작점(ImageBase)으로부터 몇 바이트 뒤에 있는지 적힌 주소 정보입니다.
//
//    int pClassDescriptor;          // 이 클래스의 '상속 계보 총람(ClassHierarchyDescriptor)'이 
//                                   // ImageBase로부터 몇 바이트 뒤에 있는지 적힌 주소 정보입니다.
//
//    int pSelf;                     // [ImageBase 계산의 핵심] 
//    // 프로그램 시작점(ImageBase)으로부터 '이 Locator 자신'까지의 거리입니다.
//    // (현재 Locator의 실제 메모리 주소) - pSelf = (프로그램의 시작 주소)
//    // 이 계산을 통해 64비트 보안 환경에서도 다른 정보들의 위치를 다 찾아낼 수 있습니다.
//};
//
//
//// 64비트에서 매우 중요한 함수입니다.
//// MSVC 64비트는 주소를 직접 안 적고 'ImageBase + 오프셋' 방식을 쓰기 때문입니다.
//template <typename T>
//T* GetAddress(uintptr_t imageBase, int offset)
//{
//    if (offset == 0) 
//    {
//        return nullptr;
//    }
//    return (T*)(imageBase + offset);
//}
//
//// 상속 관계 설정
//class GrandParent { public: virtual ~GrandParent() { } };
//class Parent : public GrandParent { public: virtual ~Parent() { } };
//class Child : public Parent { public: virtual ~Child() { } };
//
//void InspectIdentity(void* objPtr) 
//{
//    std::cout << "\n[ 신분증 검사 시작 (Address: " << objPtr << ") ]" << std::endl;
//
//    // 1. 객체의 맨 앞 8바이트는 vtable을 가리키는 vptr입니다.
//    void** vtable = *(void***)objPtr;
//
//    // 2. MSVC는 vtable[-1]에 CompleteObjectLocator의 주소를 숨겨둡니다.
//    CompleteObjectLocator* locator = (CompleteObjectLocator*)vtable[-1];
//
//    // 3. 64비트 보안(ASLR) 때문에 프로그램이 실행될 때마다 주소가 변합니다.
//    // locator->pSelf에는 '프로그램 시작점에서 로케이터까지의 거리'가 저장되어 있습니다.
//    // (로케이터 실제 주소 - 거리)를 하면 '프로그램의 진짜 시작 주소(ImageBase)'가 나옵니다.
//    uintptr_t imageBase = (uintptr_t)locator - locator->pSelf;
//    std::cout << "🚩 프로그램 시작 지점(ImageBase): " << (void*)imageBase << std::endl;
//
//    // 4. 로케이터에서 '내 이름표(TypeDescriptor)' 오프셋을 가져와 주소로 변환합니다.
//    TypeDescriptor* myType = GetAddress<TypeDescriptor>(imageBase, locator->pTypeDescriptor);
//    std::cout << "🆔 내 진짜 이름 (Mangling): " << (myType->name) << std::endl;
//
//    // 5. 로케이터에서 '상속 계보 총람'을 가져옵니다.
//    ClassHierarchyDescriptor* hierarchy = GetAddress<ClassHierarchyDescriptor>(imageBase, locator->pClassDescriptor);
//    
//    // 6. 계보 총람 안에 있는 '가족 기록부 배열'의 시작 주소를 구합니다.
//    int* baseClassArray = GetAddress<int>(imageBase, hierarchy->pBaseClassArray);
//
//    std::cout << "📂 상속 계보에 등록된 클래스 수: " << hierarchy->numBaseClasses << std::endl;
//
//    // 7. 계보를 하나씩 돌면서 부모들의 이름을 출력합니다.
//    for (unsigned int i = 0; i < hierarchy->numBaseClasses; ++i)
//    {
//        // 가족 기록부(BaseClassDescriptor)를 하나씩 꺼냅니다.
//        BaseClassDescriptor* bcd = GetAddress<BaseClassDescriptor>(imageBase, baseClassArray[i]);
//        
//        // 그 기록부 안에 적힌 부모의 이름표(TypeDescriptor) 주소를 찾습니다.
//        TypeDescriptor* baseType = GetAddress<TypeDescriptor>(imageBase, bcd->pTypeDescriptor);
//        
//        std::cout << "   - [" << i << "] 계보 이름: " << (baseType->name) << std::endl;
//    }
//}
//
//int main()
//{
//    // Child 객체를 힙에 생성합니다.
//    Child* myChild = new Child();
//    
//    // 이 녀석의 정체를 파헤칩니다.
//    InspectIdentity(myChild);
//
//    delete myChild;
//    return 0;
//}