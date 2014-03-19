#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

int seek_substring_KMP (char s[], char p[])
{ 
	int i, j, N, M; 
	N = strlen(s); 
	M = strlen(p); 
	int *d =(int*)malloc(M*sizeof(int)); /* динамический массив длины М*/ 
	/* Вычисление префикс-функции */ 
	d[0]=0;
	for(i=1,j=0;i<M;i++)
	{
		while(j>0 && p[j]!=p[i]) 
			j = d[j-1];
		if(p[j]==p[i])
			j++;
		d[i]=j;
	}
	/* поиск */
	for(i=0,j=0;i<N; i++)
	{
		while(j>0 && p[j]!=s[i])
			j=d[j-1];
		if(p[j]==s[i]) 
                        j++;
		if (j==M)
                {
		        free (d); /* освобождение памяти массива d */ 
                        return i-j+1;
                }
	}
    free (d); /* освобождение памяти массива d */ 
	return -1;
}

void grep(char *substr, char* file) {
	FILE *fp = fopen(file, "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    printf("sdf");
	if (fp == NULL) {
		perror(file);
		return;
	}
	
	while ((read = getline(&line, &len, fp)) != -1) {
		if (seek_substring_KMP(line, substr) >= 0)
			printf("%s \n \n", line);
	}

    free(line);
	fclose(fp);
}

void grep_r(char* substr, char* file) {
	DIR *dir = opendir(file);
	if (dir == NULL) {
		perror(file);
	}
	struct dirent* dir_cur;
	while ((dir_cur = readdir(dir)) != NULL) {
		if ( dir_cur->d_name[0] == '.' )
			continue;
		size_t path_len = strlen(file);
		size_t name_len = strlen(dir_cur->d_name);
		char *new_path = malloc(path_len + 1 + name_len + 1);
		strcpy(new_path, file);
		strcat(new_path, "/");
		strcat(new_path, dir_cur->d_name);
		if (dir_cur->d_type & DT_DIR)
			(substr, new_path);
		if (dir_cur->d_type & DT_REG)
			grep(substr, new_path);
		free(new_path);
	}
	closedir(dir);
}


int main(int argc, char **argv) {
	int i = 0;
	
	if ( strcmp(argv[1], "-R") == 0 ) {
		for (i = 3; i <= argc - 1; ++i) {
			grep_r(argv[2], argv[i]);
		}			
	}
	
	else {
		for (i = 2; i <= argc - 1; ++i) {
			grep(argv[1], argv[i]);
		}
		
	}
	
	return 0;
}
