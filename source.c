#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

enum {
	MAX_STACK_SIZE = 100, // 스택당 사이즈 제한
	MAX_STACK_NUM = 100, // 스택 개수
	MAX_STRING_SIZE = 1000, // 각 줄당 길이 제한
	MAX_LINE_NUM = 1000, // 소스 코드 줄 수 제한
	MAX_WORD_SIZE = 100, // 단어 길이 제한
	MAX_WORD_NUM = 100, // 단어 개수 제한
	CMD_LENGTH = 3, // 명령어 길이 제한 (NULL포함)
	MAX_FLAG_NUM = 1000 // 플래그 개수 제한
};
long long int stacks[MAX_STACK_NUM][MAX_STACK_SIZE];
int cursor[MAX_STACK_NUM] = { 0 }; // 각 스택에 대한 커서

int flags[MAX_FLAG_NUM] = { 1 };



int currentStack = 0; // SE를 통해 선택된 현재 스택
int currentLine = 1;
int currentDirection = 1; // 소스 코드의 진행 방향; 1은 아래로, -1은 위로
bool isJumping = false; // JU, CH등에 의한 점프중 여부
int currentJumpId = 0; // JU, CH등에 의한 점프의 현재 ID

int set_flag(int flagId, int lineno) {
	if (flagId <= 0) return -1;
	flags[flagId] = lineno;
	return 0;
}
int get_flag(int flagId) {
	if (flagId < 0) return -1;
	if (flagId == 0) return 1;
	if (flagId > 0) return flags[flagId];
	return -1;
}

long long int stack_top(int stackId) {
	return stacks[stackId][cursor[stackId]-1];
}
long long int stack_pop(int stackId) {
	return stacks[stackId][--cursor[stackId]];
}
int stack_push(int stackId, long long int i) {
	stacks[stackId][cursor[stackId]++] = i;
	return cursor[stackId];
}

/**
 * 문자열이 숫자인지 아닌지 검사합니다.
 * @param input: 숫자인지 검사할 문자열
 * @returns 숫자가 아니면 false, 숫자이면 true
 */
bool checkNumber(char* input) {
	int idx = 0;
	while (input[idx] != '\0') {
		if (!isdigit(input[idx])&&input[idx]!='-'&&input[idx]!='+') {
			printf("%s is not a number\n", input);
			return false;
		}
		idx++;
	}
	return true;
}

/** 
 * @param line: 현재 줄 문자열
 * @param lineno: 현재 줄 번호
 * @returns 예외 발생시 false, 정상 실행시 true
 *
 */
