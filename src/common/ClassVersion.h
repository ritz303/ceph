#ifndef __CLASSVERSION_H
#define __CLASSVERSION_H

#include "include/types.h"


class ClassVersion
{
protected:
  std::string ver;

public:
  ClassVersion(string& v) : ver(v) {}
  ClassVersion(const char *s) : ver(s) {}
  ClassVersion() {}

  void operator=(const char *s) { ver = s; }
  void operator=(string& v) { ver = v; }

  friend bool operator==(ClassVersion& v1, ClassVersion& v2);
  friend bool operator<(ClassVersion& v1, ClassVersion& v2);
  friend std::ostream& operator<<(std::ostream& out, const ClassVersion& v);
  void encode(bufferlist& bl) const {
    ::encode(ver, bl);
  }
  void decode(bufferlist::iterator& bl) {
    ::decode(ver, bl);
  }

  const char *str() { return ver.c_str(); };
  bool is_default() { return (ver.length() == 0); }
};
WRITE_CLASS_ENCODER(ClassVersion)

static int compare_single(const char *v1, const char *v2)
{
  int i1 = atoi(v1);
  int i2 = atoi(v2);

  if (i1 != i2)
    return (i1-i2);

  const char *p1 = v1;
  const char *p2 = v2;

  while (isdigit(*p1))
    p1++;
  while (isdigit(*p2))
    p2++;

  return strcmp(p1, p2);
}

inline std::ostream& operator<<(std::ostream& out, const ClassVersion& v)
{
  out << v.ver;

  return out;
}
inline bool operator==(ClassVersion& v1, ClassVersion& v2)
{
  return (v1.ver == v2.ver);
}

inline bool operator<(ClassVersion& v1, ClassVersion& v2)
{
  const char *_s1 = v1.ver.c_str();
  const char *_s2 = v2.ver.c_str();
  int l1 = strlen(_s1);
  int l2 = strlen(_s2);
  char s1[l1 + 1];
  char s2[l2 + 1];
  char *p1 = s1;
  char *p2 = s2;

  const char *tok1, *tok2;

  memcpy(s1, _s1, l1 + 1);
  memcpy(s2, _s2, l2 + 1);


  while (true) {
    tok1 = strsep(&p1, ".");
    tok2 = strsep(&p2, ".");
    if (!tok1 || !tok2) {
      if (!tok1 && !tok2)
        return false;
      if (!tok1)
        return true;
      return false;
    }
    int r = compare_single(tok1, tok2);
    if (r < 0)
      return true;
    if (r > 0)
      return false;
  }
}


#endif
