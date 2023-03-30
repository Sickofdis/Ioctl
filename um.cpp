#include <iostream>
#include <Windows.h>


#define IO_SEND_MESSAGE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x996, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define Print(a1) std::cout << a1 << std::endl;
#define IO_NAME "\\\\.\\RandomDriver"

class Driver {

private:  

    HANDLE hDriver;

public:


    Driver(LPCSTR Dpath) {

        hDriver = CreateFileA(Dpath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
        Print(GetOutput());
    }
 

    DWORD GetOutput() {

        if (hDriver == INVALID_HANDLE_VALUE)
            return 0;

        ULONG Output;
        DWORD Return;

        if(DeviceIoControl(hDriver, IO_SEND_MESSAGE, 0, 0, &Output, sizeof(Output),&Return, NULL)) {

            CloseHandle(hDriver);
            return Output;
        }
        return 0;
   
    }



};


int main()
{
    
    Driver Driiver = Driver(IO_NAME);

    while (true) {

    }
    
}

