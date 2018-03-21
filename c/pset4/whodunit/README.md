# Questions

## What's `stdint.h`?

>>The `stdint.h` library defines the size of integer values, usually used in Windows.
>>E.g.
>> - An 8-bit integer is referred to as a `byte`
>> - A 16-bit integet is referred to as a `word`
>> etc.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

>>Using these definitions in a program allows you to not only specify the data type as `int`,
>>but also allows you to specify how many bits the value will use.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

>>`BYTE` == 1 byte (unsigned)
>>
>>`DWORD` == 4 bytes (unsigned)
>>
>>`LONG` == 4 bytes (signed)
>>
>>`WORD` == 2 bytes (unsigned)

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be?
##Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

>>The first two bytes in the Bitmap file header are `BM` (in ASCII), or `0x42 0x4D` (in Hex).

## What's the difference between `bfSize` and `biSize`?

>>`bfSize` is short for `Bitmap File Size` and is the total size of the file (including headers).
>>
>>`biSize` is short for `Bitmap Image Size` and is the total size of the image (not including headers).

## What does it mean if `biHeight` is negative?

>>If `biHeight` is negative, it indicates a `Top-Down Device Independant Bitmap (DIB)`, whereby the
>>the start of the buffer is the top left pixel and the last byte is the bottom right pixel.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

>>The `biBitCount` member specifies the color depth of the image.

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

>>`fopen()` will return `NULL` if the file it is attempting to use doesn't exist, or if it
>>encounters an error while opening it.

## Why is the third argument to `fread` always `1` in our code?

>>In this instance, `fread` is being used to read 1 unit of the size of a bitmap header.
>>In this case, it will read the header and store it exactly as it appears in the origin
>>file, ensuring that there are no formatting errors when writing it to a new file.

## What value does line 63 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

>>`copy.c` will assign a value of 3 to `padding` if the `bi.biWidth` member is `3`.

## What does `fseek` do?

>>`fseek()` allows you to move a specified number of bytes within a file.

## What is `SEEK_CUR`?

>>`SEEK_CUR` is a possible value that can be passed to `fseek()`. When `SEEK_CUR` is
>>passed, it informs `fseek()` to move the specified number of bytes in the file, starting
>>from the current position.