bool processLine(char* line, int lineno) { 
	
	int idx = 0;
	char words[MAX_WORD_NUM][MAX_WORD_SIZE];
	int wordIdx = 0;
	while (line[idx] != '\n' && line[idx] != '\0') { // 현재 줄의 끝까지 반복
		char word[MAX_WORD_SIZE];
		int start = idx;
		char current;
		while ((current=line[idx]) != ' ' && current != '\n' && current != '\0') { // 단어 파싱: 띄어쓰기, NULL, 개행을 만나면 토큰을 끊고 한 단어로 저장
			word[idx - start] = current;
			idx++;
		}
		
		word[idx-start] = '\0'; // 단어의 끝에 NULL문자 저장
		strcpy(words[wordIdx++], word);
		if (current == '\0')break; // 단어 파싱 while문에 의해 idx가 임의로 증가하였으므로 바깥 while문에서 조건 체크가 되지 않을 수 있음
		idx++;
	}
	char cmd[CMD_LENGTH]; // 첫 토큰, 즉 명령어를 저장
	strcpy(cmd, words[0]);

	if (isJumping) { // JU또는 CH에 의해 점프가 진행 중일 때, 유효한 JE를 만나면 점프 중단, 방향을 1로 설정
		if (!strcmp(cmd, "JE")) {
			if (!checkNumber(words[1])) {
				return false;
			}
			int jumpId = atoi(words[1]);
			if (currentJumpId == jumpId) { // 현재 진행중인 점프와 같은 ID일때만 점프를 끝냅니다. 다를 경우 점프를 지속함.
				isJumping = false;
				currentDirection = 1;
			}
			return true;
		}
		else {
			return true;
		}
		printf("Unexpected Exception occurred at %d", lineno);
		return false;
	}
	else if (!strcmp(cmd, "SE")) {
		if (!checkNumber(words[1])) {
			return false;
		}
		int temp = atoi(words[1]);
		
		if (temp > MAX_STACK_NUM) {
			printf("SE failed: max stack num is %d\n", MAX_STACK_NUM - 1);
			return false;
		}

		currentStack = temp;
		return true;
	}
	else if (!strcmp(cmd, "PU")) {
		if (!checkNumber(words[1])) {
			return false;
		}
		stack_push(currentStack, atoi(words[1]));
		return true;
	}
	else if (!strcmp(cmd, "PO")) {
		stack_pop(currentStack);
		return true;
	}
	else if (!strcmp(cmd, "CO")) {
		if (!checkNumber(words[1])) {
			return false;
		}
		stack_push(atoi(words[1]), stack_top(currentStack));
		return true;
	}
	else if (!strcmp(cmd, "PL")) {
		long long int a = stack_pop(currentStack);
		long long int b = stack_pop(currentStack);
		stack_push(currentStack, b + a);
		return true;
	}
	else if (!strcmp(cmd, "MI")) {
		long long int a = stack_pop(currentStack);
		long long int b = stack_pop(currentStack);
		stack_push(currentStack, b - a);
		return true;
	}
	else if (!strcmp(cmd, "MU")) {
		long long int a = stack_pop(currentStack);
		long long int b = stack_pop(currentStack);
		stack_push(currentStack, b * a);
		return true;
	}
	else if (!strcmp(cmd, "MO")) {
		long long int a = stack_pop(currentStack);
		long long int b = stack_pop(currentStack);
		stack_push(currentStack, b % a);
		return true;
	}
	else if (!strcmp(cmd, "DI")) {
		long long int a = stack_pop(currentStack);
		long long int b = stack_pop(currentStack);
		stack_push(currentStack, b / a);
		return true;
	}
	else if (!strcmp(cmd, "IN")) {
		printf("Input:");
		long long int a;
		scanf("%lld", &a);
		stack_push(currentStack, a);
		return true;
	}
	else if (!strcmp(cmd, "OU")) {
		printf("%lld", stack_top(currentStack));
		return true;
	}
	else if (!strcmp(cmd, "OA")) {
		printf("%c", stack_top(currentStack));
		return true;
	}
	else if (!strcmp(cmd, "FL")) {
		if (!checkNumber(words[1])) {
			return false;
		}
		int flagId = atoi(words[1]);
		int retn=set_flag(flagId, lineno);
		if (retn != 0) {
			printf("Invalid flag id: %d\n", flagId);
			return false;
		}
		return true;
	}
	else if (!strcmp(cmd, "GO")) {
		if (!checkNumber(words[1])) {
			return false;
		}
		int flagId = atoi(words[1]);
		int retn = get_flag(flagId);
		if (retn == -1 || retn == 0) {
			printf("Invalid flag id: %d\n", flagId);
			return false;
		}
		currentLine = retn;
		return true;
	}
	else if (!strcmp(cmd, "JU")) {
		if (!checkNumber(words[1]) || !checkNumber(words[2])) {
			return false;
		}
		int direction = atoi(words[2]);
		if (direction == -1 || direction == 1) {
			currentJumpId = atoi(words[1]);
			currentDirection = direction;
			isJumping = true;
		}
		else {
			printf("Invalid direction: %d", direction);
			return false;
		}
		return true;
	}
	else if (!strcmp(cmd, "CH")) {
		if (!checkNumber(words[1])) {
			return false;
		}
		if (!stack_top(currentStack)) {
			isJumping = true;
			currentJumpId = atoi(words[1]);
		}
		return true;
	}
	else if (!strcmp(cmd, "CM")) {
		// do nothing
		return true;
	}
	return true;
}

int main(int argc, char** argv) {

	if (argc == 2) { // 소스 코드 파일 경로가 제공되었을 경우
		FILE* file;
		int totalLines;
		char lines[MAX_LINE_NUM][MAX_STRING_SIZE];
		int i = 0;
		currentLine = 1;
		currentDirection = 1; 
		file = fopen(argv[1], "r");
		while (fgets(lines[i], MAX_STRING_SIZE, file) != NULL) {
			i++;
		}
		totalLines = i;

		while (currentLine <= totalLines) {
			if (!processLine(lines[currentLine - 1], currentLine))break;
			currentLine += currentDirection; // 현재 방향으로 읽기 진행
		}

		printf("\n\nEnd of the program");
		system("pause>nul");
		
	}
	else { // 경로가 제공되지 않았다면 프로그램의 사용 방법을 설명 (명령줄 도구에서만)
		printf("Usage: DogFootRuler <file_path>");
	}
	
}
