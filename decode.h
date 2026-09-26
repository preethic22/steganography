#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "types.h"

typedef struct _DecodeInfo
{
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Output File Info */
    char *output_fname;
    FILE *fptr_output;

    /* Secret File Info */
    char extn_secret_file[5];
    int extn_size;
    int size_secret_file;

} DecodeInfo;


/* Check operation type */
OperationType check_operation_type(char opt);

/* Decode functions */
Status decode_byte_from_lsb(char *image_buffer, char *data);

Status decode_size_from_lsb(char *image_buffer, int *size);

Status decode_magic_string(const char *magic_string,
                           DecodeInfo *decInfo);

Status decode_secret_file_extn_size(DecodeInfo *decInfo);

Status decode_secret_file_extn(DecodeInfo *decInfo);

Status decode_secret_file_size(DecodeInfo *decInfo);

Status decode_secret_file_data(DecodeInfo *decInfo);

/* File functions */
Status open_decode_files(DecodeInfo *decInfo);

Status read_and_validate_decode_args(char *argv[],
                                     DecodeInfo *decInfo);

Status do_decoding(DecodeInfo *decInfo);

#endif