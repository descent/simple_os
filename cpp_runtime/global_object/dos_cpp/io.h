__asm__(".code16gcc\n");
#ifndef IO_H
#define IO_H

class Io
{
  public:
    Io();
    ~Io();
    void print(const char   *s);
    virtual void fun1()
    {
      print("Io::func1\r\n");
    }
  private:
    const char *str_;
};

class Ab
{
  public:
    Ab(int i);
    ~Ab();
    void print(const char   *s);
  private:
    const char *str_;
    int i_;
};

class DeriveIo : public Io
{
  public:
    virtual void fun1()
    {
      print("DeriveIo::func1\r\n");
    }
  private:
};

#endif
