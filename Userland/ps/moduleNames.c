char * names[] = {"idle", "init", "sh", "help", "ps"};

char * getModuleName(int i)
{
  if(i<=4)
    return names[i];
  return "unknown";
}
