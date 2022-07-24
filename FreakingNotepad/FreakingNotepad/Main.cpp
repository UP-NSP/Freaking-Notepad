//#define DEBUG

#include <Windows.h>
#include <string>
//#include <stdlib>
#include "Enums.h"
#ifdef DEBUG
	#include <iostream>
#endif // DEBUG

HWND MainWindow;

HWND EditVidget;

wchar_t Version[10] = L"1.0.0";

OPENFILENAME fileName;

wchar_t FileNameBuffer[512];

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR ClassName, WNDPROC Procedure);

void AddMenu( HWND hWnd );
void AddObjects( HWND hWnd );

WNDCLASS SoftwareMainClass;

void SetOpenFileParams( HWND hWnd );

void SaveData( LPCWSTR path );
void LoadData( LPCWSTR path );

bool CheckFromIndex(wchar_t String[], wchar_t StringToCheck[], int Index);

int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow )
{
	SoftwareMainClass = NewWindowClass( (HBRUSH)COLOR_WINDOW, LoadCursor( hInst ,IDC_ARROW ), hInst, LoadIcon( hInst, MAKEINTRESOURCE( IDI_ICON2 ) ), L"MainWindowClass", SoftwareMainProcedure );

	if ( !RegisterClass( &SoftwareMainClass ) )
	{
		MessageBox( NULL, L"Cannot register window class!", L"Error", MB_ICONERROR );
		return -1;
	}

	MSG SoftwareMainMessage = { 0 };

	MainWindow = CreateWindow(L"MainWindowClass", L"Freaking Notepad", WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_MINIMIZEBOX, 300, 200, 500, 500, NULL, NULL, hInst, NULL);

	while ( GetMessage( &SoftwareMainMessage, NULL, NULL, NULL ) )
	{
		TranslateMessage( &SoftwareMainMessage );
		DispatchMessage( &SoftwareMainMessage );
	}

	return 0;
}

