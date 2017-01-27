void test(int x) {
  bigint n = x;
  cout << n << "\n";
}

void test(char* s) {
  bigint n = s;
  cout << n << "\n";
}

int main() {
  test(0);
  test(123123);
  test(-123123);
  test(1);
  test(10);
  test(100);
  test(1000);
  test(10000);
  test(11);
  test(101);
  test(1001);
  test(10001);
  test(100001);
  test(-1);
  test(-10);
  test(-100);
  test(-1000);
  test(-10000);
  test(-11);
  test(-101);
  test(-1001);
  test(-10001);
  test(-100001);

  test("0");
  test("123123");
  test("-123123");
  test("1");
  test("10");
  test("100");
  test("1000");
  test("10000");
  test("11");
  test("101");
  test("1001");
  test("10001");
  test("100001");
  test("-1");
  test("-10");
  test("-100");
  test("-1000");
  test("-10000");
  test("-11");
  test("-101");
  test("-1001");
  test("-10001");
  test("-100001");
  test("+000000000123456789123456789");
  test("-000000000123456789123456789");
  test(" - +  -   +  -    000000   0 0 0  123456789123456789");

  return 0;
}
