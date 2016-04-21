/*
    this code has been written for an article about the "transformer"
    challenge found in the VolgaCTF 2016 Quals.

    Blog post: https://alessandrogar.io/ctf/volgactf-2016-quals-transformer-writeup/
*/

#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdint>
#include <memory>

union uint128_t final
{
    std::uint32_t dwords[4];
    std::uint8_t bytes[16];
};

int main(int argc, char *argv[]);

int Encoder(int argc, char *argv[]);
void sub_401836(std::uint8_t *buffer, std::streamsize buffer_size);
void sub_40179e(uint128_t &xmm0);
void sub_4015d0(uint128_t &xmm7);
void sub_40167d(uint128_t &xmm7);
void sub_40161f(uint32_t &value);
void sub_40169f(uint128_t &xmm7);
void sub_4016e9(uint32_t &value);

int Decoder(int argc, char *argv[]);
void sub_401836_decoder(std::uint8_t *buffer, std::streamsize buffer_size);
void sub_40179e_decoder(uint128_t &encoded_xmmword);
void sub_40167d_decoder(uint128_t &encoded_xmmword);
void sub_4015d0_decoder(uint128_t &encoded_xmmword);
void sub_40161f_decoder(std::uint32_t &encoded_dword);
void sub_40169f_decoder(uint128_t &encoded_xmmword);
void sub_4016e9_decoder(std::uint32_t &encoded_dword);

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cout << "Usage: \n";
        std::cout << "\t" << argv[0] << " encode <input_file> <output_file>\n";
        std::cout << "\t" << argv[0] << " decode <input_file> <output_file>" << std::endl;

        return 1;
    }

    if (std::strcmp(argv[1], "encode") == 0)
        return Encoder(argc, argv);

    else if (std::strcmp(argv[1], "decode") == 0)
        return Decoder(argc, argv);

    else
    {
        std::cout << "Invalid operation type" << std::endl;
        return 1;
    }
}

int Encoder(int argc, char *argv[])
{
    (void) argc;

    const char *input_file_path = argv[2];
    const char *output_file_path = argv[3];

    // 0040111a
    std::fstream input_file(input_file_path);
    if (!input_file)
    {
        std::cout << "Failed to open the input file" << std::endl;
        return 1;
    }

    // 00401135
    std::fstream output_file(output_file_path, std::fstream::out | std::fstream::trunc);
    if (!output_file)
    {
        std::cout << "Failed to open the input file" << std::endl;
        return 1;
    }

    // 004011c5
    input_file.seekg(0, std::ios_base::end);

    // 004011d2
    std::streamsize file_size = input_file.tellg();

    // 004011ee
    input_file.seekg(0, std::ios_base::beg);

    std::unique_ptr<std::uint8_t> buffer;

    try
    {
        // 004011fa
        unsigned long buffer_size = static_cast<unsigned long>(file_size);
        buffer = std::unique_ptr<std::uint8_t>(new std::uint8_t[buffer_size]);
    }

    catch (std::bad_alloc &)
    {
        std::cout << "Failed to allocate the buffer" << std::endl;
        return 1;
    }

    // 0040121D
    input_file.read(reinterpret_cast<char *>(buffer.get()), file_size);

    // 0040123A
    // note that in the original binary the buffer is passed twice!
    sub_401836(buffer.get(), file_size);

    // 00401248
    output_file.write(reinterpret_cast<const char *>(buffer.get()), file_size);

    return 0;
}

void sub_401836(std::uint8_t *buffer, std::streamsize buffer_size)
{
    // 00401860
    std::uint32_t block_count = static_cast<std::uint32_t>(buffer_size / 0x10);

    // 00401870
    if (block_count == 0)
        return;

    do
    {
        // 00401877
        uint128_t xmm0;
        std::memcpy(xmm0.bytes, buffer, sizeof(uint128_t));

        // 0040187b
        sub_40179e(xmm0);

        // 00401880
        std::memcpy(buffer, &xmm0, sizeof(uint128_t));

        // 00401884
        buffer += 0x10;

        // 0040188c
        block_count--;
    } while (block_count != 0); // 00401864

    // 0040189f
    return;
}

