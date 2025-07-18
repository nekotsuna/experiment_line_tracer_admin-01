#include <stdio.h>
#include <stdlib.h> 

#define STRLEN 14
#define BUFMAX 256
#define SECDIV 1000

typedef enum setting_index{
  SEC,
  STRAIGHT,
  BENDING_SM,
  BENDING_WM,
  BENDING_STSM,
  BENDING_STWM,
  CURVE_SM,
  CURVE_WM,
  CURVE_STRM,
  CURVE_STR_SEC,
  CURVE_INIT_SEC,
  UTURN_SM,
  UTURN_WM,
  UTURN_SEC,
} SettingIndex;

const char* str[STRLEN] = { \
     "sec", \
     "straight", \
     "bending sm", \
     "bending wm",  \
     "bending ssm", \
     "bending swm", \
     "curve_sm", \
     "curve_wm", \
     "curve_strm", \
     "curve_str_sec", \
     "curve_init_sec", \
     "uturn_sm", \
     "uturn_wm", \
     "uturn_sec", \
};

void setting(int settings[]);
void set();

void setting(int settings[]){

  FILE* fp = fopen("course_t_setting.txt", "r");
  if(fp == NULL){
    printf("error: cannnot find file\n");
    exit(1);
  }

  char c;
  int i = 0;
  int line = 0;
  char buf[BUFMAX];
  while((c = fgetc(fp)) != EOF){
    if(line >= STRLEN) break; 
    
    if(c == '\n'){
      buf[i] = '\0';
      settings[line] = atoi(buf);
      i = 0;  
      line++;
    }
    else{
      buf[i] = c; 
      i++;
    }
  }

  fclose(fp);
}

void set(){
  FILE* fp;
  int i;
  char buf[BUFMAX];

  int old_settings[STRLEN];

  setting(old_settings);

  fp = fopen("course_t_setting.txt", "w"); 
  i = 0;
  if(fp == NULL){
    printf("error: cannnot find file\n");
    exit(1);
  }

  for(i = 0; i < STRLEN; i++){
    printf("%s, %d : ", str[i], old_settings[i]);
    fgets(buf, BUFMAX, stdin); 

    int j = 0;
    while(buf[j] != '\n') j++;

    if(j == 0){
      fprintf(fp, "%d\n", old_settings[i]);
    }
    else{
      buf[j] = '\0';
      fprintf(fp, "%s\n", buf);
    }
  }

  fclose(fp);
}
