#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    /*
    -> check argv[2] have ".bmp" as last 4 char
       * If not,print the proper error msg, return e_failure
    encInfo -> *src_image_fname = argv[2]

    encoInfo -> *secret_fname =argv[3]

    -> check argv[4] == NULL
         encoInfo -> *stego_image_fname = "Output.bmp"
    ->else
        * validate argv[4] is ".bmp"
             => If not,print the proper error msg, return e_failure
        * encoInfo -> *stego_image_fname = argv[4]

    -> call open_files(encoInfo) == e_failure
       return e_failure

    return e_success
    */
    if(argv[2] == NULL || strlen(argv[2]) < 4 || strcmp(argv[2] + strlen(argv[2]) - 4, ".bmp") != 0)
    {
        printf("ERROR: Source image should be  a .bmp file\n");
        return e_failure;
    }
    encInfo->src_image_fname = argv[2];

    encInfo->secret_fname = argv[3];

    if(argv[4] == NULL)
    {
        encInfo->stego_image_fname = "Output.bmp";
    }
    else
    {
        if(strlen(argv[4] < 4 || strcmp(argv[4])+ strlen(argv[4]) - 4, ".bmp") != 0)
        {
            printf("ERROR: Output image should be a .bmp file\n");
            return e_failure;
        }
        encInfo->stego_image_fname = argv[4];
    }
    if(open_files(encInfo) == e_failure)
    {
        return e_failure;
    }
    return e_success;
}

Status open_files(EncodeInfo *encInfo)
{
    /*
        -> open 'encoInfo -> src_image_fname' file in read mode
           * If ret value is NULL, print error, return e_failure
           fptr_src_image = fopen()


        -> open 'encoInfo -> secret_file' file in read mode
           * If ret value is NULL, print error, return e_failure
           fptr_secret = fopen()
        
        -> open 'encoInfo -> stego_image_fname' file in write mode
           * If ret value is NULL, print error, return e_failure
           fptr_stego_image = fopen()

           return e_success
    */
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname,"r");

    if(encInfo->fptr_src_image == NULL)
    {
        perror("fopen")
        fprintf(stderr, "ERROR: Unable to open file %s\n",encInfo->src_image_fname);

        return e_failure;
    }

    encInfo->fptr_secret = fopen(encInfo->secret_fname,"r");

    if(encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",encInfo->secret_fname);

        return e_failure;

    }
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname,"w");

    if(encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",encInfo->stego_image_fname);

        return e_failure;
    }
    return e_success; 

}

Status do_encoding(EncodeInfo *encInfo)
{
    /*
         // Call check_capacity(encInfo) == e_failure
            print error msg, return e_failure

        // Call copy_bmp_header(fptr_src_file,fptr_dest_file) == e_failure
            print error msg, return e_failure

        // Call encode_magic_string(Magic_STRING, encInfo) == e_failure
            print error msg, return e_failure

        // Call encode_secret_file_extn_size(encInfo) == e_failure
            print error msg, return e_failure

        // Call encode_secret_file_extn(extn_secret_file,encInfo) == e_failure
            print error msg, return e_failure

        // Call encode_secret_file_size(size_secret_file,encInfo) == e_failure
            print error msg, return e_failure

        // Call encode_secret_file_data(encInfo) == e_failure
            print error msg, return e_failure

        // Call copy_remaining_img_data(fptr_src_file,fptr_dest_file) == e_failure
            print error msg, return e_failure

        return e_success
    */

    if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo) == e_failure)
    {
        printf("ERROR: Failed to encode secret file extension\n");
        return e_failure;
    }
    if(encode_secret_file_size(encInfo->size_secret_file,encInfo)== e_failure)
    {
        printf("ERROR: Failed to encode secret file size\n");
        return e_failure;
    }
    if(encode_secret_file_data(encInfo) == e_failure)
    {
        printf("ERROR: Failed to encode secret file data\n");
        return e_failure;
    }
    if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("ERROR: Failed to copy remaining image data\n");
        return e_failure;
    }
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    /*
         -> call get_image_size_for_bmp(encode -> fptr_src_image)
             image_capacity = get_image_size()
         -> call get_file_size(encode -> fptr_secret)
            size_secret_file = get_file_size()

        -> check ((14 + size_secret_file) * 8) > image_capacity
            return r_failure

        -> return e_success
    */
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->src_image);

    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

    if(((14 + encInfo->size_secret_file) * 8) > encInfo->image_capacity)
    {
        return e_failure;
    }
    return e_success;
}