void sub_40179e(uint128_t &xmm0)
{
    const unsigned char xmmword_602128_bytes[2816] =
    {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c,
        0xa0, 0xfa, 0xfe, 0x17, 0x88, 0x54, 0x2c, 0xb1, 0x23, 0xa3, 0x39, 0x39, 0x2a, 0x6c, 0x76, 0x05,
        0xf2, 0xc2, 0x95, 0xf2, 0x7a, 0x96, 0xb9, 0x43, 0x59, 0x35, 0x80, 0x7a, 0x73, 0x59, 0xf6, 0x7f,
        0x3d, 0x80, 0x47, 0x7d, 0x47, 0x16, 0xfe, 0x3e, 0x1e, 0x23, 0x7e, 0x44, 0x6d, 0x7a, 0x88, 0x3b,
        0xef, 0x44, 0xa5, 0x41, 0xa8, 0x52, 0x5b, 0x7f, 0xb6, 0x71, 0x25, 0x3b, 0xdb, 0x0b, 0xad, 0x00,
        0xd4, 0xd1, 0xc6, 0xf8, 0x7c, 0x83, 0x9d, 0x87, 0xca, 0xf2, 0xb8, 0xbc, 0x11, 0xf9, 0x15, 0xbc,
        0x6d, 0x88, 0xa3, 0x7a, 0x11, 0x0b, 0x3e, 0xfd, 0xdb, 0xf9, 0x86, 0x41, 0xca, 0x00, 0x93, 0xfd,
        0x4e, 0x54, 0xf7, 0x0e, 0x5f, 0x5f, 0xc9, 0xf3, 0x84, 0xa6, 0x4f, 0xb2, 0x4e, 0xa6, 0xdc, 0x4f,
        0xea, 0xd2, 0x73, 0x21, 0xb5, 0x8d, 0xba, 0xd2, 0x31, 0x2b, 0xf5, 0x60, 0x7f, 0x8d, 0x29, 0x2f,
        0xac, 0x77, 0x66, 0xf3, 0x19, 0xfa, 0xdc, 0x21, 0x28, 0xd1, 0x29, 0x41, 0x57, 0x5c, 0x00, 0x6e,
        0xd0, 0x14, 0xf9, 0xa8, 0xc9, 0xee, 0x25, 0x89, 0xe1, 0x3f, 0x0c, 0xc8, 0xb6, 0x63, 0x0c, 0xa6
    };

    // 00602118
    const std::uint8_t xmmword_602118[16] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C };

    // 004017ac
    uint128_t xmm7 = xmm0;

    // 004017c8
    // 004017d1
    for (std::size_t i = 0; i < 16; i++)
        xmm7.bytes[i] ^= xmmword_602118[i];

    // 004017d6
    std::size_t r15;

    // 00401802
    for (r15 = 0x10; r15 < 0xA0; r15 += 0x10)
    {
        // 004017e1
        sub_4015d0(xmm7);

        // 004017e6
        sub_40167d(xmm7);

        // 004017eb
        sub_40169f(xmm7);

        // 004017f0
        xmm0 = *reinterpret_cast<const uint128_t *>(&xmmword_602128_bytes[r15]);

        // 004017f9
        for (std::size_t i = 0; i < 16; i++)
            xmm7.bytes[i] ^= xmm0.bytes[i];
    }

    // 0040180b
    sub_4015d0(xmm7);

    // 00401810
    sub_40167d(xmm7);

    // 00401815
    xmm0 = *reinterpret_cast<const uint128_t *>(&xmmword_602128_bytes[r15]);

    // 0040181e
    for (std::size_t i = 0; i < 16; i++)
        xmm7.bytes[i] ^= xmm0.bytes[i];

    // 00401823
    xmm0 = xmm7;

    // 00401835
    return;
}

void sub_4015d0(uint128_t &xmm7)
{
    // 004015de
    for (std::size_t i = 0; i < 4; i++)
        sub_40161f(xmm7.dwords[i]);

    // 0040161e
    return;
}

void sub_40169f(uint128_t &xmm7)
{
    // 004016ad
    for (std::size_t i = 0; i < 4; i++)
        sub_4016e9(xmm7.dwords[i]);

    // 004016e8
    return;
}

void sub_40161f(std::uint32_t &value)
{
    const unsigned char byte_602268[256] =
    {
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01,
        0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, 0xca, 0x82, 0xc9, 0x7d,
        0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4,
        0x72, 0xc0, 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
        0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, 0x04, 0xc7,
        0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2,
        0xeb, 0x27, 0xb2, 0x75, 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e,
        0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb,
        0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, 0xd0, 0xef, 0xaa, 0xfb,
        0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c,
        0x9f, 0xa8, 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
        0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 0xcd, 0x0c,
        0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d,
        0x64, 0x5d, 0x19, 0x73, 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a,
        0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3,
        0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, 0xe7, 0xc8, 0x37, 0x6d,
        0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a,
        0xae, 0x08, 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
        0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, 0x70, 0x3e,
        0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9,
        0x86, 0xc1, 0x1d, 0x9e, 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9,
        0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99,
        0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
    };

    union
    {
        std::uint32_t eax;
        std::uint8_t eax_bytes[4];
    };

    union
    {
        std::uint32_t ebx;
        std::uint8_t ebx_bytes[4];
    };

    ebx = value;

    // 00401646
    eax_bytes[0] = byte_602268[ebx_bytes[0]];

    // 00401653
    eax_bytes[1] = byte_602268[ebx_bytes[1]];

    // 0040165A
    eax <<= 0x10;

    // 0040165D
    ebx >>= 0x10;

    // 00401663
    eax_bytes[0] = byte_602268[ebx_bytes[0]];

    // 0040166D
    eax_bytes[1] = byte_602268[ebx_bytes[1]];

    // 00401674
    std::uint32_t low_word = eax & 0xFFFF;
    std::uint32_t high_word = eax & 0xFFFF0000;

    value = (low_word << 0x10) | (high_word >> 0x10);

    // 0040167C
    return;
}

