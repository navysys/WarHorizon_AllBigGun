# Info
"게임 개발 혼자서 어디까지 할 수 있을까?" 라는 의문에 시작하여 프로그래밍, 디자인, 기획 전부 가능한 만큼 해보는 프로젝트 입니다.
<br>
친숙한 '리그 오브 레전드' 의 조작법과 시스템을 기반으로 전함끼리의 함대전을 구현하는 것이 목표입니다.
구축함, 순양함, 전함, 항공모함의 특성을 표현하기 위해 이동 속도, 체력 등의 요소에서 많은 차이가 있어 일반적인 MOBA 문법을 그대로 사용할 수 없으므로 3개의 덱(목숨)을 가지고 팀 데스매치의 형식으로 게임이 진행됩니다.
적의 모든 전함을 소모시키거나 적의 기지를 점령하면 게임에서 승리하게 됩니다.
<br>
<br>
+ 최대 10 vs 10 멀티 플레이어 환경 + 최대 1000개 가량의 포탑 + 함재기
+ 메모리 참조를 통한 맵핵 등의 외부 프로그램 사용을 방지하는 서버 기반 "전장의 안개" 구현(시야 시스템)
+ MOBA장르의 시스템과 조작법에 맞도록 함선 클래스 구조의 변환과 AI가 빙의를 통해 플레이어와 동일하게 동작
+ 
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



