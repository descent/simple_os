#ifndef IO_H
#define IO_H

class Io
{
  public:
    Io();
    ~Io();
    void print(const char   *s);
  private:
    const char *str_;
};


#endif
