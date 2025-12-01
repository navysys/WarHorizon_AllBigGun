# Info
"게임 개발 혼자서 어디까지 할 수 있을까?" 라는 의문에 시작하여 프로그래밍, 디자인, 기획 전부 가능한 만큼 해보는 프로젝트 입니다.
<br>
친숙한 '리그 오브 레전드' 의 조작법과 시스템을 기반으로 전함끼리의 함대전을 구현하는 것이 목표입니다.
<br>
구축함, 순양함, 전함, 항공모함의 특성을 표현하기 위해 이동 속도, 체력 등의 요소에서 많은 차이가 있어 일반적인 MOBA 문법을 그대로 사용할 수 없으므로 3개의 덱(목숨)을 가지고 팀 데스매치의 형식으로 게임이 진행됩니다.
<br>
적의 모든 전함을 소모시키거나 적의 기지를 점령하면 게임에서 승리하게 됩니다.
<br>
구름보다 위의 아주 먼 곳에서 바라보는 시점을 구현하는 경우 그림자가 먼 거리에서 사라지는 문제가 발생하였고 이를 해결하기 위해 여러 솔루션을 테스트하여 코드 변경 이외에도 엔진 설정 변경 및 리스소 편집 등의 방법을 사용하였습니다.
<br>
원경의 시점은 한 가지의 예시일 뿐 비슷한 레퍼런스가 적은 세계 2차대전 전함의 탑뷰 시점 게임을 제작하며 다양한 문제가 발생하고 이를 해결하였는데 그 목록은 아래와 같습니다.
<br>
<br>
+ 최대 10 vs 10 멀티 플레이어 환경 구축
+ 메모리 참조를 통한 맵핵 등의 외부 프로그램 사용을 방지하는 서버 기반 "전장의 안개" 구현(시야 시스템)
+ MOBA장르의 시스템과 조작법에 맞도록 함선 클래스 구조의 변환과 AI가 빙의를 통해 플레이어와 동일하게 동작
+ 독자적으로 분리된 스킬 에셋들을 제작하여 서로 다른 전함에도 같은 스킬을 적용할 수 있도록 설계
+ 무거운 전함은 프로펠러를 통한 직선 운동과 키를 통해 회전을 제어하는데 이 움직임과 관성의 구현
+ 함재기는 착함을 제외하면 정지할 수 없고 항상 움직여야하는데 속도와 회전만을 제어하여 원형으로 회전하며 위치를 지키는 움직임 구현
+ 전함에 많게는 수백개의 포탑이 존재하고 플레이어는 주 함포만 직접 조정하는데 나머지 함포에 효율적으로 타겟을 할당하고 이를 추적 공격하는 메커니즘 및 알고리즘 활용
<br>

# Tech Stack
<img src="https://img.shields.io/badge/unreal%20engine-%23313131.svg?&style=for-the-badge&logo=unreal%20engine&logoColor=white" /> <img src="https://img.shields.io/badge/springboot-6DB33F?&style=for-the-badge&logo=springboot&logoColor=white" /> <img src="https://img.shields.io/badge/mysql-%234479A1.svg?&style=for-the-badge&logo=mysql&logoColor=white" /> <img src="https://img.shields.io/badge/json-%23000000.svg?&style=for-the-badge&logo=json&logoColor=white" />
<br>
<img src="https://img.shields.io/badge/c++-00599C?style=for-the-badge&logo=C%2B%2B&logoColor=white"><img src="https://img.shields.io/badge/javascript-F7DF1E?style=for-the-badge&logo=javascript&logoColor=white">
<br>
<img src="https://img.shields.io/badge/visual%20studio-%235C2D91.svg?&style=for-the-badge&logo=visual%20studio&logoColor=white" /><img src="https://img.shields.io/badge/intellij%20idea-%23000000.svg?&style=for-the-badge&logo=intellij%20idea&logoColor=white" />

# Server Client Structure
![서버클라이언트구조](https://github.com/user-attachments/assets/664bf13b-987c-4ab5-a71a-7a468ec58be4)
<br>

# Feature
### Unreal Engine
+ 로그인
+ 서버 생성
+ 서버 참여
+ 매치메이킹
+ 플레이어 / AI
+ 채팅

### Web Server
+ 로그인 / 회원가입
+ 게임 결과 저장, 반환
+ 채팅 기록



