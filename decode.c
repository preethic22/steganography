#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"


/* Decode one byte from 8 image bytes */
/*decode*/
Status decode_byte_from_lsb(char *image_buffer, char *data)
{
    int i;

    *data = 0;

    for(i = 0; i < 8; i++)
    {
        if(image_buffer[i] & 1)
        {
            *data = *data | (1 << (7 - i));
        }
    }

    return e_success;
}


/* Decode size from 32 image bytes */
Status decode_size_from_lsb(char *image_buffer, int *size)
{
    int i;

    *size = 0;

    for(i = 0; i < 32; i++)
    {
        if(image_buffer[i] & 1)
        {
            *size = *size | (1U << (31 - i));
        }
    }

    return e_success;
}


/* Decode magic string */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
    char buffer[8];
    char data;
    int i;

    for(i = 0; magic_string[i] != '\0'; i++)
    {
        fread(buffer, 8, 1, decInfo->fptr_stego_image);

        decode_byte_from_lsb(buffer, &data);

        if(data != magic_string[i])
        {
            return e_failure;
        }
    }

    return e_success;
}


/* Decode secret file extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char buffer[32];

    fread(buffer, 32, 1, decInfo->fptr_stego_image);

    decode_size_from_lsb(buffer, &decInfo->extn_size);

    return e_success;
}


/* Decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char buffer[8];
    char data;
    int i;

    for(i = 0; i < decInfo->extn_size; i++)
    {
        fread(buffer, 8, 1, decInfo->fptr_stego_image);

        decode_byte_from_lsb(buffer, &data);

        decInfo->extn_secret_file[i] = data;
    }

    decInfo->extn_secret_file[i] = '\0';

    return e_success;
}


/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char buffer[32];

    fread(buffer, 32, 1, decInfo->fptr_stego_image);

    decode_size_from_lsb(buffer, &decInfo->size_secret_file);

    return e_success;
}


/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char buffer[8];
    char data;
    int i;

    for(i = 0; i < decInfo->size_secret_file; i++)
    {
        fread(buffer, 8, 1, decInfo->fptr_stego_image);

        decode_byte_from_lsb(buffer, &data);

        fwrite(&data, 1, 1, decInfo->fptr_output);
    }

    return e_success;
}


/* Open decode files */
Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image =
        fopen(decInfo->stego_image_fname, "r");

    if(decInfo->fptr_stego_image == NULL)
    {
        printf("ERROR: Unable to open stego image\n");
        return e_failure;
    }

    decInfo->fptr_output =
        fopen(decInfo->output_fname, "w");

    if(decInfo->fptr_output == NULL)
    {
        printf("ERROR: Unable to open output file\n");
        return e_failure;
    }

    return e_success;
}


/* Read and validate decode arguments */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if(argv[2] == NULL ||
       strlen(argv[2]) < 4 ||
       strcmp(argv[2] + strlen(argv[2]) - 4, ".bmp") != 0)
    {
        printf("ERROR: Stego image should be a .bmp file\n");
        return e_failure;
    }

    if(argv[3] == NULL)
    {
        printf("ERROR: Output file is missing\n");
        return e_failure;
    }

    decInfo->stego_image_fname = argv[2];

    decInfo->output_fname = argv[3];

    if(open_decode_files(decInfo) == e_failure)
    {
        return e_failure;
    }

    return e_success;
}


/* Perform decoding */
Status do_decoding(DecodeInfo *decInfo)
{
    /*
     * Skip 54-byte BMP header
     */
    if(fseek(decInfo->fptr_stego_image, 54, SEEK_SET) != 0)
    {
        return e_failure;
    }

    /*
     * Decode magic string
     */
    if(decode_magic_string(MAGIC_STRING, decInfo) == e_failure)
    {
        printf("ERROR: Magic string decoding failed\n");
        return e_failure;
    }

    /*
     * Decode secret file extension size
     */
    if(decode_secret_file_extn_size(decInfo) == e_failure)
    {
        printf("ERROR: Failed to decode extension size\n");
        return e_failure;
    }

    /*
     * Decode secret file extension
     */
    if(decode_secret_file_extn(decInfo) == e_failure)
    {
        printf("ERROR: Failed to decode extension\n");
        return e_failure;
    }

    /*
     * Decode secret file size
     */
    if(decode_secret_file_size(decInfo) == e_failure)
    {
        printf("ERROR: Failed to decode secret file size\n");
        return e_failure;
    }

    /*
     * Decode secret file data
     */
    if(decode_secret_file_data(decInfo) == e_failure)
    {
        printf("ERROR: Failed to decode secret file data\n");
        return e_failure;
    }

    return e_success;
}