// 0040167d
void sub_40167d(uint128_t &xmm7)
{
    // 00401694
    const std::uint8_t xmmword_401683[16] = { 0x00, 0x05, 0x0A, 0x0F, 0x04, 0x09, 0x0E, 0x03, 0x08, 0x0D, 0x02, 0x07, 0x0C, 0x01, 0x06, 0x0B };

    // 00401698 (pshufb)
    std::uint8_t output[16];

    for (std::size_t i = 0; i < 16; i++)
    {
        if ((xmmword_401683[i] & 0x80) != 0)
            output[i] = 0;

        else
        {
            std::size_t index = xmmword_401683[i] & 0x0F;
            output[i] = xmm7.bytes[index];
        }
    }

    for (std::size_t i = 0; i < 16; i++)
        xmm7.bytes[i] = output[i];

    // 0040169e
    return;
}

void sub_4016e9(std::uint32_t &value)
{
    const unsigned char byte_602394[256] =
    {
        0x00, 0x02, 0x04, 0x06, 0x08, 0x0a, 0x0c, 0x0e, 0x10, 0x12,
        0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26,
        0x28, 0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a,
        0x3c, 0x3e, 0x40, 0x42, 0x44, 0x46, 0x48, 0x4a, 0x4c, 0x4e,
        0x50, 0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c, 0x5e, 0x60, 0x62,
        0x64, 0x66, 0x68, 0x6a, 0x6c, 0x6e, 0x70, 0x72, 0x74, 0x76,
        0x78, 0x7a, 0x7c, 0x7e, 0x80, 0x82, 0x84, 0x86, 0x88, 0x8a,
        0x8c, 0x8e, 0x90, 0x92, 0x94, 0x96, 0x98, 0x9a, 0x9c, 0x9e,
        0xa0, 0xa2, 0xa4, 0xa6, 0xa8, 0xaa, 0xac, 0xae, 0xb0, 0xb2,
        0xb4, 0xb6, 0xb8, 0xba, 0xbc, 0xbe, 0xc0, 0xc2, 0xc4, 0xc6,
        0xc8, 0xca, 0xcc, 0xce, 0xd0, 0xd2, 0xd4, 0xd6, 0xd8, 0xda,
        0xdc, 0xde, 0xe0, 0xe2, 0xe4, 0xe6, 0xe8, 0xea, 0xec, 0xee,
        0xf0, 0xf2, 0xf4, 0xf6, 0xf8, 0xfa, 0xfc, 0xfe, 0x1b, 0x19,
        0x1f, 0x1d, 0x13, 0x11, 0x17, 0x15, 0x0b, 0x09, 0x0f, 0x0d,
        0x03, 0x01, 0x07, 0x05, 0x3b, 0x39, 0x3f, 0x3d, 0x33, 0x31,
        0x37, 0x35, 0x2b, 0x29, 0x2f, 0x2d, 0x23, 0x21, 0x27, 0x25,
        0x5b, 0x59, 0x5f, 0x5d, 0x53, 0x51, 0x57, 0x55, 0x4b, 0x49,
        0x4f, 0x4d, 0x43, 0x41, 0x47, 0x45, 0x7b, 0x79, 0x7f, 0x7d,
        0x73, 0x71, 0x77, 0x75, 0x6b, 0x69, 0x6f, 0x6d, 0x63, 0x61,
        0x67, 0x65, 0x9b, 0x99, 0x9f, 0x9d, 0x93, 0x91, 0x97, 0x95,
        0x8b, 0x89, 0x8f, 0x8d, 0x83, 0x81, 0x87, 0x85, 0xbb, 0xb9,
        0xbf, 0xbd, 0xb3, 0xb1, 0xb7, 0xb5, 0xab, 0xa9, 0xaf, 0xad,
        0xa3, 0xa1, 0xa7, 0xa5, 0xdb, 0xd9, 0xdf, 0xdd, 0xd3, 0xd1,
        0xd7, 0xd5, 0xcb, 0xc9, 0xcf, 0xcd, 0xc3, 0xc1, 0xc7, 0xc5,
        0xfb, 0xf9, 0xff, 0xfd, 0xf3, 0xf1, 0xf7, 0xf5, 0xeb, 0xe9,
        0xef, 0xed, 0xe3, 0xe1, 0xe7, 0xe5
    };

    const unsigned char byte_602494[256] =
    {
        0x00, 0x03, 0x06, 0x05, 0x0c, 0x0f, 0x0a, 0x09, 0x18, 0x1b,
        0x1e, 0x1d, 0x14, 0x17, 0x12, 0x11, 0x30, 0x33, 0x36, 0x35,
        0x3c, 0x3f, 0x3a, 0x39, 0x28, 0x2b, 0x2e, 0x2d, 0x24, 0x27,
        0x22, 0x21, 0x60, 0x63, 0x66, 0x65, 0x6c, 0x6f, 0x6a, 0x69,
        0x78, 0x7b, 0x7e, 0x7d, 0x74, 0x77, 0x72, 0x71, 0x50, 0x53,
        0x56, 0x55, 0x5c, 0x5f, 0x5a, 0x59, 0x48, 0x4b, 0x4e, 0x4d,
        0x44, 0x47, 0x42, 0x41, 0xc0, 0xc3, 0xc6, 0xc5, 0xcc, 0xcf,
        0xca, 0xc9, 0xd8, 0xdb, 0xde, 0xdd, 0xd4, 0xd7, 0xd2, 0xd1,
        0xf0, 0xf3, 0xf6, 0xf5, 0xfc, 0xff, 0xfa, 0xf9, 0xe8, 0xeb,
        0xee, 0xed, 0xe4, 0xe7, 0xe2, 0xe1, 0xa0, 0xa3, 0xa6, 0xa5,
        0xac, 0xaf, 0xaa, 0xa9, 0xb8, 0xbb, 0xbe, 0xbd, 0xb4, 0xb7,
        0xb2, 0xb1, 0x90, 0x93, 0x96, 0x95, 0x9c, 0x9f, 0x9a, 0x99,
        0x88, 0x8b, 0x8e, 0x8d, 0x84, 0x87, 0x82, 0x81, 0x9b, 0x98,
        0x9d, 0x9e, 0x97, 0x94, 0x91, 0x92, 0x83, 0x80, 0x85, 0x86,
        0x8f, 0x8c, 0x89, 0x8a, 0xab, 0xa8, 0xad, 0xae, 0xa7, 0xa4,
        0xa1, 0xa2, 0xb3, 0xb0, 0xb5, 0xb6, 0xbf, 0xbc, 0xb9, 0xba,
        0xfb, 0xf8, 0xfd, 0xfe, 0xf7, 0xf4, 0xf1, 0xf2, 0xe3, 0xe0,
        0xe5, 0xe6, 0xef, 0xec, 0xe9, 0xea, 0xcb, 0xc8, 0xcd, 0xce,
        0xc7, 0xc4, 0xc1, 0xc2, 0xd3, 0xd0, 0xd5, 0xd6, 0xdf, 0xdc,
        0xd9, 0xda, 0x5b, 0x58, 0x5d, 0x5e, 0x57, 0x54, 0x51, 0x52,
        0x43, 0x40, 0x45, 0x46, 0x4f, 0x4c, 0x49, 0x4a, 0x6b, 0x68,
        0x6d, 0x6e, 0x67, 0x64, 0x61, 0x62, 0x73, 0x70, 0x75, 0x76,
        0x7f, 0x7c, 0x79, 0x7a, 0x3b, 0x38, 0x3d, 0x3e, 0x37, 0x34,
        0x31, 0x32, 0x23, 0x20, 0x25, 0x26, 0x2f, 0x2c, 0x29, 0x2a,
        0x0b, 0x08, 0x0d, 0x0e, 0x07, 0x04, 0x01, 0x02, 0x13, 0x10,
        0x15, 0x16, 0x1f, 0x1c, 0x19, 0x1a
    };

    union
    {
        std::uint32_t edi;
        std::uint8_t edi_bytes[4];
    };

    union
    {
        std::uint32_t eax;
        std::uint8_t eax_bytes[4];
    };

    // 00401709
    edi = value;
    edi = ((byte_602494[edi_bytes[0]] ^ edi_bytes[1]) ^ edi_bytes[2]) ^ byte_602394[edi_bytes[3]];

    // 00401726
    eax_bytes[3] = edi_bytes[0];

    // 0040172B
    edi = value;
    edi = ((byte_602394[edi_bytes[2]] ^ edi_bytes[0]) ^ edi_bytes[1]) ^ byte_602494[edi_bytes[3]];

    // 401748
    eax_bytes[2] = edi_bytes[0];

    // 00401752
    edi = value;
    edi = ((byte_602394[edi_bytes[1]] ^ edi_bytes[0]) ^ byte_602494[edi_bytes[2]]) ^ edi_bytes[3];

    // 0040176F
    eax_bytes[1] = edi_bytes[0];

    // 00401774
    edi = value;
    edi = ((byte_602394[edi_bytes[0]] ^ byte_602494[edi_bytes[1]]) ^ edi_bytes[2]) ^ edi_bytes[3];

    // 00401791
    eax_bytes[0] = edi_bytes[0];

    // 00401793
    value = eax;
    return;
}

