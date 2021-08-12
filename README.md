# DogFootRuler
DogFootRuler(개/발/자)는 고등학교 자율 과제 시간에 만든 난해한 프로그래밍 언어로, 모든 명령어가 영어 알파벳 2글자로 이루어진 언어입니다.



# 언어 명세

```
SE stack_id : stack_id번 스택을 선택한다.

IN : 표준입출력으로 정수를 입력받고, 선택된 스택에 push한다.

OU : 선택된 스택의 top을 표준입출력으로 출력한다.

OA : 선택된 스택의 top을 아스키 코드로 취급하여, 표준입출력으로 문자를 출력한다.

PO : 선택된 스택을 pop한다.

CO stack_id : 선택된 스택의 top을 stack_id번 스택에 push한다.

PL : 선택된 스택을 두 번 pop하여 두 번째 값 + 첫 번째 값의 연산 결과를 선택된 스택에 push한다.

MI : 선택된 스택을 두 번 pop하여 두 번째 값 - 첫 번째 값의 연산 결과를 선택된 스택에 push한다.

MU : 선택된 스택을 두 번 pop하여 두 번째 값 * 첫 번째 값의 연산 결과를 선택된 스택에 push한다.

MO : 선택된 스택을 두 번 pop하여 두 번째 값 % 첫 번째 값의 연산 결과를 선택된 스택에 push한다.

DI : 선택된 스택을 두 번 pop하여 두 번째 값 / 첫 번째 값의 연산 결과를 선택된 스택에 push한다.

FL flag_id : flag_id의 아이디값을 가지는 플래그를 찍는다.

GO flag_id : flag_id의 아이디값을 가지는 플래그가 찍힌 코드 라인으로 돌아간다.

JU jump_id direction : direction 방향으로 아이디가 jump_id인 점프를 수행한다. (점프: 점프 끝에 다다르기 전까지 모든 코드를 무시함) 이때 direction은 –1 또는 1이며, -1은 위, 1은 아래를 의미한다.

JE jump_id : jump_id의 점프가 진행 중일 때 만나면 점프를 중단하고 direction을 1로 설정한다.

CH jump_id : 현재 스택의 top이 0이면 direction이 1인 JU jump_id를 수행한다. 0이 아니면 무시된다.
```



# 인터프리터 사용 방법

방법1: 파일 탐색기를 이용하는 방법(추천)

1. txt파일을 만든다.
2. txt파일에 코드를 작성한다.
3. txt파일을 DogFootRuler로 드래그한다.


방법2: 명령줄 도구를 이용하는 방법
1. cmd를 실행한다. (관리자 권한은 필요 없음)
2. cd 명령을 통해 실행 파일이 있는 폴더로 이동한다.
3. dogfootruler "소스 코드 경로"를 입력한다. (예: dogfootruler "Sample Codes\구구단.txt")