#include <stdio.h>
#include <cstring>
#include <string>
#include "LargeDecimal.h"

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

int deccmd_add(int argc, char *argv[])
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
    result = dec1 + dec2;
    printf("%s\n", result.GetString().c_str());
    return 0;
}

int deccmd_sub(int argc, char *argv[])
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
            ret = deccmd_sub(argc, argv);
        }
        else if (cmd == "mult")
        {
            ret = deccmd_mult(argc, argv);
        }
        else if (cmd == "h2d")
        {
            ret = deccmd_h2d(argc, argv);
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
}
