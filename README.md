# Info
"게임 개발 혼자서 어디까지 할 수 있을까?" 라는 의문에 시작하여 프로그래밍, 디자인, 기획 전부 가능한 만큼 해보는 프로젝트 입니다.
친숙한 '리그 오브 레전드' 의 조작법과 시스템을 기반으로 전함끼리의 함대전을 구현하는 것이 목표입니다.
<br>

# Tech Stack
<img src="https://img.shields.io/badge/unreal%20engine-%23313131.svg?&style=for-the-badge&logo=unreal%20engine&logoColor=white" /> <img src="https://img.shields.io/badge/springboot-6DB33F?&style=for-the-badge&logo=springboot&logoColor=white" /> <img src="https://img.shields.io/badge/mysql-%234479A1.svg?&style=for-the-badge&logo=mysql&logoColor=white" /> <img src="https://img.shields.io/badge/json-%23000000.svg?&style=for-the-badge&logo=json&logoColor=white" />
<br>
<img src="https://img.shields.io/badge/c++-00599C?style=for-the-badge&logo=C%2B%2B&logoColor=white"><img src="https://img.shields.io/badge/javascript-F7DF1E?style=for-the-badge&logo=javascript&logoColor=white">
<br>
<img src="https://img.shields.io/badge/visual%20studio-%235C2D91.svg?&style=for-the-badge&logo=visual%20studio&logoColor=white" /><img src="https://img.shields.io/badge/intellij%20idea-%23000000.svg?&style=for-the-badge&logo=intellij%20idea&logoColor=white" />
<br>
# WH_AllBigGun
## Aircraft
### WHAircraft.cpp
그룹으로 활동하기 위한 비행기 단일 개체로 네트워크 부하를 줄이기 위해 리플리케이트 옵션을 사용 X
### WHAircraftrsBase.cpp
다수의 WHAircraft 클래스를 관리하는 베이스 클래스
콜리전을 소유하여 충돌을 판정하고 리플리케이트를 통해 위치와 비행기들의 상태를 통신하는 주체
## BattleShip
### BattleShipBase.cpp
플레이어가 조종하는 함선으로 가장 기능이 많이 들어가 있는 주요 클래스
플레이어가 직접 빙의하지 않고 AI 컨트롤러가 조종하고 플레이어는 AI 에 명령하는 구조
상속 클래스
AFOW_RevealerPawn : 시야 시스템을 위한 기능을 포함하는 클래스
IBattleShipInterface : 함선의 기능을 정의해놓은  인터페이스
ISkillUsableInterface : 스킬을 사용하기 위한 인터페이스
<br>
컴포넌트
BoxComp : 러프하게 다른 유닛과 거리 비교 등을 수행하기 위한 UBoxComponent
SkeletalMeshComp : 실질적인 충돌과 함포를 배치하기 위한 스켈레톤 구조를 가지는 USkeletalMeshComponent
SmokestackComp : 함선 속도에 따라 연돌의 연기를 제어하기 위한 UNiagaraComponent
BattleShipMovementComp : 직선 운동만을 하는 함선의 움직임을 위한 컴포넌트
DetectEnemyComp : 함선 중심을 기준으로 적 함선이나 함재기를 Sphere Collision 을 통해 감지하는 컴포넌트
TurretHandlerComp : 함포들을 관리하고 탐지된 적을 공격 가능한 함포에 분배 및 할당하는 컴포넌트

## BehaviorTree
AI 컨트롤러에서 사용하는 비헤이비어 트리에 사용되는 클래스
###BTService_AircraftChangeAngle.cpp
함재기의 방향을 전환하는 서비스 노드
###BTService_AircraftHeight.cpp
함재기의 높낮이를 전환하는 서비스 노드
###BTTask_AircraftATAAttack.cpp
함재기의 공격 상태를 위한 테스크 노드
###BTTask_AircraftATAMove.cpp
함재기의 움직임 상태를 위한 테스크 노드
###BTTask_AircraftTurnCircle.cpp
함재기가 목표에 도착했을 때 목표지점을 중심으로 원을 그리며 제자리 선회하기 위한 테스크 노드

##Component
