#include <iostream>
#include <string>
#include <Windows.h>
#include <wininet.h>
#include <TlHelp32.h>
#include <random>
#include "xor.cpp" // Hinweis: Die Datei "xor.cpp" muss noch angepasst werden, um den korrekten XorStr-Mechanismus bereitzustellen.
#include <atlsecurity.h>

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "urlmon.lib")

using namespace std;

// Globale Variablen
string currVersion = XorStr("0.1");
string UrlFuerCheck = XorStr("ZENSIERT (von Pastebin)");

// Funktionen-Deklarationen
string GenerateRandomString(size_t length);
void CheckOrCreateFolder();
void CheckVersion(const string& version);
string DownloadString(const char* url);
void StartNewVersionAndDeleteOld(const wstring& newFilePath);
void RandomizeExeNameAndConsoleTitle();
string GetHWID();
static string get_hwid();
bool IsProcessRunning(const wstring& processName);
void SimulateKeyPress(WORD vkCode, bool bExtended = false);
void SimulateKeystrokes(const string& input);
void gtaRunningCheck();


// Hauptfunktion
int main() {
    // Gemeinsame Operationen unabhängig von der Bedingung
    CheckOrCreateFolder();
    CheckVersion(currVersion);
    RandomizeExeNameAndConsoleTitle();

    gtaRunningCheck();

    // Entscheidung basierend auf HWID und SID
    bool isValidCredentials = (GetHWID() == XorStr("{ZENSIERT}") && get_hwid() == XorStr("ZENSIERT")); // meine

    if (isValidCredentials) { // wenn meine HWID
        cout << "Hello Scotch!\n";
        while (isValidCredentials) {
            if (GetAsyncKeyState('O') & 0x8000) {
                Sleep(100);
                SimulateKeyPress(0x54); // Drücke die Taste 'T'
                Sleep(100);
                SimulateKeystrokes("/e reaching"); // Simuliere Tastenanschläge
                SimulateKeyPress(VK_RETURN); // Drücke die Eingabetaste
            }
            Sleep(100); // Kurze Pause, um die CPU nicht zu überlasten
        }
    }
    else { // andere HWID
        while (true) {
            if (GetAsyncKeyState('O') & 0x8000) {
                Sleep(100);
                SimulateKeyPress(0x54); // Drücke die Taste 'T'
                Sleep(100);
                SimulateKeystrokes("/e reaching"); // Simuliere Tastenanschläge
                SimulateKeyPress(VK_RETURN); // Drücke die Eingabetaste
            }
            Sleep(100); // Kurze Pause, um die CPU nicht zu überlasten
        }
    }

    return 0;
}



// Funktion zur Erzeugung eines zufälligen Strings
string GenerateRandomString(size_t length) {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    random_device rd;
    default_random_engine rng(rd());
    uniform_int_distribution<> dist(0, sizeof(charset) - 2);
    string randomString;
    randomString.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        randomString += charset[dist(rng)];
    }
    return randomString;
}

// Funktion zur Überprüfung und Erstellung eines Ordners
void CheckOrCreateFolder() {
    string folderPath = XorStr("C:\\Scotch");

    DWORD dwAttrib = GetFileAttributesA(folderPath.c_str());
    if (dwAttrib == INVALID_FILE_ATTRIBUTES || !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
        if (!CreateDirectoryA(folderPath.c_str(), NULL)) {
            return; // Fehler beim Erstellen des Ordners
        }
    }
}

// Funktion zur Überprüfung der aktuellen Programmversion
void CheckVersion(const string& version) {
    string serverVersion = DownloadString(UrlFuerCheck.c_str());
    if (serverVersion != version) {
        MessageBoxA(NULL, XorStr("Your version is outdated!"), XorStr("Old version!"), MB_ICONERROR);
        ShowWindow(::GetConsoleWindow(), SW_SHOW);
        cout << XorStr("Do you want to load the newest version? (Y/N)\n");
        string input;
        getline(cin, input);
        if (input == "Y" || input == "y") {
            string newFilePath = XorStr("C:\\Scotch\\Scotch_updated.exe");
            if (S_OK == URLDownloadToFile(NULL, XorStr(L"https://example.com/new_version.exe"), XorStr(L"C:\\Scotch\\Scotch_updated.exe"), 0, NULL)) {
                MessageBoxW(0, XorStr(L"You can find the new version here: C:\\Scotch\\Scotch_updated.exe"), XorStr(L"UPDATED"), MB_ICONINFORMATION);
                ShowWindow(::GetConsoleWindow(), SW_HIDE);
                StartNewVersionAndDeleteOld(wstring(newFilePath.begin(), newFilePath.end()));
            }
            else {
                MessageBoxW(0, XorStr(L"Failed to download new version!"), XorStr(L"FAILED"), MB_ICONERROR);
                exit(1);
            }
        }
        else if (input == "N" || input == "n") {
            exit(0);
        }
        else {
            cout << XorStr("Wrong input!\n");
            Sleep(2000);
            exit(1);
        }
    }
}


