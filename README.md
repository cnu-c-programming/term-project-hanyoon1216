Mini Student Shell
1. 프로젝트 개요
Mini Student Shell은 C 언어로 구현한 학생 정보 관리 프로그램이다.
프로그램은 시작 시 CSV 파일에서 학생 정보를 읽어오고, 학생 데이터를 linked list에 저장한다.
사용자는 shell 형태의 명령어를 입력하여 학생 정보를 조회, 추가, 삭제, 수정, 저장할 수 있다.
본 프로젝트는 Admin Program과 Client Program을 제공한다.
Admin Program: 학생 정보 조회 및 수정 가능
Client Program: 학생 정보 조회만 가능
2. 파일 구조
txt

.
├── main.c
├── student.c
├── student.h
├── file_io.c
├── file_io.h
├── command.c
├── command.h
├── Makefile
├── students.csv
├── commands.txt
└── README.md
3. 빌드 방법
Makefile을 사용하여 Admin Program과 Client Program을 빌드할 수 있다.
Admin Program 빌드:
bash

make admin
Client Program 빌드:
bash

make client
실행 파일 삭제:
bash

make clean
make가 설치되어 있지 않은 경우 다음 명령어로 직접 컴파일할 수 있다.
bash

gcc -Wall -Wextra -std=c11 -DADMIN_MODE main.c student.c file_io.c command.c -o admin_shell
gcc -Wall -Wextra -std=c11 -DCLIENT_MODE main.c student.c file_io.c command.c -o client_shell
4. 실행 방법
Admin Program 실행:
bash

./admin_shell students.csv
Client Program 실행:
bash

./client_shell students.csv
명령어 파일을 사용하는 경우:
bash

./admin_shell students.csv -f commands.txt
./client_shell students.csv -f commands.txt
Windows PowerShell에서는 다음과 같이 실행할 수 있다.
.\admin_shell.exe students.csv
.\client_shell.exe students.csv
.\admin_shell.exe students.csv -f commands.txt
5. CSV 파일 형식
CSV 파일의 첫 줄은 반드시 다음 header여야 한다.
txt

id,name,score
예시는 다음과 같다.
txt

id,name,score
1,Alice,90
2,Bob,85
3,Charlie,95
CSV 규칙은 다음과 같다.
id는 정수여야 한다.
id는 양수여야 한다.
name은 비어 있을 수 없다.
name에는 쉼표가 들어갈 수 없다.
score는 0 이상 100 이하의 정수여야 한다.
header는 정확히 id,name,score여야 한다.
6. 명령어 목록
Admin Program 명령어
명령어	설명
save	현재 linked list를 CSV 파일에 저장
reload	CSV 파일에서 학생 정보를 다시 불러오기
add <id> <name> <score>	학생 추가
delete <id>	ID로 학생 삭제
update <id> <score>	학생 점수 수정
find <id>	ID로 학생 검색
list	전체 학생 출력
stats	학생 수, 평균, 최고점, 최저점 출력
help	사용 가능한 명령어 출력
clear	화면 지우기
exit	프로그램 종료
Client Program 명령어
명령어	설명
reload	CSV 파일에서 학생 정보를 다시 불러오기
find <id>	ID로 학생 검색
list	전체 학생 출력
stats	학생 수, 평균, 최고점, 최저점 출력
help	사용 가능한 명령어 출력
clear	화면 지우기
exit	프로그램 종료
Client Program에서는 save, add, delete, update 명령어를 사용할 수 없다.
Client Program에서 제한된 명령어를 입력하면 다음 메시지가 출력된다.
txt

Unknown command or permission denied.
7. 명령어 파일 사용법
프로그램은 -f 옵션을 사용하여 명령어 파일을 실행할 수 있다.
bash

./admin_shell students.csv -f commands.txt
명령어 파일은 한 줄에 하나의 명령어를 포함한다.
예시:
txt

list
add 4 David 88
update 99 70
find 4
save
exit
빈 줄은 무시된다.
#으로 시작하는 줄은 주석으로 처리되어 무시된다.
명령어 파일 실행 중 오류가 발생하면 오류 메시지를 출력하고 다음 줄의 명령어를 계속 실행한다.
명령어 파일에 exit이 없으면 모든 명령어 실행 후 interactive shell로 진입한다.
명령어 파일을 열 수 없는 경우 오류 메시지를 출력하고 interactive shell로 진입한다.
8. 실행 예시
Admin Program 실행 예:
txt

[Admin Program]
Loaded 3 students from students.csv.
admin> list
ID Name Score
1 Alice 90
2 Bob 85
3 Charlie 95
admin> add 4 David 88
Student added.
admin> save
Saved 4 students to students.csv.
admin> exit
Goodbye.
Client Program 실행 예:
txt

[Client Program]
Loaded 3 students from students.csv.
client> list
ID Name Score
1 Alice 90
2 Bob 85
3 Charlie 95
client> add 4 David 88
Unknown command or permission denied.
client> exit
Goodbye.
9. 예외 처리
본 프로그램은 다음 예외 상황을 처리한다.
CSV 파일 열기 실패
잘못된 CSV header
잘못된 CSV 데이터
잘못된 명령어
인자 부족
잘못된 ID
잘못된 이름
잘못된 점수
중복 ID
존재하지 않는 학생
명령어 파일 열기 실패
10. 구현 방식
학생 정보는 singly linked list로 저장한다.
Admin Program과 Client Program은 같은 소스 코드를 사용하며, Makefile에서 preprocessing flag를 전달하여 서로 다른 실행 파일로 빌드한다.
txt

-DADMIN_MODE
-DCLIENT_MODE
command.c에서는 preprocessing flag에 따라 서로 다른 command table을 구성한다.
