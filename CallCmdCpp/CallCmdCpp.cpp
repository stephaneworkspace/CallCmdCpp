// CallCmdCpp.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//
// str_cpy unsafe
#pragma warning(disable:4996)

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

// https://stackoverflow.com/questions/5419356/redirect-stdout-stderr-to-a-string/5419409#5419409
//#include <unistd.h>
#include<io.h> // #include <unistd.h> https://stackoverflow.com/questions/341817/is-there-a-replacement-for-unistd-h-for-windows-visual-c
#include <fcntl.h>
//#include <stdio.h>
#include <string>

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

/*
call BeginCapture() when you need to start capture
call EndCapture() when you need to stop capture
call GetCapture() to retrieve captured output
*/

class StdCapture
{
public:
    StdCapture(): m_capturing(false), m_init(false), m_oldStdOut(0), m_oldStdErr(0)
    {
        m_pipe[READ] = 0;
        m_pipe[WRITE] = 0;
        if (_pipe(m_pipe, 65536, O_BINARY) == -1)
            return;
        m_oldStdOut = _dup(_fileno(stdout));
        m_oldStdErr = _dup(_fileno(stderr));
        if (m_oldStdOut == -1 || m_oldStdErr == -1)
            return;

        m_init = true;
    }

    ~StdCapture()
    {
        if (m_capturing)
        {
            EndCapture();
        }
        if (m_oldStdOut > 0)
            _close(m_oldStdOut);
        if (m_oldStdErr > 0)
            _close(m_oldStdErr);
        if (m_pipe[READ] > 0)
            _close(m_pipe[READ]);
        if (m_pipe[WRITE] > 0)
            _close(m_pipe[WRITE]);
    }


    void BeginCapture()
    {
        if (!m_init)
            return;
        if (m_capturing)
            EndCapture();
        fflush(stdout);
        fflush(stderr);
        _dup2(m_pipe[WRITE], _fileno(stdout));
        _dup2(m_pipe[WRITE], _fileno(stderr));
        m_capturing = true;
    }

    bool EndCapture()
    {
        if (!m_init)
            return false;
        if (!m_capturing)
            return false;
        fflush(stdout);
        fflush(stderr);
        _dup2(m_oldStdOut, _fileno(stdout));
        _dup2(m_oldStdErr, _fileno(stderr));
        m_captured.clear();

        std::string buf;
        const int bufSize = 1024;
        buf.resize(bufSize);
        int bytesRead = 0;
        if (!_eof(m_pipe[READ]))
        {
            bytesRead = _read(m_pipe[READ], &(*buf.begin()), bufSize);
        }
        while(bytesRead == bufSize)
        {
            m_captured += buf;
            bytesRead = 0;
            if (!_eof(m_pipe[READ]))
            {
                bytesRead = _read(m_pipe[READ], &(*buf.begin()), bufSize);
            }
        }
        if (bytesRead > 0)
        {
            buf.resize(bytesRead);
            m_captured += buf;
        }
        m_capturing = false;
        return true;
    }

    std::string GetCapture() const
    {
        std::string::size_type idx = m_captured.find_last_not_of("\r\n");
        if (idx == std::string::npos)
        {
            return m_captured;
        }
        else
        {
            return m_captured.substr(0, idx+1);
        }
    }

private:
    enum PIPES { READ, WRITE };
    int m_pipe[2];
    int m_oldStdOut;
    int m_oldStdErr;
    bool m_capturing;
    bool m_init;
    std::string m_captured;
};
/*
void _tmain(int argc, TCHAR * argv[])
{
    StdCapture std_capture;
    std_capture.BeginCapture();

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
    std_capture.EndCapture();

    std::cout << "OK" << std::endl;
    std::cout << std_capture.GetCapture() << std::endl;
    std::cout << "OK" << std::endl;

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}
*/

char* SizeAllocatorTxt(int sizeTab);

char* SizeAllocatorTxt(int sizeTab)
{
    return (char*) malloc(sizeTab + 1); // sizeof(char) * // == 1
}

// CA2CT http://msdn.microsoft.com/en-us/library/87zae4a3%28VS.80%29.aspx
#include <atlbase.h>
#include <atlconv.h>

int bridge(char** out_str); // REMOVE mutable if not nececarry -> OO onyl

int bridge(char** out_str) {
    StdCapture std_capture;
    std_capture.BeginCapture();

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

    const char *cmd = "ls -l";

	// Start the child process. 
	if (!CreateProcess(NULL,   // No module name (use command line)
		CA2CT(cmd),        // Command line
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
		return 1;
	}

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);
    std_capture.EndCapture();

    // const char* cstr = std_capture.GetCapture().c_str();
    std::string str = std_capture.GetCapture();
    char* cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());
    // do stuff
    int size = 255;
    *out_str = SizeAllocatorTxt(str.length()); // TODO TEST DEBUGGER
    if (*out_str == NULL)
    {
        std::cout << "Echec de l'allocation de memoire." << std::endl;
        return 2;
        // TODO EXIT PROGRAM
    }
    //**out_str = &cstr; // DON'T WORK
    strcpy(*out_str, str.c_str());
    //printf("Address of out_str = %u\n", out_str);

    delete[] cstr;

    //free(out_str); // au cas ou pour plus tard


	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

    // TODO status code 0 or 1
    return 0;
}


int main()
{
    char* cstr_temp = NULL;
    bridge(&cstr_temp);
    std::cout << cstr_temp << std::endl;
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
