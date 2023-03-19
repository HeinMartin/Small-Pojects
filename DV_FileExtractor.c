#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdint.h>

/**
 * TODO: 
 * choose size for new_dir and dest_file by the length of the elements / paths!
 * possibly giving paths by console?
 * automatically create the "Gesammelt" folder
 */



int checkFile(char *toCheck, char *ref) {

    // char ref[] = ".ipynb";
    size_t ref_len = strlen(ref);

    // char toCheck[] = "Hallo.ipynb";
    size_t toCheck_len = strlen(toCheck);

    uint8_t start = toCheck_len - ref_len;

    char sliced[] = ""; // = s + (s_len - ref_len);
    strcpy(sliced, toCheck + start);

    return strcmp(sliced, ref); // 0 if strings are equal
}

int copyFileTo(char *srcPath, char *destPath) {

    FILE *srcFile = fopen(srcPath, "r");
    if (srcFile == NULL) {
        printf("Could not open source file: %s\n", srcFile);
        return -1;
    }

    FILE *destFile = fopen(destPath, "w");
    if (destFile == NULL) {
        printf("Could not open destination file: %s\n", destFile);
        return -1;
    }

    char content;
    content = fgetc(srcFile);
    while (content != EOF) {
        fputc(content, destFile);
        content = fgetc(srcFile);
    }

    fclose(srcFile);
    fclose(destFile);

    printf("Contents succesfully copied!\n");

    return 0;
}


// find all files at the and of directory tree and return their file names
char *findFiles(char *dir_name, char *destPath_name) {

    struct dirent *de;
    
    DIR *dir = opendir(dir_name);
    if (dir == NULL) {
        printf("Could not open directory %s", dir_name);
        return NULL;
    }

    char *ref = ".ipynb";
    // uint8_t i = 0; // not necessary, since the '.' will be checked!
    while ( (de = readdir(dir)) != NULL ) {

        printf("\n");
        
        char *dir_element = de->d_name;
        printf("current dir element: %s\n", dir_element);

        char new_dir[400] = "";
        strcat(new_dir, dir_name);
        strcat(new_dir, "\\");
        strcat(new_dir, dir_element);

        printf("new dir: (len %i) %s\n", strlen(new_dir), new_dir);
        printf("old dir:%s\n", dir_name);

        if (checkFile(dir_element, ref) == 0) {
            // file found
            printf("%s found notebook!\n", dir_element);
            
            // copy to new destination

            char dest_file[400] = "";
            strcat(dest_file, destPath_name);
            strcat(dest_file, "\\");
            strcat(dest_file, dir_element);
            printf("copy \"%s\"\n to \"%s\"\n", new_dir, dest_file);
            
            copyFileTo(new_dir, dest_file);

        } else {
            // File not found
            
            // check, if there is a sub-directory, i.e. check for '.'
            if (strchr(dir_element, '.') != NULL) {
                printf("\'.\' detected!\n");
                continue;
            }

            // it is a sub-directory => try searching deeper
            findFiles(new_dir, destPath_name);
        }
    }

    closedir(dir);

    return 0;
}


/*
// some snippet from forum to open file dialog window: https://cboard.cprogramming.com/c-programming/136029-openfiledialog-window-c.html

#include <windows.h>

CHAR FileName[MAX_PATH];
 
 
// pick a program file
INT ChooseProgram(PCHAR FileName){ 
    
    OPENFILENAME  ofn;        
    memset(&ofn,0,sizeof(ofn));

    ofn.lStructSize     = sizeof(ofn);
    ofn.hwndOwner       = NULL;
    ofn.hInstance       = gInst;
    ofn.lpstrFilter     = L"Text Files\0*.txt\0\0";    
    ofn.lpstrFile       = cmdl;
    ofn.nMaxFile        = MAX_PATH;
    ofn.lpstrTitle      = L"Please Select A File To Open";
    ofn.Flags           = OFN_NONETWORKBUTTON |
                          OFN_FILEMUSTEXIST |
                          OFN_HIDEREADONLY;
    
    if (!GetOpenFileName(&ofn)) return 0;

    return 1; 
}
*/


// Main program
int main(int argc, char *argv[]) {
    
    // not necessary anymore
    /* prepare here: <path_to_query> <destination_path>
    DV_FileExtractor "C:\Users\marti\Desktop\Rund_ums_Studium\DV-Hiwi\DV-Praktikum\Abgaben\Praktikum 5_ Messung der Fallbeschleunigung (2)\Abgaben" "C:\Users\marti\Desktop\Rund_ums_Studium\DV-Hiwi\DV-Praktikum\Abgaben\Praktikum 5_ Messung der Fallbeschleunigung (2)\Gesammelt"
    */
    
    // argv[0] = "DV_FileExtractor" !
    // printf("printing here argv:\n");
    // for (int i=0; i < argc; ++i) {
    //     printf("%s\n", argv[i]);
    // }

    // problem with excape sequences ...
    char *general_path = "C:/Users/marti/Desktop/Rund_ums_Studium/DV-Hiwi/DV-Praktikum/Abgaben/Praktikum 12_ Gammastrahlung von einem aktiven Galaxienkern (2)/";
    
    char *dir_apdx = "Abgaben";
    char *dest_apdx = "Gesammelt";

    char dir_name[400] = "";
    strcat(dir_name, general_path);
    strcat(dir_name, dir_apdx);
    
    char destPath_name[400] = "";
    strcat(destPath_name, general_path);
    strcat(destPath_name, dest_apdx);

    // char *dir_name = argv[1];
    // char *destPath_name = argv[2];

    printf("query directory: \"%s\"\n", dir_name);
    printf("destination directory: \"%s\"\n", destPath_name);
    findFiles(dir_name, destPath_name);

    return 0;
}