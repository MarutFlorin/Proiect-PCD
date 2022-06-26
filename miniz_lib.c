// // example1.c - Demonstrates miniz.c's compress() and uncompress() functions (same as zlib's).
// // Public domain, May 15 2011, Rich Geldreich, richgel99@gmail.com. See "unlicense" statement at the end of tinfl.c.
// #include <stdio.h>
// #include "miniz.c"
// typedef unsigned char uint8;
// typedef unsigned short uint16;
// typedef unsigned int uint;

// // The string to compress.
// // static const char *s_pStr = "Good morning Dr. Chandra. This is Hal. I am ready for my first lesson." \
// //   "Good morning Dr. Chandra. This is Hal. I am ready for my first lesson." \
// //   "Good morning Dr. Chandra. This is Hal. I am ready for my first lesson." \
// //   "Good morning Dr. Chandra. This is Hal. I am ready for my first lesson." \
// //   "Good morning Dr. Chandra. This is Hal. I am ready for my first lesson." \
// //   "Good morning Dr. Chandra. This is Hal. I am ready for my first lesson." \
// //   "Good morning Dr. Chandra. This is Hal. I am ready for my first lesson.";


// char * compress_bytes(const char *input)
// {
//   int cmp_status;
//   uLong src_len = (uLong)strlen(input);
//   uLong cmp_len = compressBound(src_len);
//   uint8 *pCmp;
//   uint total_succeeded = 0;

//   // Allocate buffers to hold compressed and uncompressed data.
//     pCmp = (mz_uint8 *)malloc((size_t)cmp_len);
//     if (!pCmp)
//     {
//       printf("Out of memory!\n");
//       return EXIT_FAILURE;
//     }

//     // Compress the string.
//     cmp_status = compress(pCmp, &cmp_len, (const unsigned char *)input, src_len);
//     while(read())

//     if (cmp_status != Z_OK)
//     {
//       printf("compress() failed!\n");
//       free(pCmp);
//       return EXIT_FAILURE;
//     }

//     printf("Compressed from %u to %u bytes\n", (mz_uint32)src_len, (mz_uint32)cmp_len);

//     return pCmp;
// }

// char * decompress_bytes(const char *input)
// {
//   int cmp_status;
//   uLong src_len = (uLong)strlen(input);
//   uLong uncomp_len = src_len;
//   uint8 *pUncomp;
//   uint total_succeeded = 0;

//   cmp_status = uncompress(pUncomp, &uncomp_len, input);
//   total_succeeded += (cmp_status == Z_OK);

//   if (cmp_status != Z_OK)
//   {
//     printf("uncompress failed!\n");
//     free(pCmp);
//     free(pUncomp);
//     return EXIT_FAILURE;
//   }

//   printf("Decompressed from %u to %u bytes\n", (mz_uint32)cmp_len, (mz_uint32)uncomp_len);

//   // Ensure uncompress() returned the expected data.
//   if ((uncomp_len != src_len) || (memcmp(pUncomp, input, (size_t)src_len)))
//   {
//     printf("Decompression failed!\n");
//     free(pCmp);
//     free(pUncomp);
//     return EXIT_FAILURE;
//   }
// }

// int main(int argc, char *argv[])
// {
//   uint step = 0;
//   int cmp_status;
//   uLong src_len = (uLong)strlen(s_pStr);
//   uLong cmp_len = compressBound(src_len);
//   uLong uncomp_len = src_len;
//   uint8 *pCmp, *pUncomp;
//   uint total_succeeded = 0;
//   (void)argc, (void)argv;

//   printf("miniz.c version: %s\n", MZ_VERSION);

//   do
//   {
//     // Allocate buffers to hold compressed and uncompressed data.
//     pCmp = (mz_uint8 *)malloc((size_t)cmp_len);
//     pUncomp = (mz_uint8 *)malloc((size_t)src_len);
//     if ((!pCmp) || (!pUncomp))
//     {
//       printf("Out of memory!\n");
//       return EXIT_FAILURE;
//     }

//     // Compress the string.
//     cmp_status = compress(pCmp, &cmp_len, (const unsigned char *)s_pStr, src_len);
//     fprintf(stdout, s_pStr);
//     while(read())

//     if (cmp_status != Z_OK)
//     {
//       printf("compress() failed!\n");
//       free(pCmp);
//       free(pUncomp);
//       return EXIT_FAILURE;
//     }

//     printf("Compressed from %u to %u bytes\n", (mz_uint32)src_len, (mz_uint32)cmp_len);

//     if (step)
//     {
//       // Purposely corrupt the compressed data if fuzzy testing (this is a very crude fuzzy test).
//       uint n = 1 + (rand() % 3);
//       while (n--)
//       {
//         uint i = rand() % cmp_len;
//         pCmp[i] ^= (rand() & 0xFF);
//       }
//     }

