int strcmp(char * s1, char * s2)
{
  while(*s1!=0 && *s2!=0)
  {
    if(*s1!=*s2) { return *s1-*s2; }
    s1++; s2++;
  }
  return *s1-*s2;;
}

int strlen(char * s)
{
  int len = 0;

  while(*s != '\0') {
    len++;
    s++;
  }
  return len;
}
