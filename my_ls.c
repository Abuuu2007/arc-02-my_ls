#include <stdio.h>    //c for printf
#include <stdlib.h>   // for malloc и free
#include <unistd.h>   // for write
#include <sys/stat.h> // for stat и lstat
#include <dirent.h>   // for opendir, closedir и readdir
#include <stdbool.h>  // for bool

typedef struct{
    int int_val;
    char *str_arr;
} forSort;

int my_strlen(char* p1){
    int i = 0;
    while(p1[i] != '\0')
        i++;
    return i;
}

int my_strcmp(char* p1, char* p2){
    for(int i = 0; p1[i] != '\0' || p2[i] != '\0'; i++){
        if(p1[i] > p2[i])
            return 1;
        else if(p1[i] < p2[i])
            return -1;
    }
    return 0;
}

char* my_strdup(char* p1){
    int len = my_strlen(p1);
    char* res = malloc((len + 1) * sizeof(char));
    int i;
    for(i = 0; i < len; i++)
        res[i] = p1[i];
    res[i] = '\0';
    return res;
}

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

int main(int argc, char* argv[]){
    return my_ls(argc, argv);
}   