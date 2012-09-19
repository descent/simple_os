#ifndef IO_H
#define IO_H

class Io
{
  public:
    Io();
    ~Io();
  private:
    const char *str_;
    int num_;
    static int count_;
};

class Ab
{
  public:
    Ab(int i);
    ~Ab();
  private:
    const char *str_;
    int i_;
};

#endif
