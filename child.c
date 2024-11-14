#include <windows.h>
#include <stdio.h>

#define SIZE 4096
int main() {
    // Open the existing file mapping (it must have the same name as in the parent process)
    HANDLE hMapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,   // Read/write access
        FALSE,                 // Do not inherit the handle
        NULL);                 // Name of the mapping (NULL means unnamed)

    if (hMapFile == NULL) {
        printf("Error opening file mapping object (%d)\n", GetLastError());
        return 1;
    }

    // Map the shared memory into the address space of the child process
    char *shared_memory = (char *)MapViewOfFile(
        hMapFile,              // Handle to the file mapping object
        FILE_MAP_ALL_ACCESS,   // Read/write access
        0,                     // High-order DWORD of file offset
        0,                     // Low-order DWORD of file offset
        SIZE);                 // Number of bytes to map

    if (shared_memory == NULL) {
        printf("Error mapping view of file (%d)\n", GetLastError());
        CloseHandle(hMapFile);
        return 1;
    }

    // Child process reads the shared memory
    printf("Child reads: %s\n", shared_memory);

    // Clean up
    UnmapViewOfFile(shared_memory);
    CloseHandle(hMapFile);

    return 0;
}//
// Created by Usuario on 11/14/2024.
//
