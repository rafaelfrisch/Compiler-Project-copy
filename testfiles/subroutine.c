
void f(int z, int a)
{
  a = z;
}

int testFunction(int z) {
  return z;
}

void main(void)
{
  int a;
  int x;
  int z;

  z = 2;
  a = 3;

  f(z, a);
  a = testFunction(z);
  return x;
}