#include <windows.h>
#include <stdio.h>
#include <string.h>

#define SIZE 4096

int main() {
    // Create a named file mapping object for shared memory
    HANDLE hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // Use system paging file
        NULL,                    // Default security attributes
        PAGE_READWRITE,          // Allow read/write access
        0,                       // High-order DWORD of size (not used)
        SIZE,                    // Low-order DWORD of size (4 KB)
        NULL);                   // Name of the mapping (NULL means unnamed)

    if (hMapFile == NULL) {
        printf("Error creating file mapping object (%d)\n", GetLastError());
        return 1;
    }

    // Map the shared memory into the process's address space
    char *shared_memory = (char *)MapViewOfFile(
        hMapFile,                // Handle to the file mapping object
        FILE_MAP_ALL_ACCESS,     // Read and write access
        0,                       // High-order DWORD of file offset
        0,                       // Low-order DWORD of file offset
        SIZE);                   // Number of bytes to map

    if (shared_memory == NULL) {
        printf("Error mapping view of file (%d)\n", GetLastError());
        CloseHandle(hMapFile);
        return 1;
    }

    // Create the child process
    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;

    // Command to execute (this could be a separate executable or command line)
    // In this example, we are creating a simple process that will read shared memory.
    if (!CreateProcess(
        NULL,             // Application name
        "child.exe",      // Command line to run the child process
        NULL,             // Process security attributes
        NULL,             // Thread security attributes
        FALSE,            // Inherit handles
        0,                // Creation flags
        NULL,             // Environment
        NULL,             // Current directory
        &si,              // Startup info
        &pi               // Process information
    )) {
        printf("CreateProcess failed (%d)\n", GetLastError());
        UnmapViewOfFile(shared_memory);
        CloseHandle(hMapFile);
        return 1;
    }

    // Parent process writes to shared memory
    strcpy(shared_memory, "Hello, child process!");

    // Wait for child process to complete
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Clean up
    UnmapViewOfFile(shared_memory);
    CloseHandle(hMapFile);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
