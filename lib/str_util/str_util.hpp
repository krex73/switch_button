#ifndef _str_util_h
#define _str_util_h


// void del_char(const char * src, char c)
// {
// char *tmp = src;
// do
// if (*src != c)
// *res++ = *src;
// while (*src++);
// return tmp;
// }


bool split2(const String &s, String &name, String &val)
{
  int i;
  for (i = 0; i < s.length(); ++i)
    if (s[i] == '=')
      break;
  if (i == s.length())
    return false;
  name = s.substring(0, i - 1);
  name.trim();
  val = s.substring(i + 1, s.length());
  val.trim();
  return true;
}

String splitFirst(const String &s, String &ost)
{
  int i;
  for (i = 0; i < s.length(); ++i)
    if ((s[i] == '\n') || (s[i] == '\r'))
      break;

  if (i == s.length())
  {
    ost = s.substring(i + 1, s.length());
    return "";
  }

  i++;
  if (((s[i] == '\n') || (s[i] == '\r')))
  {
    ost = s.substring(i + 2, s.length());
  }
  else
  {
    ost = s.substring(i + 1, s.length());
  }

  return s.substring(0, i - 1);
}

#endif