uint get_file_size(FILE *fptr)
{
    /*
    -> move the offset to last pos
    -> return ftell()

    */
    fseek(fptr, 0, SEEK_END);

    return ftell(fptr);
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    /*
       -> move the file pointers to the SEEK_SET
       -> declare the buff[54]
       -> Read 54 bytes from src file
       -> write 54 bytes to dest file

       -> return e_success
    */
    char buff[54];

    fseek(fptr_src_image, 0, SEEK_SET);
    fseek(fptr_dest_image, 0, SEEK_SET);

    fread(buff, 54, 1, fptr_src_image);
    fwrite(buff, 54, 1, fptr_dest_image);

    return e_success;
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    /*

      declare a buff of 8 bytes
      -> Loop for (length of magic_string )2times
      read 8 bytes from the src_file into buff
      encode_byte_to_lsb(magic_string[1],buff)
      write the encoded buff to output_file

      ->return e_success

    */
    char buff[8];
    for(int i=0;magic_string[i] != '\0';i++)
    {
        fread(buff, 8, 1, encInfo->fptr_src_image);

        encode_byte_to_lsb(magic_string[i], buff);

        fwrite(buff, 8, 1, encInfo->fptr_stego_image);
    }
    return e_success;

}
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    /*
        for(int i = 7;i >= 0;i--)
        {
            ->get the ith bit is or not
               => if set, set the LSB of image_buffer[]
               => if clear, clear the LSB of image_buffer[]

        }
    */
    for(int i=7;i >= 0;i--)
    {
        if(data &(1 << i))
        {
            image_buffer[7 - i] = image_buffer[7 - i] | 1;
        }
        else
        {
            image_buffer[7 - i] = image_buffer[7 - i] & ~1;
        }
    }
    return e_success;

}
Status encode_secret_file_extn_size(EncodeInfo *encoInfo)
{
    /*
        -> char *dot = strchr(secret_file_name,'.')
        -> strcpy(extn_secret_file,dot);
        -> Declare a buff[32]

        -> Read 32 bytes from src_file into buff
        -> Call  encode_size_to_lsb(strlen(extn_secret_file),buff)
        -> Write 32 bytes of buff to output_file

        return e_success;
    */
    char *dot = strchr(encInfo->secret_fname,'.');

    if(dot == NULL)
    {
        return e_failure;
    }
    strcpy(encInfo->extn_secret_file,dot);

    char buff[32];

    fread(buff, 32, 1, encInfo->fptr_src_image);

    encode_size_to_lsb(strlen(encInfo->extn_secret_file),buff);

    fwrite(buff, 32, 1, encInfo->fptr_stego_image);

    return e_success;

}
Status encode_size_to_lsb(int size, char *Image_buff)
{
    /*
       for(int i = 31;i >= 0;i--)
        {
            ->get the ith bit is or not
               => if set, set the LSB of image_buffer[]
               => if clear, clear the LSB of image_buffer[]

        } 
        
        return e_success;

    */
    for(int i = 31;i >=0;i--)
    {
        if((unsigned int)size &(1U <<i))
        {
            Image_buff[31 - i] = Image_buff[31 - i] | 1;
        }
        else
        {
            Image_buff[31 - i] = Image_buff[31 - i] & ~1;
        }
    }
    return e_success;

}
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    /*
        Declare buff[8]
    => Loop for extention length
        -> read 8 bytes from src_image
        -> encode_byte_to_lsb(file_extn[],buff)
        -> Write the 8 bytes of buff to output_file

        return e_success

    */
    for(int i = 0; file_extn[i]!= '\0';i++)
    {
        fread(buff, 8, 1, encInfo->fptr_src_image);

        encode_byte_to_lsb(file_extn[i], buff);

        fwrite(buff, 8, 1, encInfo->fptr_stego_image);
    }
    return e_success;

}
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);
{
    /*
        -> Declare the buff[32] 

        -> Read the 32 byte from src_image
        -> Call encode_size_to_lsb(file_size,buff)
        -> Write the 32 bytes of buff to output_file

        return e_success
    */
    char buff[32];

    fread(buff, 32, 1, encInfo->fptr_src_image);

    encode_size_to_lsb(file_size, buff);

    fwrite(buff, 32, 1, encInfo->fptr_stego_image);

    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    /*
        ->Declare buff[8]
    => Loop till EOF of secret_file
        -> Read 8 bytes from src_file
        -> Read 1 byte from secret_file
        -> encode_byte_to_lsb(data,buff)

        return e_success
    */
    char buff[8];
    char data;

    while(fread(&data, 1, 1, encInfo->fptr_secret) == 1)
    {
        fread(buff, 8, 1, encInfo->fptr_src_image);

        encode_byte_to_lsb(data, buff);

        fwrite(buff, 8, 1, encInfo->fptr_stego_image);
    }
    return e_success;
}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    /*
        -> Declare a char as data
    => Loop till EOF of src_file
        ->Read a char from src_file
        -> Write the data o dest_file

        return e_success
    */
    char data;

    while(fread(&data, 1, 1, fptr_src) == 1)
    {
        fwrite(&data, 1, 1, fptr_dest);
    }
    return e_success;s
}
