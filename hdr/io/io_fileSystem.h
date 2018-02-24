#pragma once

// Get the length of a file
int io_getFileSize(char *fileName);

// Start the packfile system
bool io_startFileSystem();

// Load a text file into a pointer
int io_getFileIntoMemory(char *fileName, char *results);