int Decoder(int argc, char *argv[])
{
    (void) argc;

    const char *input_file_path = argv[2];
    const char *output_file_path = argv[3];

    std::fstream input_file(input_file_path);
    if (!input_file)
    {
        std::cout << "Failed to open the input file" << std::endl;
        return 1;
    }

    std::fstream output_file(output_file_path, std::fstream::out | std::fstream::trunc);
    if (!output_file)
    {
        std::cout << "Failed to open the input file" << std::endl;
        return 1;
    }

    input_file.seekg(0, std::ios_base::end);
    std::streamsize file_size = input_file.tellg();
    input_file.seekg(0, std::ios_base::beg);

    std::unique_ptr<std::uint8_t> buffer;

    try
    {
        unsigned long buffer_size = static_cast<unsigned long>(file_size);
        buffer = std::unique_ptr<std::uint8_t>(new std::uint8_t[buffer_size]);
    }

    catch (std::bad_alloc &)
    {
        std::cout << "Failed to allocate the buffer" << std::endl;
        return 1;
    }

    input_file.read(reinterpret_cast<char *>(buffer.get()), file_size);

    sub_401836_decoder(buffer.get(), file_size);

    output_file.write(reinterpret_cast<const char *>(buffer.get()), file_size);
    return 0;
}

