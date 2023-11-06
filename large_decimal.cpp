#include <stdio.h>
#include <cstring>
#include <string>
#include "LargeDecimal.h"

#define CALC_BUF_SIZE 128

#if 0 /* Obsolete*/
// Get decimal display string of 128bit integer (divided into 64bit-hi + 64bit-lo parts)
static std::string str128(uint64_t hi, uint64_t lo)
{
    char buf[41];                                                                      // 40 digits
    const char c64[20] = {1, 8, 4, 4, 6, 7, 4, 4, 0, 7, 3, 7, 0, 9, 5, 5, 1, 6, 1, 6}; // 2^64
    int i, j, k;
    char hi_digit;

    std::memset(buf, 0, 41);
    for (i = 0; i < 20; i++)
    {
        buf[39 - i] = lo % 10;
        lo /= 10;
    }
    if (hi)
    {
        for (i = 0; i < 20; i++)
        {
            hi_digit = hi % 10;
            hi /= 10;
            if (hi_digit)
            {
                for (j = 0; j < 20; j++)
                {
                    k = 39 - (i + j);
                    buf[k] += hi_digit * c64[19 - j];
                    while (buf[k] >= 10)
                    {
                        buf[k] -= 10;
                        buf[k - 1]++;
                    }
                }
            }
        }
    }
    for (i = 0; i < 39; i++)
    {
        if (buf[i] == 0)
        {
            buf[i] = ' ';
        }
        else
        {
            break;
        }
    }
    for (j = i; j < 40; j++)
    {
        buf[j] += '0';
    }
    return std::string(buf + i);
}
#endif
// Coparator
int dec_comp(const char *dec1, const char *dec2, int bufsize) {
    int ret = 0;
    for (int i = 0; i < bufsize; i++) {
        if (dec1[i] != dec2[i]) {
            ret = dec1[i] > dec2[i] ? 1: -1;
            break;
        }
    }
    return ret;
}

// Subtractor
int dec_sub(char *dec1, const char *dec2, int bufsize) {
    int borrow = 0;
    if (dec_comp(dec1, dec2, bufsize) < 0) {
        return -1;
    }
    for (int i = bufsize - 1; i >= 0; i--) {
        char d = dec1[i] - dec2[i] - borrow;
        if (d >= 0) {
            dec1[i] = d;
            borrow = 0;
        } else {
            dec1[i] = d + 10;
            borrow = 1;
        }
    }
    return 0;
}

// Multiplier
int dec_mult(char *inout, char n, int bufsize)
{
    int i;
    unsigned char d, roll;

    if (n < 0 || n > 16)
    {
        return -1; // illegal multiplier
    }
    roll = 0;
    for (i = bufsize - 1; i >= 0; i--)
    {
        d = inout[i] * n + roll;
        inout[i] = d % 10;
        roll = d / 10;
    }

    if (roll != 0)
    {
        return -2; // buffer overflow
    }

    return 0;
}

// Multiply-accumlate function
int dec_mac(char *result, const char *input, char n, int bufsize)
{
    int i;
    unsigned char d, roll;

    if (n < 0 || n > 15)
    {
        return -1; // illegal multiplier
    }
    roll = 0;
    for (i = bufsize - 1; i >= 0; i--)
    {
        d = input[i] * n + roll;
        d += result[i];
        result[i] = d % 10;
        roll = d / 10;
    }

    if (roll != 0)
    {
        return -2; // buffer overflow
    }

    return 0;
}

std::string digits2str(const char *digits, int bufsize)
{
    int i, j;
    char *buf = new char[bufsize + 1];
    std::memcpy(buf, digits, bufsize);
    buf[bufsize] = '\0';
    std::string ret;

    for (i = 0; i < bufsize - 1; i++)
    {
        if (digits[i] != 0)
            break;
        buf[i] = ' ';
    }
    for (j = i; j < bufsize; j++)
    {
        buf[j] = digits[j] + '0';
    }
    ret = std::string(buf + i);
    delete[] buf;
    return ret;
}

