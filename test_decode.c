#include <stdio.h>
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    DecodeInfo decInfo;

    if (argc < 3)
    {
        printf("Usage: ./decode -d output.bmp output.txt\n");
        return 0;
    }

    if (check_operation_type(argv[1][1]) == e_decode)
    {
        if (read_and_validate_decode_args(argv, &decInfo) == e_success)
        {
            if (do_decoding(&decInfo) == e_success)
            {
                printf("Decoding completed successfully\n");
            }
            else
            {
                printf("Decoding failed\n");
            }
        }
        else
        {
            printf("Invalid decoding arguments\n");
        }
    }
    else
    {
        printf("Unsupported operation\n");
    }

    return 0;
}


OperationType check_operation_type(char opt)
{
    if (opt == 'e')
    {
        return e_encode;
    }
    else if (opt == 'd')
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}