void sub_401836_decoder(std::uint8_t *buffer, std::streamsize buffer_size)
{
    std::uint32_t block_count = static_cast<std::uint32_t>(buffer_size / 0x10);
    if (block_count == 0)
        return;

    do
    {
        uint128_t xmmword;
        std::memcpy(xmmword.bytes, buffer, sizeof(uint128_t));

        sub_40179e_decoder(xmmword);
        std::memcpy(buffer, &xmmword, sizeof(uint128_t));

        buffer += 0x10;
        block_count--;
    } while (block_count != 0);
}

void sub_40179e_decoder(uint128_t &encoded_xmmword)
{
    const unsigned char xmmword_602128_bytes[2816] =
    {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c,
        0xa0, 0xfa, 0xfe, 0x17, 0x88, 0x54, 0x2c, 0xb1, 0x23, 0xa3, 0x39, 0x39, 0x2a, 0x6c, 0x76, 0x05,
        0xf2, 0xc2, 0x95, 0xf2, 0x7a, 0x96, 0xb9, 0x43, 0x59, 0x35, 0x80, 0x7a, 0x73, 0x59, 0xf6, 0x7f,
        0x3d, 0x80, 0x47, 0x7d, 0x47, 0x16, 0xfe, 0x3e, 0x1e, 0x23, 0x7e, 0x44, 0x6d, 0x7a, 0x88, 0x3b,
        0xef, 0x44, 0xa5, 0x41, 0xa8, 0x52, 0x5b, 0x7f, 0xb6, 0x71, 0x25, 0x3b, 0xdb, 0x0b, 0xad, 0x00,
        0xd4, 0xd1, 0xc6, 0xf8, 0x7c, 0x83, 0x9d, 0x87, 0xca, 0xf2, 0xb8, 0xbc, 0x11, 0xf9, 0x15, 0xbc,
        0x6d, 0x88, 0xa3, 0x7a, 0x11, 0x0b, 0x3e, 0xfd, 0xdb, 0xf9, 0x86, 0x41, 0xca, 0x00, 0x93, 0xfd,
        0x4e, 0x54, 0xf7, 0x0e, 0x5f, 0x5f, 0xc9, 0xf3, 0x84, 0xa6, 0x4f, 0xb2, 0x4e, 0xa6, 0xdc, 0x4f,
        0xea, 0xd2, 0x73, 0x21, 0xb5, 0x8d, 0xba, 0xd2, 0x31, 0x2b, 0xf5, 0x60, 0x7f, 0x8d, 0x29, 0x2f,
        0xac, 0x77, 0x66, 0xf3, 0x19, 0xfa, 0xdc, 0x21, 0x28, 0xd1, 0x29, 0x41, 0x57, 0x5c, 0x00, 0x6e,
        0xd0, 0x14, 0xf9, 0xa8, 0xc9, 0xee, 0x25, 0x89, 0xe1, 0x3f, 0x0c, 0xc8, 0xb6, 0x63, 0x0c, 0xa6
    };

    const std::uint8_t xmmword_602118[16] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C };

    uint128_t xmm7 = *reinterpret_cast<const uint128_t *>(&xmmword_602128_bytes[0xA0]);

    for (std::size_t i = 0; i < 16; i++)
        encoded_xmmword.bytes[i] ^= xmm7.bytes[i];

    sub_40167d_decoder(encoded_xmmword);
    sub_4015d0_decoder(encoded_xmmword);

    for (std::size_t r15 = 0x90; r15 >= 0x10; r15 -= 0x10)
    {
        xmm7 = *reinterpret_cast<const uint128_t *>(&xmmword_602128_bytes[r15]);

        for (std::size_t i = 0; i < 16; i++)
            encoded_xmmword.bytes[i] ^= xmm7.bytes[i];

        sub_40169f_decoder(encoded_xmmword);
        sub_40167d_decoder(encoded_xmmword);
        sub_4015d0_decoder(encoded_xmmword);
    }

    for (std::size_t i = 0; i < 16; i++)
        encoded_xmmword.bytes[i] ^= xmmword_602118[i];
}