//     // Decompress.
//     cmp_status = uncompress(pUncomp, &uncomp_len, pCmp, cmp_len);
//     total_succeeded += (cmp_status == Z_OK);

//     if (step)
//     {
//       printf("Simple fuzzy test: step %u total_succeeded: %u\n", step, total_succeeded);
//     }
//     else
//     {
//       if (cmp_status != Z_OK)
//       {
//         printf("uncompress failed!\n");
//         free(pCmp);
//         free(pUncomp);
//         return EXIT_FAILURE;
//       }

//       printf("Decompressed from %u to %u bytes\n", (mz_uint32)cmp_len, (mz_uint32)uncomp_len);

//       // Ensure uncompress() returned the expected data.
//       if ((uncomp_len != src_len) || (memcmp(pUncomp, s_pStr, (size_t)src_len)))
//       {
//         printf("Decompression failed!\n");
//         free(pCmp);
//         free(pUncomp);
//         return EXIT_FAILURE;
//       }
//     }

//     free(pCmp);
//     free(pUncomp);

//     step++;

//     // Keep on fuzzy testing if there's a non-empty command line.
//   } while (argc >= 2);

//   printf("Success.\n");
//   return EXIT_SUCCESS;
// }

// example3.c - Demonstrates how to use miniz.c's deflate() and inflate() functions for simple file compression.
// Public domain, May 15 2011, Rich Geldreich, richgel99@gmail.com. See "unlicense" statement at the end of tinfl.c.
// For simplicity, this example is limited to files smaller than 4GB, but this is not a limitation of miniz.c.
#include <stdio.h>
#include <limits.h>
#include "miniz.c"

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint;

#define my_max(a,b) (((a) > (b)) ? (a) : (b))
#define my_min(a,b) (((a) < (b)) ? (a) : (b))

#define BUF_SIZE (1024 * 1024)
static uint8 s_inbuf[BUF_SIZE];
//static uint8 s_outbuf[BUF_SIZE];

