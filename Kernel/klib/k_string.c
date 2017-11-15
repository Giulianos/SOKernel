#include <lib.h>

int k_strcmp(const char * s1, const char * s2)
{
  while(*s1!=0 && *s2!=0)
  {
    if(*s1!=*s2) { return *s1-*s2; }
    s1++; s2++;
  }
  return *s1-*s2;;
}

int k_strncpy(char * destination, const char * source, size_t n)
{
  int i = 0;

  while(*source != '\0' && n > 0) {
    i++;
    n--;
    *(destination++) = *(source++);
  }
  return i;
}
