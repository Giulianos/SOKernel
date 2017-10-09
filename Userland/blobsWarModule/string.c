#include "./types.h"
#include "include/string.h"

int strlen(const char* str) {
  int length = 0;
  while(*str != '\0') {
    str++;
    length++;
  }
  return length;
}
boolean isspace(char c){
	if(c==' ')
		return true;
	return false;
}
boolean strcmp(char* s1,char* s2){
	int x=0;
	boolean flag=true;
	while(s1[x]!=0 && s2[x]!=0 && flag==true){
		if(s1[x]!=s2[x]){
			flag=false;
		}
		x++;
	}
	if(flag==false)
		return flag;
	if(s1[x]==s2[x]){
		return true;
	}
	return false;
}