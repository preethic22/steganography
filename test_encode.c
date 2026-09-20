#include <stdio.h>
#include "encode.h"
#include "types.h"

int main(int argc, )
{
    EncodeInfo encInfo;
    // -> call check_operation_type(argv[1][1]) == e_encode
    /*
         -> Call read_and_validate_encode_args(argv,&encInfo) == e_success
             => Call do_encoding(&encoInfo) == e_success
                print "Encoding is success"
    */

    
    return 0;
}
OperationType check_operation_type(char opt)
{
    /*
        *check opt is 'e'
             return e_encode;
        *check opt is 'd'
             return e_decode;
        *else
             return e_unsupported;
        
     */

}