int dec_expand(const char *str, char *buf, int bufsize)
{
    std::string dec_str(str);
    int strsize = dec_str.size();
    if (strsize > bufsize)
    {
        return -1;
    }
    int index = bufsize - strsize;
    for (char const &c : dec_str)
    {
        if (std::isdigit(c) == 0)
        {
            return -2;
        }
        buf[index++] = c - '0';
    }
    return strsize;
}

int deccmd_add(int argc, char *argv[])
{
    char dec1[CALC_BUF_SIZE] = {0};
    char dec2[CALC_BUF_SIZE] = {0};
    if (argc < 4)
    {
        printf("Usage: %s %s <decimal_exp1> <decimal_exp2>\n", argv[0], argv[1]);
        return -1;
    }
    if (dec_expand(argv[2], dec1, sizeof(dec1)) < 0)
    {
        printf("Error: illegal decimal1 input\n");
        return -1;
    }
    if (dec_expand(argv[3], dec2, sizeof(dec2)) < 0)
    {
        printf("Error: illegal decimal2 input\n");
        return -1;
    }
    if (dec_mac(dec1, dec2, 1, CALC_BUF_SIZE) < 0)
    {
        printf("Error: buffer overflow\n");
        return -1;
    }
    printf("%s\n", digits2str(dec1, CALC_BUF_SIZE).c_str());
    return 0;
}

int deccmd_sub(int argc, char *argv[])
{
    char dec1[CALC_BUF_SIZE] = {0};
    char dec2[CALC_BUF_SIZE] = {0};
    if (argc < 4)
    {
        printf("Usage: %s %s <decimal_exp1> <decimal_exp2>\n", argv[0], argv[1]);
        return -1;
    }
    if (dec_expand(argv[2], dec1, sizeof(dec1)) < 0)
    {
        printf("Error: illegal decimal1 input\n");
        return -1;
    }
    if (dec_expand(argv[3], dec2, sizeof(dec2)) < 0)
    {
        printf("Error: illegal decimal2 input\n");
        return -1;
    }
    if (dec_sub(dec1, dec2, CALC_BUF_SIZE) < 0)
    {
        printf("Error: decimal_exp1 should be greater than or equals to decimal_exp2\n");
        return -1;
    }
    printf("%s\n", digits2str(dec1, CALC_BUF_SIZE).c_str());
    return 0;
}

int deccmd_sub_2(int argc, char *argv[])
{
    LargeDecimal result, dec1, dec2;
    if (argc < 4)
    {
        printf("Usage: %s %s <decimal_exp1> <decimal_exp2>\n", argv[0], argv[1]);
        return -1;
    }

    if (!dec1.SetDec(argv[2]) || !dec2.SetDec(argv[3])) {
        printf("Error: illegal decimal1 input\n");
        return -1;
    }
    result = dec1 - dec2;
    printf("%s\n", result.GetString().c_str());
    return 0;
}

int deccmd_mult(int argc, char *argv[])
{
    char result[CALC_BUF_SIZE] = {0};
    char dec1[CALC_BUF_SIZE] = {0};
    char dec2[CALC_BUF_SIZE] = {0};
    if (argc < 4)
    {
        printf("Usage: %s %s <decimal_exp1> <decimal_exp2>\n", argv[0], argv[1]);
        return -1;
    }
    int dec1_len = dec_expand(argv[2], dec1, sizeof(dec1));
    int dec2_len = dec_expand(argv[3], dec2, sizeof(dec2));
    if (dec1_len <= 0 || dec2_len <= 0)
    {
        printf("Error: illegal decimal input\n");
        return -2;
    }

    for (int i = 0; i < dec2_len; i++)
    {
        dec_mac(result, dec1, dec2[CALC_BUF_SIZE - i - 1], CALC_BUF_SIZE);
        dec_mult(dec1, 10, CALC_BUF_SIZE);
    }
    printf("%s\n", digits2str(result, CALC_BUF_SIZE).c_str());
    return 0;
}