WNDCLASS NewWindowClass( HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR ClassName, WNDPROC Procedure ) // √‰Â BGColor - background color 
{
	WNDCLASS NewWindow = { 0 };

	NewWindow.lpszClassName = ClassName;
	NewWindow.hInstance = hInst;
	NewWindow.style = NULL;
	NewWindow.hCursor = Cursor;
	NewWindow.hbrBackground = BGColor;
	NewWindow.lpfnWndProc = Procedure;
	NewWindow.lpszMenuName = NULL;

	if (Icon == NULL)
	{	
		MessageBox(NULL, L"Cannot load application icon! Application instance will continue to work.", L"Error", MB_ICONERROR);

		NewWindow.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON2));

		if (NewWindow.hIcon == NULL)
		{
			MessageBox(NULL, L"Application attempted to load icon again, but attempt unexpectedly failed. Application will continue to work.", L"Error", MB_ICONERROR);

		}

	}
	else
		NewWindow.hIcon = Icon;

	
	

	return NewWindow;
}

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		case WM_COMMAND :
		{
			switch ( wp )
			{
				case AboutInfoClicked:
				{
					MessageBox( hWnd, (std::wstring( L"This is a test project, not recommended for everyday use, based on Win32.\n\n\"Freaking Notepad\" is very simple program to write primitive text files.\n\n\t\t\tVersion: ") + std::wstring(Version)).c_str(), L"About", MB_OK);
				}	break;

				case SaveClicked:
				{
					if ( GetSaveFileName( &fileName )  )
					{ 
						if ( CheckFromIndex( FileNameBuffer, (wchar_t*) L".ntp", wcslen(FileNameBuffer) - 4 ) )
						{					
							#ifdef DEBUG
							MessageBox(NULL, L"Extension is scpecified!", L"Info", MB_ICONINFORMATION);
							#endif 
							SaveData( FileNameBuffer );
						}
						else
						{
							int max, i;
							i = wcslen(FileNameBuffer);
							max = i + 4;
							wchar_t postfix[5] = L".ntp";
							for ( int j = 0; i < max; i++, j++)
							{
								*(FileNameBuffer + i) = *(postfix + j);
							}

							SaveData(FileNameBuffer);

						}
					}
					else if (wcslen(FileNameBuffer) != 0)
					{
						DWORD ErrorCode = CommDlgExtendedError(); //GetLastError(); 
						MessageBox(NULL, (std::wstring(L"File wasn't saved! Error code: ") + std::to_wstring(ErrorCode)).c_str(), L"Error", MB_ICONERROR);
					}
				}
				break;

				case LoadClicked:
				{
					if (GetOpenFileName(&fileName))
					{
						if (CheckFromIndex(FileNameBuffer, (wchar_t*)L".ntp", wcslen(FileNameBuffer) - 4))
						{
							LoadData(FileNameBuffer);
						}
						else
						{
							int max, i;
							i = wcslen(FileNameBuffer);
							max = i + 4;
							wchar_t postfix[5] = L".ntp";
							for (; i < max; i++)
							{
								*(FileNameBuffer + i) = *(postfix + (max - i));
							}

							LoadData(FileNameBuffer);

						}
					}
					else if (wcslen(FileNameBuffer) != 0)
					{
						DWORD ErrorCode = CommDlgExtendedError(); //GetLastError(); 
						MessageBox(NULL, (std::wstring(L"File wasn't loaded! Error code: ") + std::to_wstring(ErrorCode)).c_str(), L"Error", MB_ICONERROR);
					}
				}
				break;

				case Set_1kb_Buffer: {
					BytesToIterate = 1024;
					MessageBox(NULL, L"Buffer size is now set to 1024 bytes (1 kilobyte)", L"Info", MB_ICONINFORMATION);
				}				
				break;

				case Set_1mb_Buffer: {
					BytesToIterate = 1024 * 1024;
					MessageBox(NULL, L"Buffer size is now set to 1048576 bytes (1 megabyte)", L"Info", MB_ICONINFORMATION);
				}
				break;
			
				case Set_1gb_Buffer: {
					BytesToIterate = 1024 * 1024 * 1024;
					MessageBox(NULL, L"Buffer size is now set to 1073741824 bytes (1 gigabyte)", L"Info", MB_ICONINFORMATION);
				}
				break;

				case Set_2gb_Buffer: {
					BytesToIterate = 2147483648;
					MessageBox(NULL, L"Buffer size is now set to 2147483648 bytes (2 gigabytes)", L"Info", MB_ICONINFORMATION);
				}
				break;

				case Set_4gb_Buffer: {
					BytesToIterate = (unsigned long)4294967296;
					MessageBox(NULL, L"Buffer size is now set to 4294967296 bytes (4 gigabytes)", L"Info", MB_ICONINFORMATION);
				}
				break;
			}


		}	break;
		case WM_CREATE: {
			AddMenu( hWnd );
			AddObjects( hWnd );
			SetOpenFileParams( hWnd );
			
		}	break;

		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}	break;

		case WM_SIZE: {
			LPRECT rect;
			rect = (LPRECT) new LPRECT;
			//rect = (LPRECT)malloc( sizeof( *rect ) );
			GetClientRect( hWnd, rect );
			//485, 500; 500, 500 ( 15; 0)
#ifdef DEBUG
			MessageBoxW(NULL, std::to_wstring(rect->bottom).c_str(), L"Info", MB_ICONINFORMATION);
			MessageBoxW(NULL, std::to_wstring(rect->right).c_str(), L"Info", MB_ICONINFORMATION);
#endif // DEBUG
			SetWindowPos(EditVidget, NULL, 0, 0, rect->right, rect->bottom, NULL);
			
			delete rect;

		}

		default: {
			return DefWindowProc(hWnd, msg, wp, lp);
		}	break;
	}
}