void sub_40167d_decoder(uint128_t &encoded_xmmword)
{
    const std::uint8_t xmmword_401683[16] = { 0x00, 0x05, 0x0A, 0x0F, 0x04, 0x09, 0x0E, 0x03, 0x08, 0x0D, 0x02, 0x07, 0x0C, 0x01, 0x06, 0x0B };

    std::uint8_t output[16];
    for (std::size_t i = 0; i < 16; i++)
        output[xmmword_401683[i]] = encoded_xmmword.bytes[i];

    for (std::size_t i = 0; i < 16; i++)
        encoded_xmmword.bytes[i] = output[i];
}

void sub_4015d0_decoder(uint128_t &encoded_xmmword)
{
    for (std::size_t i = 0; i < 4; i++)
        sub_40161f_decoder(encoded_xmmword.dwords[i]);
}

void sub_40161f_decoder(std::uint32_t &encoded_dword)
{
    const unsigned char byte_602268[256] =
    {
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01,
        0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, 0xca, 0x82, 0xc9, 0x7d,
        0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4,
        0x72, 0xc0, 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
        0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, 0x04, 0xc7,
        0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2,
        0xeb, 0x27, 0xb2, 0x75, 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e,
        0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb,
        0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, 0xd0, 0xef, 0xaa, 0xfb,
        0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c,
        0x9f, 0xa8, 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
        0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 0xcd, 0x0c,
        0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d,
        0x64, 0x5d, 0x19, 0x73, 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a,
        0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3,
        0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, 0xe7, 0xc8, 0x37, 0x6d,
        0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a,
        0xae, 0x08, 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
        0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, 0x70, 0x3e,
        0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9,
        0x86, 0xc1, 0x1d, 0x9e, 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9,
        0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99,
        0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
    };

    union
    {
        std::uint32_t eax;
        std::uint8_t eax_bytes[4];
    };

    union
    {
        std::uint32_t ebx;
        std::uint8_t ebx_bytes[4];
    };

    auto L_LookupValue = [byte_602268](std::uint8_t value, std::uint8_t &index) -> bool
    {
        for (std::size_t i = 0; i < sizeof(byte_602268); i++)
        {
            if (value != byte_602268[i])
                continue;

            index = static_cast<std::uint8_t>(i);
            return true;
        }

        return false;
    };

    eax = encoded_dword;

    if (!L_LookupValue(eax_bytes[1], ebx_bytes[1]))
        throw std::runtime_error("Failed to lookup the index");

    if (!L_LookupValue(eax_bytes[0], ebx_bytes[0]))
        throw std::runtime_error("Failed to lookup the index");

    eax >>= 0x10;
    ebx <<= 0x10;

    if (!L_LookupValue(eax_bytes[1], ebx_bytes[1]))
        throw std::runtime_error("Failed to lookup the index");

    if (!L_LookupValue(eax_bytes[0], ebx_bytes[0]))
        throw std::runtime_error("Failed to lookup the index");

    std::uint32_t low_word = ebx & 0xFFFF;
    std::uint32_t high_word = ebx & 0xFFFF0000;

    encoded_dword = (low_word << 0x10) | (high_word >> 0x10);
}

void sub_40169f_decoder(uint128_t &encoded_xmmword)
{
    for (std::size_t i = 0; i < 4; i++)
        sub_4016e9_decoder(encoded_xmmword.dwords[i]);
}