int deccmd_mult_2(int argc, char *argv[])
{
    LargeDecimal result, dec1, dec2;

    if (argc < 4)
    {
        printf("Usage: %s %s <decimal_exp1> <decimal_exp2>\n", argv[0], argv[1]);
        return -1;
    }
    if (!dec1.SetDec(argv[2]) || !dec2.SetDec(argv[3]))
    {
        printf("Error: illegal decimal input\n");
        return -2;
    }

    result = dec1 * dec2;

    printf("%s\n", result.GetString().c_str());
    return 0;
}

int deccmd_h2d(int argc, char *argv[])
{
    char result[CALC_BUF_SIZE] = {0};
    char hex2[CALC_BUF_SIZE] = {0};
    hex2[CALC_BUF_SIZE - 1] = 1;
    if (argc < 3)
    {
        printf("Usage: %s %s <hex_exp1>\n", argv[0], argv[1]);
        return -1;
    }

    std::string tmp(argv[2]);
    std::string hex1_le;
    for (auto const& c : tmp) {
        char h = std::toupper(c);
        if (std::isxdigit(h)) {
            hex1_le = h + hex1_le;
        } else if (c != '_' && c != ',') {
            printf("Error: illegal hexadecimal input\n");
            return -2;
        }
    }
    
    for (auto h : hex1_le) {
        if (std::isdigit(h)) {
            h = h - '0';
        } else {
            h = h - 'A' + 10; 
        }
        dec_mac(result, hex2, h, CALC_BUF_SIZE);
        dec_mult(hex2, 16, CALC_BUF_SIZE);
    }

    printf("%s\n", digits2str(result, CALC_BUF_SIZE).c_str());
    return 0;
}

int deccmd_h2d_2(int argc, char *argv[])
{
    LargeDecimal dec;

    if (argc < 3)
    {
        printf("Usage: %s %s <hex_exp1>\n", argv[0], argv[1]);
        return -1;
    }

    if (!dec.SetHex(argv[2])) {
        printf("Error: illegal hexadecimal input\n");
        return -2;
    };
    
    printf("%s\n", dec.GetString().c_str());
    return 0;
}

int deccmd_d2h(int argc, char *argv[])
{
    LargeDecimal dec;

    if (argc < 3)
    {
        printf("Usage: %s %s <dec_exp1>\n", argv[0], argv[1]);
        return -1;
    }

    if (!dec.SetDec(argv[2])) {
        printf("Error: illegal decimal input\n");
        return -2;
    };
    
    printf("%s\n", dec.GetString().c_str());
    return 0;
}

void usage(const char *app)
{
    printf("USAGE: %s <command> [param] ...\n", app);
    printf("Available commands\n");
    printf(" help add sub mult h2d\n");
}

int main(int argc, char *argv[])
{
#if 1
    int ret = 0;
    std::string cmd;
    if (argc < 2)
    {
        usage(argv[0]);
    }
    else
    {
        cmd = argv[1];
        if (cmd == "help")
        {
            usage(argv[0]);
        }
        else if (cmd == "add")
        {
            ret = deccmd_add(argc, argv);
        }
        else if (cmd == "sub")
        {
//            ret = deccmd_sub(argc, argv);
            ret = deccmd_sub_2(argc, argv);
        }
        else if (cmd == "mult")
        {
            ret = deccmd_mult_2(argc, argv);
        }
        else if (cmd == "h2d")
        {
//            ret = deccmd_h2d(argc, argv);
            ret = deccmd_h2d_2(argc, argv);
        }
        else if (cmd == "d2h")
        {
            ret = deccmd_d2h(argc, argv);
        }
        else
        {
            printf("Unknown command \"%s\"\n", cmd.c_str());
        }
    }
    if (ret == 0)
    {
        exit(EXIT_SUCCESS);
    }
    else
    {
        exit(EXIT_FAILURE);
    }
#else
    std::string result;
    uint64_t hi, lo;
    uint64_t data[4] = {0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff};

    hi = 0xffffffffffffffff;
    lo = 0xffffffffffffffff;
    result = decstr256(data);
    printf("0x%016llx_%016llx\n", hi, lo);
    printf(" = %s\n", result.c_str());
    result = str128(hi, lo);
    printf("0x%016llx_%016llx\n", hi, lo);
    printf(" = %s\n", result.c_str());
    return 0;
#endif
}