void AddMenu(HWND hWnd)
{
	HMENU RootMenu = CreateMenu();

	HMENU FileSubMenu = CreateMenu();

	HMENU HelpMenu = CreateMenu();

	AppendMenu(FileSubMenu, MF_STRING, SaveClicked, L"Save" );

	AppendMenu(FileSubMenu, MF_STRING, LoadClicked, L"Load" );

	//AppendMenu(FileSubMenu, MF_SEPARATOR, NULL, NULL );

	AppendMenu(HelpMenu, MF_STRING, AboutInfoClicked, L"About" );

	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)FileSubMenu, L"File");

	HMENU SettingsSubMenu = CreateMenu();

	HMENU BytesToIterateMenu = CreateMenu();

	AppendMenu(BytesToIterateMenu, MF_STRING, Set_1kb_Buffer, L"1024 bytes"); //1 kb

	AppendMenu(BytesToIterateMenu, MF_STRING, Set_1mb_Buffer, L"1048576 bytes"); //1 mb

	AppendMenu(BytesToIterateMenu, MF_STRING, Set_1gb_Buffer, L"1073741824 bytes"); //1 gb

	AppendMenu(BytesToIterateMenu, MF_STRING, Set_2gb_Buffer, L"2147483648 bytes"); //2 gb

	AppendMenu(BytesToIterateMenu, MF_STRING, Set_4gb_Buffer, L"4294967296 bytes"); //4 gb

	AppendMenu(SettingsSubMenu, MF_POPUP, (UINT_PTR)BytesToIterateMenu, L"Bytes to iterate");

	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SettingsSubMenu, L"Settings");

	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)HelpMenu, L"Help");

	SetMenu( hWnd, RootMenu );

	

}

void AddObjects( HWND hWnd )
{
	EditVidget = CreateWindow( L"edit", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL, 0, 0, 485, 500, hWnd, NULL, NULL, NULL );

}

void SaveData( LPCWSTR path )
{
	HANDLE File = CreateFile( path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	
	int TextSize = GetWindowTextLength( EditVidget ) * 2;

	wchar_t* text = new wchar_t[ TextSize ];

	*(text + TextSize) = '\0';

	GetWindowText( EditVidget , text, TextSize );

	DWORD BytesIterated;

	WriteFile( File, text, TextSize * 2, &BytesIterated, NULL );

	CloseHandle( File );

#ifdef DEBUG
	DWORD ErrorCode = GetLastError();

	MessageBox(NULL, (std::wstring(L"Error code: ") + std::to_wstring(ErrorCode)).c_str(), L"Error", MB_ICONERROR );

	MessageBoxA( NULL, std::to_string(TextSize).c_str(), "Debug", MB_ICONINFORMATION);
	MessageBox (NULL, text, L"Debug", MB_ICONINFORMATION);
#endif // DEBUG

	delete [] text;

	MessageBox(NULL, L"File saved successfully!", L"Saved!", MB_ICONINFORMATION);

}

void LoadData( LPCWSTR path )
{
	HANDLE File = CreateFile( path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	DWORD* BytesIterated = new DWORD;

	wchar_t* Buffer = new wchar_t[ BytesToIterate / 2 ]; 

	BOOL result = ReadFile( File, Buffer, BytesToIterate, BytesIterated, NULL );

#ifdef DEBUG
	MessageBox(NULL, Buffer, L"Info", MB_ICONINFORMATION);
#endif 

	if (result != 0)
	{
		
		SetWindowText(EditVidget, Buffer);

	}
	else
	{
		MessageBoxW(NULL, L"Cannot open file! Please try again.", L"Error", MB_ICONERROR);
	}

	CloseHandle( File );

	delete[] Buffer;

	delete BytesIterated;

}
void SetOpenFileParams( HWND hWnd )
{
	ZeroMemory( &fileName, sizeof( fileName ) );

	fileName.lStructSize = sizeof( fileName );

	fileName.hwndOwner = hWnd;

	fileName.lpstrFile = FileNameBuffer;

	fileName.nMaxFile = sizeof( FileNameBuffer );

	fileName.lpstrFilter = L"\'Freaking Notepad\' files(*.ntp)\0*.ntp\0\0";

	fileName.lpstrFileTitle = NULL;

	fileName.nMaxFileTitle = 0;

	fileName.lpstrInitialDir = L"C:/Documents";

	fileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

}

bool CheckFromIndex( wchar_t String[], wchar_t StringToCheck[], int Index )
{
	int CurrentIndex = Index;
	int i = 0;
	int maxIndex = std::wcslen(String);

	if ( !( CurrentIndex < maxIndex ) )
	{
		return false;
	}

	while ( CurrentIndex < maxIndex )
	{
		if (String[CurrentIndex] == StringToCheck[i])
		{
#ifdef DEBUG
			MessageBox(NULL, (std::to_wstring( String[CurrentIndex]) + std::to_wstring( StringToCheck[i]) ).c_str(), L"DEBUG", MB_ICONINFORMATION);
#endif
			CurrentIndex++;
			i++;
		}
		else
		{
			return false;
		}
	}

	return true;
}