#include <stdio.h>
#include <stdlib.h> 

#define STRLEN 6 
#define BUFMAX 256

int main(){
  FILE* fp = fopen("course_t_setting.txt", "w"); 
  if(fp == NULL){
    printf("error: cannnot find file\n");
    exit(1);
  }

  const char* str[STRLEN] = { \
     "sec", \
     "straight", \
     "bending sm", \
     "bending wm",  \
     "curve_sm", \
     "curve_wm", \
  };

  char buf[BUFMAX];
  int i = 0;
  for(i = 0; i < STRLEN; i++){
    printf("%s: ", str[i]);
    fgets(buf, BUFMAX, stdin); 

    int j = 0;
    while(buf[j] != '\n') j++;
    buf[j] = '\0';

    fprintf(fp, "%s\n", buf);
  }

  fclose(fp);
}
