# My Ls
This code is an implementation of a standard Unix utility that prints directory contents to standard output called *ls*.

The code was written by [Abdussamad Turdixojayev](https://github.com/Abuuu2007).

## Content
- [Description](#description)
- [Usage](#usage)
- [Code Component](#code-component)
- [Main function](#main-function)

## Description
I was given the task of creating My_Ls with two parameters:
1. `-a` - Include directory entries whose names begin with a dot (.).
2. `-t` - Sort by time modified (most recently modified first) before sorting the operands by lexicographical order.

But with all this, I am allowed to use a limited number of functions, namely:

* malloc(3)
* free(3)
* printf(3)
* write(2)
* stat(2)
* lstat(2)
* opendir(2)
* closedir(2)
* readdir(2)

## Usage
### The program will work depending on what parameters are used:
#### If used without parameters:
```
$ ./my_ls
Makefile 
my_ls.c  
my_ls.exe
README.md
```
---
#### If `-a` is used, then hidden files starting with "." will also display:
```
$ ./my_ls -a
.        
..       
Makefile 
my_ls.c  
my_ls.exe
README.md
```
---
#### If `-t` is used, sorting by modification time will occur:
```
$ ./my_ls -t
README.md
my_ls.exe
my_ls.c
Makefile
```
---
#### You can enter these two parameters at the same time:
```
$ ./my_ls -at
README.md
.        
Makefile 
my_ls.c  
my_ls.exe
..
```
---
#### You can also select a directory in the console:
```
$ ./my_ls -at ..
.
my_ls
.vscode
..
```
## Code component
### Analogous to `strlen`
```c
int my_strlen(char* p1){
    int i = 0;
    while(p1[i] != '\0')
        i++;
    return i;
}
```
### Analogous to `strcmp`
```c
int my_strcmp(char* p1, char* p2){
    for(int i = 0; p1[i] != '\0' || p2[i] != '\0'; i++){
        if(p1[i] > p2[i])
            return 1;
        else if(p1[i] < p2[i])
            return -1;
    }
    return 0;
}
```
### Analogous to `strdup`
```c
char* my_strdup(char* p1){
    int len = my_strlen(p1);
    char* res = malloc((len + 1) * sizeof(char));
    int i;
    for(i = 0; i < len; i++)
        res[i] = p1[i];
    res[i] = '\0';
    return res;
}
```
### `BubbleSort` for sorting by modification time
```c
void bubble_sort(forSort* p1, int len){
    for(int i = 0; i < len; i++)
        for(int j = 0; j < len-i-1; j++)
            if(p1[j].int_val > p1[j+1].int_val){
                int int_temp = p1[j].int_val;
                p1[j].int_val = p1[j+1].int_val;
                p1[j+1].int_val = int_temp;
                char* char_temp = p1[j].str_arr;
                p1[j].str_arr = p1[j+1].str_arr;
                p1[j+1].str_arr = char_temp;
            }
}
```
### Checking the entered parameters
```c
void param_check(int argc, char* argv[], bool* time_sort, bool* include_dot, char* dirs[], int* dirs_count){
    for(int i = 1; i < argc; i++){
        if(my_strcmp(argv[i], "-t") == 0)
            *time_sort = true;
        else if(my_strcmp(argv[i], "-a") == 0)
            *include_dot = true;
        else if(my_strcmp(argv[i], "-at") == 0 || my_strcmp(argv[i], "-ta") == 0){
            *time_sort = true;
            *include_dot = true;
        }else{
            dirs[*dirs_count] = argv[i];
            (*dirs_count)++;
        }
    }
}
```
### Function printing directory contents
```c
int list_files(char* dirs, bool time_sort, bool include_dot){
    struct dirent *entry;
    struct stat fileStat;

    stat(dirs, &fileStat);
    if(S_ISREG(fileStat.st_mode))
        return printf("%s\n", dirs)*0;

    DIR *dir = opendir(dirs);
    
    if(dir == NULL)
        return printf("ls: cannot access '%s': No such file or directory\n", dirs)*1;
    
    int count = 0;
    while((entry = readdir(dir)) != NULL)
        if(!(entry->d_name[0] == '.' && !include_dot))
            count++;
    closedir(dir);

    forSort* array = malloc(count * sizeof(forSort));

    int i = 0;      
    dir = opendir(dirs);
    while((entry = readdir(dir)) != NULL){
        if(entry->d_name[0] == '.' && !include_dot)
            continue;
        array[i].str_arr = my_strdup(entry->d_name);
        stat(entry->d_name, &fileStat);
        array[i].int_val = fileStat.st_mtime;
        i++;
    }
    if(time_sort){
        bubble_sort(array, count);
        for(int i = count-1; i >= 0; i--)
            printf("%s\n", array[i].str_arr);
    }else
        for(int i = 0; i < count; i++)
            printf("%s\n", array[i].str_arr);
    closedir(dir);

    for(int i = 0; i < count; i++)
        free(array[i].str_arr);
    free(array);

    return 0;
}
```
### Function containing main code
```c
int my_ls(int argc, char* argv[]){
    struct stat fileStat;
    bool time_sort = false;
    bool include_dot = false;
    int dirs_count = 0;
    char* dirs[argc];

    param_check(argc, argv, &time_sort, &include_dot, dirs, &dirs_count);

    if(dirs_count == 0)
        list_files(".", time_sort, include_dot);
    else if(dirs_count == 1)
        list_files(dirs[0], time_sort, include_dot);
    else{
        for(int i = 0; i < dirs_count; i++){
            stat(dirs[i], &fileStat);
            if(!(S_ISREG(fileStat.st_mode)))
                printf("'%s':\n", dirs[i]);
            list_files(dirs[i], time_sort, include_dot);
            if(i < dirs_count - 1)
                printf("\n");
        }
    }
    return 0;
}
```
## Main function
### Here I return a function containing all the main code called `my_ls`:
```c
int main(int argc, char* argv[]){
    return my_ls(argc, argv);
}
```
