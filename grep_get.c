#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <malloc.h>
#include <fcntl.h>

//нет getline

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
    
	if (fp == NULL) {
		perror(file);
		return;
	}
	
	int MAXSIZE = 100;
	char* buf = malloc(MAXSIZE);
	if (buf == NULL) {
		perror(buf);
		fclose(fp);
		return;
	}
	
	int i = 0;
	while ( !feof(fp) ) {
		
		char c;
		c = fgetc(fp);
		if (c != '\n') {
			buf[i++] = c;
			if (i == MAXSIZE - 1) {
				buf = realloc(buf, 2*MAXSIZE);
				MAXSIZE = MAXSIZE*2;
			}
		} 
		else {
			buf[i] = '\0';
			
			if ( seek_substring_KMP(buf, substr) >= 0) 
				printf("%s\n", buf);
			
			i = 0;
		}
	}

    free(buf);
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
	//проверить аргументы
	if (argc < 3) {
		printf("Wrong number of arguments");
		exit(0);
	}
	
	
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