void gtaRunningCheck()
{
    wstring processName = L"GTA5.exe";
    if (IsProcessRunning(processName)) {
        wcout << processName << " is running.\n";
    }
    else {
        MessageBoxA(0, "GTA5.exe is not running", "ERROR", MB_ICONERROR);
        exit(1);
    }
}

// Funktion zum Herunterladen einer Zeichenfolge von einer URL
string DownloadString(const char* url) {
    HINTERNET hInternet = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        return "";
    }

    HINTERNET hConnect = InternetOpenUrlA(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hConnect) {
        InternetCloseHandle(hInternet);
        return "";
    }

    char buffer[1024];
    DWORD bytesRead;
    string result;

    while (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead != 0) {
        buffer[bytesRead] = '\0';
        result.append(buffer);
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return result;
}

// Funktion zum Umbenennen der aktuellen ausführbaren Datei
void RandomizeExeNameAndConsoleTitle() {
    char currentPath[MAX_PATH];
    GetModuleFileNameA(NULL, currentPath, MAX_PATH);

    string newFileName = GenerateRandomString(10) + ".exe";
    string newFilePath = string(currentPath).substr(0, string(currentPath).find_last_of("\\/") + 1) + newFileName;

    if (MoveFileA(currentPath, newFilePath.c_str())) {
        string newConsoleTitle = GenerateRandomString(15);
        SetConsoleTitleA(newConsoleTitle.c_str());
    }
    else {
        DWORD error = GetLastError();
        string errorMessage = "Error renaming file. Error code: " + to_string(error);
        MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_ICONERROR);
    }

    HWND consoleWindow = GetConsoleWindow();
    LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
    style &= ~(WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME);
    SetWindowLong(consoleWindow, GWL_STYLE, style);
    EnableMenuItem(GetSystemMenu(consoleWindow, FALSE), SC_CLOSE, MF_ENABLED);

    RECT rect;
    GetWindowRect(consoleWindow, &rect);
    MoveWindow(consoleWindow, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
}

// Funktion zur Simulation eines Tastendrucks
void SimulateKeyPress(WORD vkCode, bool bExtended) {
    INPUT inputs[2] = {};
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = vkCode;
    inputs[0].ki.dwFlags = bExtended ? KEYEVENTF_EXTENDEDKEY : 0;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = vkCode;
    inputs[1].ki.dwFlags = bExtended ? KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP : KEYEVENTF_KEYUP;

    SendInput(2, inputs, sizeof(INPUT));
}

// Funktion zur Simulation von Tastenanschlägen
void SimulateKeystrokes(const string& input) {
    for (char c : input) {
        if (c == '/') {
            SimulateKeyPress(VK_DIVIDE, true);
        }
        else {
            SimulateKeyPress(VkKeyScanA(c) & 0xFF, false);
        }
        Sleep(10); // Kleine Pause zwischen den Tastendrücken
    }
}

// Funktion zur Überprüfung der Hardware-ID
string GetHWID() {
    HW_PROFILE_INFO hwProfileInfo;
    GetCurrentHwProfile(&hwProfileInfo);
    wstring hwidWString = hwProfileInfo.szHwProfileGuid;
    return string(hwidWString.begin(), hwidWString.end());
}

static string get_hwid()
{
    ATL::CAccessToken accessToken;
    ATL::CSid currentUserSid;
    if (accessToken.GetProcessToken(TOKEN_READ | TOKEN_QUERY) &&
        accessToken.GetUser(&currentUserSid))
        return std::string(CT2A(currentUserSid.Sid()));
}

// Funktion zur Überprüfung, ob ein Prozess läuft
bool IsProcessRunning(const wstring& processName) {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return false;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return false;
    }

    do {
        if (wcscmp(pe32.szExeFile, processName.c_str()) == 0) {
            CloseHandle(hProcessSnap);
            return true;
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return false;
}

// Funktion zum Starten der neuen Version und Löschen der alten Datei
void StartNewVersionAndDeleteOld(const wstring& newFilePath) {
    wstring currentPath;
    currentPath.resize(MAX_PATH);
    GetModuleFileNameW(NULL, &currentPath[0], MAX_PATH);

    wstring deleteCommand = XorStr(L"cmd.exe /c timeout /t 2 & del \"") + currentPath + XorStr(L".old\"");

    SHELLEXECUTEINFOW sei = { sizeof(SHELLEXECUTEINFOW) };
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb = XorStr(L"runas");
    sei.lpFile = newFilePath.c_str();
    sei.nShow = SW_SHOWNORMAL;

    if (ShellExecuteExW(&sei)) {
        WaitForSingleObject(sei.hProcess, 5000);
        _wsystem(deleteCommand.c_str());
    }
    else {
        DWORD error = GetLastError();
        wstring errorMessage = XorStr(L"Failed to start new file. Error code: ") + to_wstring(error);
        MessageBoxW(NULL, errorMessage.c_str(), XorStr(L"Error"), MB_ICONERROR);
    }
}