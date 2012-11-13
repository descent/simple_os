
int main(int argc, const char *argv[])
{
  *((unsigned char *)(0xb8000+160*22+0)) = 'e';
  *((unsigned char *)(0xb8000+160*22+2)) = 'c';
  *((unsigned char *)(0xb8000+160*22+4)) = 'h';
  *((unsigned char *)(0xb8000+160*22+6)) = 'o';
  while(1);
  return 0;
}
