#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void handle_lenght_specified_case(const char *filePath, unsigned int lineLenght);
void handle_default_case(const char *filepath);
void print_line(char *begin, unsigned int size, unsigned int startingByteNumber);
char *read_entire_file(const char *filepath, int *size);
int is_char_ok(unsigned char c);

#define DEFAULT_SIZE 500

void handle_lenght_specified_case(const char *filePath, unsigned int lineLenght)
{
    int Size = 0;
    char *FileContent = read_entire_file(filePath, &Size);

    int GeneralByteCount = 0;

    while (GeneralByteCount < Size)
    {
        unsigned int BytesPerLine = Size - GeneralByteCount;
        if (BytesPerLine > lineLenght)
            BytesPerLine = lineLenght;

        print_line(FileContent, BytesPerLine, GeneralByteCount);
        FileContent += BytesPerLine;
        GeneralByteCount += BytesPerLine;
    }

    exit(EXIT_SUCCESS);
}

void handle_default_case(const char *filePath)
{
    int Size = 0;
    char *FileContent = read_entire_file(filePath, &Size);

    int GeneralByteCount = 0;

    while (GeneralByteCount < Size)
    {
        int BytesPerLine = Size - GeneralByteCount;
        if (BytesPerLine > 16)
            BytesPerLine = 16;

        print_line(FileContent, BytesPerLine, GeneralByteCount);
        FileContent += BytesPerLine;
        GeneralByteCount += BytesPerLine;
    }

    exit(EXIT_SUCCESS);
}

void handle_stdin_case(char *FileContent, size_t size)
{
    unsigned int GeneralByteCount = 0;

    while (GeneralByteCount < size)
    {
        int BytesPerLine = size - GeneralByteCount;
        if (BytesPerLine > 16)
            BytesPerLine = 16;

        print_line(FileContent, BytesPerLine, GeneralByteCount);
        FileContent += BytesPerLine;
        GeneralByteCount += BytesPerLine;
    }

    exit(EXIT_SUCCESS);
}

void print_line(char *begin, unsigned int size, unsigned int startingByteNumber)
{
    printf("%05x ", startingByteNumber);
    for (unsigned int i = 0; i < size; ++i)
    {
        char CharToPrint = begin[i];
        if (CharToPrint < 0)
            printf(" %02X ", (unsigned char)CharToPrint);
        else
            printf(" %02X ", CharToPrint);
    }

    printf("|");
    for (unsigned int i = 0; i < size; ++i)
    {
        char CharToPrint = begin[i];
        if (is_char_ok((unsigned char)CharToPrint))
            printf("%c", CharToPrint);
        else
            printf(".");
    }
    printf("|");
    printf("\n");
}

char *read_entire_file(const char *filePath, int *size)
{
    char *Buffer;
    int Size = 0;
    FILE *File = fopen(filePath, "rb");

    if (File)
    {
        fseek(File, 0, SEEK_END);
        Size = ftell(File);
        rewind(File);
        Buffer = (char *)malloc(sizeof(char) * Size);
        fread(Buffer, sizeof(char), Size, File);
    }
    else
    {
        printf("File is not found: %s", filePath);
        exit(1);
    }
    *size = Size;
    return Buffer;
}

int is_char_ok(unsigned char c)
{
    if (c == 0x7F)
        return 0;

    if (c >= 0x21)
        return 1;
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc == 3)
    {
        if (isdigit(*argv[1]))
        {
            int IntFirst = *argv[1] - '0';
            int IntSecond = *(argv[1] + 1) - '0';

            unsigned int Arg = 0;
            if (isdigit(*((argv[1]) + 1)))
                Arg = IntFirst * 10 + IntSecond;
            else
                Arg = IntFirst;

            if (Arg != 0)
            {
                printf("\n");
                handle_lenght_specified_case(argv[2], Arg);
            }
            else
            {
                printf("Wrong argument (0 is not allowed)...");
            }
        }
        else
        {
            printf("\nWrong argument...\n\n$ dumpy [bytes per line] [file path]\n\nExample:\ndumpy a.png\ndumpy 8 ./pass.txt\n");
            exit(1);
        }
    }
    else if (argc == 2)
    {
        printf("\n");
        handle_default_case(argv[1]);
    }
    else if (argc > 3)
    {
        printf("\nWrong argument...\n\n$ dumpy [bytes per line] [file path]\n\nExample:\ndumpy a.png\ndumpy 8 ./pass.txt\n");
        exit(1);
    }

    char *buffer = malloc(DEFAULT_SIZE);

    size_t count = 0;
    size_t defSize = DEFAULT_SIZE;
    while (1)
    {
        buffer[count] = getc(stdin);
        if (buffer[count] == EOF)
        {
            buffer[count] = '\0';
            break;
        }
        count++;
        if (count > defSize)
        {
            buffer = (char *)realloc(buffer, defSize * 2);
            defSize = defSize * 2;
        }
    }
    handle_stdin_case(buffer, count);

    return EXIT_SUCCESS;
}