void mess_file(const char *input_file, const char *output_file, const char *mode, int m_level)
{
  static uint8 s_outbuf[BUF_SIZE];
  const char *pMode;
  FILE *pInfile, *pOutfile;
  uint infile_size;
  int level = Z_BEST_COMPRESSION;
  z_stream stream;
  int p = 1;
  const char *pSrc_filename;
  const char *pDst_filename;
  long file_loc;

  // printf("miniz.c version: %s\n", MZ_VERSION);

  // if (argc < 4)
  // {
  //   printf("Usage: example3 [options] [mode:c or d] infile outfile\n");
  //   printf("\nModes:\n");
  //   printf("c - Compresses file infile to a zlib stream in file outfile\n");
  //   printf("d - Decompress zlib stream in file infile to file outfile\n");
  //   printf("\nOptions:\n");
  //   printf("-l[0-10] - Compression level, higher values are slower.\n");
  //   return EXIT_FAILURE;
  // }

  // while ((p < argc) && (argv[p][0] == '-'))
  // {
  //   switch (argv[p][1])
  //   {
  //     case 'l':
  //     {
  //       level = atoi(&argv[1][2]);
  //       if ((level < 0) || (level > 10))
  //       {
  //         printf("Invalid level!\n");
  //         return EXIT_FAILURE;
  //       }
  //       break;
  //     }
  //     default:
  //     {
  //       printf("Invalid option: %s\n", argv[p]);
  //       return EXIT_FAILURE;
  //     }
  //   }
  //   p++;
  // }

  // if ((argc - p) < 3)
  // {
  //   printf("Must specify mode, input filename, and output filename after options!\n");
  //   return EXIT_FAILURE;
  // }
  // else if ((argc - p) > 3)
  // {
  //   printf("Too many filenames!\n");
  //   return EXIT_FAILURE;
  // }

  // pMode = argv[p++];
  // if (!strchr("cCdD", pMode[0]))
  // {
  //   printf("Invalid mode!\n");
  //   return EXIT_FAILURE;
  // }

  pMode = mode;
  level = atoi(&m_level);
  pSrc_filename = input_file;
  pDst_filename = output_file;

  printf("Mode: %c, Level: %u\nInput File: \"%s\"\nOutput File: \"%s\"\n", pMode[0], level, pSrc_filename, pDst_filename);

  // Open input file.
  pInfile = fopen(pSrc_filename, "rb");
  if (!pInfile)
  {
    printf("Failed opening input file!\n");
    return EXIT_FAILURE;
  }

  // Determine input file's size.
  fseek(pInfile, 0, SEEK_END);
  file_loc = ftell(pInfile);
  fseek(pInfile, 0, SEEK_SET);

  if ((file_loc < 0) || (file_loc > INT_MAX))
  {
     // This is not a limitation of miniz or tinfl, but this example.
     printf("File is too large to be processed by this example.\n");
     return EXIT_FAILURE;
  }

  infile_size = (uint)file_loc;

  // Open output file.
  pOutfile = fopen(pDst_filename, "wb");
  if (!pOutfile)
  {
    printf("Failed opening output file!\n");
    return EXIT_FAILURE;
  }

  printf("Input file size: %u\n", infile_size);

  // Init the z_stream
  memset(&stream, 0, sizeof(stream));
  stream.next_in = s_inbuf;
  stream.avail_in = 0;
  stream.next_out = s_outbuf;
  stream.avail_out = BUF_SIZE;

  if ((pMode[0] == 'c') || (pMode[0] == 'C'))
  {
    // Compression.
    uint infile_remaining = infile_size;

    if (deflateInit(&stream, level) != Z_OK)
    {
      printf("deflateInit() failed!\n");
      return EXIT_FAILURE;
    }

    for ( ; ; )
    {
      int status;
      if (!stream.avail_in)
      {
        // Input buffer is empty, so read more bytes from input file.
        uint n = my_min(BUF_SIZE, infile_remaining);

        if (fread(s_inbuf, 1, n, pInfile) != n)
        {
          printf("Failed reading from input file!\n");
          return EXIT_FAILURE;
        }

        stream.next_in = s_inbuf;
        stream.avail_in = n;

        infile_remaining -= n;
        //printf("Input bytes remaining: %u\n", infile_remaining);
      }

      status = deflate(&stream, infile_remaining ? Z_NO_FLUSH : Z_FINISH);

      if ((status == Z_STREAM_END) || (!stream.avail_out))
      {
        // Output buffer is full, or compression is done, so write buffer to output file.
        uint n = BUF_SIZE - stream.avail_out;
        if (fwrite(s_outbuf, 1, n, pOutfile) != n)
        {
          printf("Failed writing to output file!\n");
          return EXIT_FAILURE;
        }
        
        
        stream.next_out = s_outbuf;
        stream.avail_out = BUF_SIZE;
      }

      if (status == Z_STREAM_END)
        break;
      else if (status != Z_OK)
      {
        printf("deflate() failed with status %i!\n", status);
        return EXIT_FAILURE;
      }
    }

    if (deflateEnd(&stream) != Z_OK)
    {
      printf("deflateEnd() failed!\n");
      return EXIT_FAILURE;
    }
  }
  else if ((pMode[0] == 'd') || (pMode[0] == 'D'))
  {
    // Decompression.
    uint infile_remaining = infile_size;

    if (inflateInit(&stream))
    {
      printf("inflateInit() failed!\n");
      return EXIT_FAILURE;
    }

    for ( ; ; )
    {
      int status;
      if (!stream.avail_in)
      {
        // Input buffer is empty, so read more bytes from input file.
        uint n = my_min(BUF_SIZE, infile_remaining);

        if (fread(s_inbuf, 1, n, pInfile) != n)
        {
          printf("Failed reading from input file!\n");
          return EXIT_FAILURE;
        }

        stream.next_in = s_inbuf;
        stream.avail_in = n;

        infile_remaining -= n;
      }

      status = inflate(&stream, Z_SYNC_FLUSH);

      if ((status == Z_STREAM_END) || (!stream.avail_out))
      {
        // Output buffer is full, or decompression is done, so write buffer to output file.
        uint n = BUF_SIZE - stream.avail_out;
        if (fwrite(s_outbuf, 1, n, pOutfile) != n)
        {
          printf("Failed writing to output file!\n");
          return EXIT_FAILURE;
        }
        stream.next_out = s_outbuf;
        stream.avail_out = BUF_SIZE;
      }

      if (status == Z_STREAM_END)
        break;
      else if (status != Z_OK)
      {
        printf("inflate() failed with status %i!\n", status);
        return EXIT_FAILURE;
      }
    }

    if (inflateEnd(&stream) != Z_OK)
    {
      printf("inflateEnd() failed!\n");
      return EXIT_FAILURE;
    }
  }
  else
  {
    printf("Invalid mode!\n");
    return EXIT_FAILURE;
  }

  fclose(pInfile);
  if (EOF == fclose(pOutfile))
  {
    printf("Failed writing to output file!\n");
    return EXIT_FAILURE;
  }

  printf("Total input bytes: %u\n", (mz_uint32)stream.total_in);
  printf("Total output bytes: %u\n", (mz_uint32)stream.total_out);
  printf("Success.\n");
}
