#include <iostream>
#include <Windows.h>
#include <string>
#include <TlHelp32.h>

using namespace std;

DWORD GetModuleBase(const wchar_t* ModuleName, DWORD ProcessId) {
    //fornece diversas infos sobre o modulo
    MODULEENTRY32 ModuleEntry = { 0 };

    // obtem uma fotografia ed todos os modulos de um determinado processo
    HANDLE SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessId);

    if (!SnapShot)
        return NULL;

    ModuleEntry.dwSize = sizeof(ModuleEntry);

    // pegar o primeiro modulo do processo
    if (!Module32First(SnapShot, &ModuleEntry))
        return NULL;

    do
    {
        // verificar se o nome do modulo bate com o nome que estamos procurando
        if (!wcscmp(ModuleEntry.szModule, ModuleName)) {
            CloseHandle(SnapShot);
            return (DWORD)ModuleEntry.modBaseAddr; // retorna o addr base do modulo procurado
        }
    } while (Module32Next(SnapShot, &ModuleEntry));

    // nao encotrou nenhum modulo com nome especificado
    CloseHandle(SnapShot);
    return NULL;
}

DWORD PID;
int hp;

int main()  
{
    HWND hWnd = FindWindowA(0, ("HitHealth"));

    GetWindowThreadProcessId(hWnd, &PID); // obtem o process id  e joga na variavel PID
    HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
    DWORD clientSoftware = GetModuleBase(L"HitHealth.exe", PID); // obtem o endereco base do modulo especificado no caso hithealth.exe . Vai ser sempre o Modulo + Offset
    cout << hex << clientSoftware << endl;

    DWORD baseAddress = 0x0115FE50;
    cout << hex << baseAddress << endl;

    // fazer a leitura  a cada 100ms  para obter o valor  sempre atualizado
    while (true)
    {
        ReadProcessMemory(pHandle, (PBYTE*)baseAddress, &hp, sizeof(int), 0);
        cout << dec << hp << endl;
        Sleep(100);
        system("CLS");
    }
    system("Pause");
}

