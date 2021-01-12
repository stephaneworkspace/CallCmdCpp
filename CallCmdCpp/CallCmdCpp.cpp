// CallCmdCpp.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>

// system
#include <cstdlib>
#include <fstream>
#include <iostream>

// sprintf
#include <cstdio>
#include <limits>
#include <cstdint>
#include <cinttypes>

// CreateProcess
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

/*int main()
{
    // create a string, i.e. an array  of 50 char
    char command[50];

    // this will 'fill' the string command with the right stuff,
    // assuming myFile and convertedFile are strings themselves
    sprintf(command, "convert -in %s -out %s -n -e -h", myFile, convertedFile);

    // system call
    system(command);

    //std::system("ls -l >test.txt"); // execute the UNIX command "ls -l >test.txt"
    //std::cout << std::ifstream("test.txt").rdbuf();
}*/



void _tmain(int argc, TCHAR * argv[])
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (argc != 2)
	{
		printf("Usage: %s [cmdline]\n", argv[0]);
		return;
	}

	// Start the child process. 
	if (!CreateProcess(NULL,   // No module name (use command line)
		argv[1],        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return;
	}

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

/*
int main()
{
    std::cout << "Hello World!\n";
}*/

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