void sub_4016e9_decoder(std::uint32_t &encoded_dword)
{
    const unsigned char byte_602394[256] =
    {
        0x00, 0x02, 0x04, 0x06, 0x08, 0x0a, 0x0c, 0x0e, 0x10, 0x12,
        0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26,
        0x28, 0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a,
        0x3c, 0x3e, 0x40, 0x42, 0x44, 0x46, 0x48, 0x4a, 0x4c, 0x4e,
        0x50, 0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c, 0x5e, 0x60, 0x62,
        0x64, 0x66, 0x68, 0x6a, 0x6c, 0x6e, 0x70, 0x72, 0x74, 0x76,
        0x78, 0x7a, 0x7c, 0x7e, 0x80, 0x82, 0x84, 0x86, 0x88, 0x8a,
        0x8c, 0x8e, 0x90, 0x92, 0x94, 0x96, 0x98, 0x9a, 0x9c, 0x9e,
        0xa0, 0xa2, 0xa4, 0xa6, 0xa8, 0xaa, 0xac, 0xae, 0xb0, 0xb2,
        0xb4, 0xb6, 0xb8, 0xba, 0xbc, 0xbe, 0xc0, 0xc2, 0xc4, 0xc6,
        0xc8, 0xca, 0xcc, 0xce, 0xd0, 0xd2, 0xd4, 0xd6, 0xd8, 0xda,
        0xdc, 0xde, 0xe0, 0xe2, 0xe4, 0xe6, 0xe8, 0xea, 0xec, 0xee,
        0xf0, 0xf2, 0xf4, 0xf6, 0xf8, 0xfa, 0xfc, 0xfe, 0x1b, 0x19,
        0x1f, 0x1d, 0x13, 0x11, 0x17, 0x15, 0x0b, 0x09, 0x0f, 0x0d,
        0x03, 0x01, 0x07, 0x05, 0x3b, 0x39, 0x3f, 0x3d, 0x33, 0x31,
        0x37, 0x35, 0x2b, 0x29, 0x2f, 0x2d, 0x23, 0x21, 0x27, 0x25,
        0x5b, 0x59, 0x5f, 0x5d, 0x53, 0x51, 0x57, 0x55, 0x4b, 0x49,
        0x4f, 0x4d, 0x43, 0x41, 0x47, 0x45, 0x7b, 0x79, 0x7f, 0x7d,
        0x73, 0x71, 0x77, 0x75, 0x6b, 0x69, 0x6f, 0x6d, 0x63, 0x61,
        0x67, 0x65, 0x9b, 0x99, 0x9f, 0x9d, 0x93, 0x91, 0x97, 0x95,
        0x8b, 0x89, 0x8f, 0x8d, 0x83, 0x81, 0x87, 0x85, 0xbb, 0xb9,
        0xbf, 0xbd, 0xb3, 0xb1, 0xb7, 0xb5, 0xab, 0xa9, 0xaf, 0xad,
        0xa3, 0xa1, 0xa7, 0xa5, 0xdb, 0xd9, 0xdf, 0xdd, 0xd3, 0xd1,
        0xd7, 0xd5, 0xcb, 0xc9, 0xcf, 0xcd, 0xc3, 0xc1, 0xc7, 0xc5,
        0xfb, 0xf9, 0xff, 0xfd, 0xf3, 0xf1, 0xf7, 0xf5, 0xeb, 0xe9,
        0xef, 0xed, 0xe3, 0xe1, 0xe7, 0xe5
    };

    const unsigned char byte_602494[256] =
    {
        0x00, 0x03, 0x06, 0x05, 0x0c, 0x0f, 0x0a, 0x09, 0x18, 0x1b,
        0x1e, 0x1d, 0x14, 0x17, 0x12, 0x11, 0x30, 0x33, 0x36, 0x35,
        0x3c, 0x3f, 0x3a, 0x39, 0x28, 0x2b, 0x2e, 0x2d, 0x24, 0x27,
        0x22, 0x21, 0x60, 0x63, 0x66, 0x65, 0x6c, 0x6f, 0x6a, 0x69,
        0x78, 0x7b, 0x7e, 0x7d, 0x74, 0x77, 0x72, 0x71, 0x50, 0x53,
        0x56, 0x55, 0x5c, 0x5f, 0x5a, 0x59, 0x48, 0x4b, 0x4e, 0x4d,
        0x44, 0x47, 0x42, 0x41, 0xc0, 0xc3, 0xc6, 0xc5, 0xcc, 0xcf,
        0xca, 0xc9, 0xd8, 0xdb, 0xde, 0xdd, 0xd4, 0xd7, 0xd2, 0xd1,
        0xf0, 0xf3, 0xf6, 0xf5, 0xfc, 0xff, 0xfa, 0xf9, 0xe8, 0xeb,
        0xee, 0xed, 0xe4, 0xe7, 0xe2, 0xe1, 0xa0, 0xa3, 0xa6, 0xa5,
        0xac, 0xaf, 0xaa, 0xa9, 0xb8, 0xbb, 0xbe, 0xbd, 0xb4, 0xb7,
        0xb2, 0xb1, 0x90, 0x93, 0x96, 0x95, 0x9c, 0x9f, 0x9a, 0x99,
        0x88, 0x8b, 0x8e, 0x8d, 0x84, 0x87, 0x82, 0x81, 0x9b, 0x98,
        0x9d, 0x9e, 0x97, 0x94, 0x91, 0x92, 0x83, 0x80, 0x85, 0x86,
        0x8f, 0x8c, 0x89, 0x8a, 0xab, 0xa8, 0xad, 0xae, 0xa7, 0xa4,
        0xa1, 0xa2, 0xb3, 0xb0, 0xb5, 0xb6, 0xbf, 0xbc, 0xb9, 0xba,
        0xfb, 0xf8, 0xfd, 0xfe, 0xf7, 0xf4, 0xf1, 0xf2, 0xe3, 0xe0,
        0xe5, 0xe6, 0xef, 0xec, 0xe9, 0xea, 0xcb, 0xc8, 0xcd, 0xce,
        0xc7, 0xc4, 0xc1, 0xc2, 0xd3, 0xd0, 0xd5, 0xd6, 0xdf, 0xdc,
        0xd9, 0xda, 0x5b, 0x58, 0x5d, 0x5e, 0x57, 0x54, 0x51, 0x52,
        0x43, 0x40, 0x45, 0x46, 0x4f, 0x4c, 0x49, 0x4a, 0x6b, 0x68,
        0x6d, 0x6e, 0x67, 0x64, 0x61, 0x62, 0x73, 0x70, 0x75, 0x76,
        0x7f, 0x7c, 0x79, 0x7a, 0x3b, 0x38, 0x3d, 0x3e, 0x37, 0x34,
        0x31, 0x32, 0x23, 0x20, 0x25, 0x26, 0x2f, 0x2c, 0x29, 0x2a,
        0x0b, 0x08, 0x0d, 0x0e, 0x07, 0x04, 0x01, 0x02, 0x13, 0x10,
        0x15, 0x16, 0x1f, 0x1c, 0x19, 0x1a
    };

    union
    {
        std::uint8_t output[4];
        std::uint32_t output_dword;
    };

    union
    {
        std::uint8_t input[4];
        std::uint32_t input_dword;
    };

    output_dword = encoded_dword;

    /*
        output[3] = byte_602494[input[0]] ^ input[1] ^ input[2] ^ byte_602394[input[3]]
        output[2] = byte_602394[input[2]] ^ input[0] ^ input[1] ^ byte_602494[input[3]]
        output[1] = byte_602394[input[1]] ^ input[0] ^ input[3] ^ byte_602494[input[2]]
        output[0] = byte_602394[input[0]] ^ input[2] ^ input[3] ^ byte_602494[input[1]]

        input[1] = output[3] ^ byte_602494[input[0]] ^ input[2] ^ byte_602394[input[3]]
        input[0] = output[2] ^ byte_602394[input[2]] ^ input[1] ^ byte_602494[input[3]]
        input[3] = output[1] ^ byte_602394[input[1]] ^ input[0] ^ byte_602494[input[2]]
        input[2] = output[0] ^ byte_602394[input[0]] ^ input[3] ^ byte_602494[input[1]]
    */

    bool found = false;

    for (std::uint16_t input1 = 0x00; !found && input1 <= 0xFF; input1++)
    {
        input[1] = static_cast<std::uint8_t>(input1);

        for (std::uint16_t input2 = 0x00; !found && input2 <= 0xFF; input2++)
        {
            input[2] = static_cast<std::uint8_t>(input2);

            for (std::uint16_t input3 = 0x00; !found && input3 <= 0xFF; input3++)
            {
                input[3] = static_cast<std::uint8_t>(input3);

                input[0] = output[2] ^ byte_602394[input[2]] ^ input[1] ^ byte_602494[input[3]];

                if (input[1] != (output[3] ^ byte_602494[input[0]] ^ input[2] ^ byte_602394[input[3]]))
                    continue;

                if (input[2] != (output[0] ^ byte_602394[input[0]] ^ input[3] ^ byte_602494[input[1]]))
                    continue;

                if (input[3] != (output[1] ^ byte_602394[input[1]] ^ input[0] ^ byte_602494[input[2]]))
                    continue;

                found = true;
            }
        }
    }

    encoded_dword = input_dword;

    if (!found)
        throw std::runtime_error("Failed to decode the value");
}
