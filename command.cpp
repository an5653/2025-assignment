#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#define SIZE 28 

int CommandFind(const string& command, string ulist[], string dlist[], int size, int& found) {
    for (int i = 0; i < size; i++) {
        if (ulist[i].find(command) != string::npos) {
            if (ulist[i].compare(command) == 0)
            {
                found = 1;
                return i;
            }
            
        }
        if (dlist[i].find(command) != string::npos) {
            if (dlist[i].compare(command) == 0)
            {
                found = 0;
                return i;
            }
            
        }
    }
    return -1;
}

int main() {
    ifstream fin("command.inp");
    ofstream fout("command.out");

    string uco[SIZE] = {
        "ls", "mkdir", "rmdir", "rm", "cp", "mv", "clear", "pwd", "cat", "man",
        "date", "find", "grep", "more", "diff", "ed", "sort", "lsattr", "pushd", "popd",
        "ps", "kill", "halt", "ifconfig", "fsck", "free", "debugfs", "lpr"
    };

    string dco[SIZE] = {
        "dir", "md", "rd", "del", "copy", "rename", "cls", "cd", "type", "help",
        "time", "find", "findstr", "more", "comp", "edlin", "sort", "attrib", "pushd", "popd",
        "taskmgr", "tskill", "shutdown", "ipconfig", "chkdsk", "mem", "scandisk", "print"
    };

    int time ;
    string line;


    getline(fin, line);
    time = stoi(line);

    for (int i = 0; i < time; i++) {
        getline(fin, line);

        int found;
        int index = CommandFind(line, uco, dco, SIZE, found);

        if (index != -1) {
            if (found == 1)
                fout << line << " -> " << dco[index] << endl;
            else
                fout << line << " -> " << uco[index] << endl;
        }
    }

    fin.close();
    fout.close();

    return 0;
}
