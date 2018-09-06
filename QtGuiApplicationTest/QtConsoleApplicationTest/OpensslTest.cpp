#include "OpensslTest.h"

#include <openssl/bn.h>
#include <openssl/bio.h>

#include <LogUtil.h>

typedef struct tagX
{
    STACK_OF(X509_EXTENSION) *ext;
} X;

OpensslTest::OpensslTest()
{
}

OpensslTest::~OpensslTest()
{
}

void OpensslTest::TestEnter()
{
    int res = this->TestASN1();
    LogUtil::Debug(CODE_LOCATION, "res = %d", res);
}

void OpensslTest::TestExit()
{
    ;
}

int OpensslTest::TestASN1()
{
    BIGNUM *a, *b, *add;
    BIO *out;
    char c[20], d[20];
    int ret;

    a = BN_new();
    strcpy(c, "32");
    ret = BN_hex2bn(&a, c);
    b = BN_new();
    strcpy(d, "100");
    ret = BN_hex2bn(&b, d);
    out = BIO_new(BIO_s_file());
    ret = BIO_set_fp(out, stdout, BIO_NOCLOSE);
    add = BN_new();
    ret = BN_add(add, a, b);
    if (ret != 1)
    {
        printf("err.\n");
        return -1;
    }

    BIO_puts(out, "bn 0x32 + 0x100 = 0x");
    BN_print(out, add);
    BIO_puts(out, "\n");
    BN_free(a);
    BN_free(b);
    BN_free(add);
    BIO_free(out);

    return 0;
}
