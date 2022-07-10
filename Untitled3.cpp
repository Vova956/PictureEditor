#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

struct BITMAPFILEHEADER
{
    unsigned int    bfType;
    unsigned long   bfSize;
    unsigned int    bfReserved1;
    unsigned int    bfReserved2;
    unsigned long   bfOffBits;
};

struct BITMAPINFOHEADER
{
    unsigned int    biSize;
    int             biWidth;
    int             biHeight;
    unsigned short  biPlanes;
    unsigned short  biBitCount;
    unsigned int    biCompression;
    unsigned int    biSizeImage;
    int             biXPelsPerMeter;
    int             biYPelsPerMeter;
    unsigned int    biClrUsed;
    unsigned int    biClrImportant;
};

struct RGBQUAD
{
    int   rgbBlue;
    int   rgbGreen;
    int   rgbRed;
    int   rgbReserved;
};

unsigned short read_u16(FILE* file)
{
	unsigned char byte_1, byte_2;
	byte_1 = getc(file);
	byte_2 = getc(file);

	return ((byte_2 << 8) | byte_1);
}

unsigned int read_u32(FILE* file)
{
	unsigned char byte_1, byte_2, byte_3, byte_4;
	byte_1 = getc(file);
	byte_2 = getc(file);
	byte_3 = getc(file);
	byte_4 = getc(file);

	return ((((((byte_4 << 8) | byte_3) << 8) | byte_2) << 8) |byte_1);
}

int read_s32(FILE* file)
{
	char byte_1, byte_2, byte_3, byte_4;
	byte_1 = getc(file);
	byte_2 = getc(file);
	byte_3 = getc(file);
	byte_4 = getc(file);

	return ((((((byte_4 << 8) | byte_3) << 8) | byte_2) << 8) |byte_1);
}

void write_u16(FILE*  file, unsigned short value)
{
	unsigned char byte_1, byte_2;

	byte_2 = (value >> 8) & 0xff;
    byte_1 = value & 0xff;

    putc(byte_1, file);
    putc(byte_2, file);
}

void write_u32(FILE* file, unsigned short value)
{
	unsigned char byte_1, byte_2, byte_3, byte_4;

	byte_4 = (((value >> 8)>>8) >> 8) & 0xff;
	byte_3 = ((value >> 8)>>8) & 0xff;
	byte_2 = (value >> 8) & 0xff;
    byte_1 = value & 0xff;

    putc(byte_1, file);
    putc(byte_2, file);
    putc(byte_3, file);
    putc(byte_4, file);
}

void write_s32(FILE* file, short value)
{
	char byte_1, byte_2, byte_3, byte_4;

	byte_4 = (((value >> 8)>>8) >> 8) & 0xff;
	byte_3 = ((value >> 8)>>8) & 0xff;
	byte_2 = (value >> 8) & 0xff;
    byte_1 = value & 0xff;

    putc(byte_1, file);
    putc(byte_2, file);
    putc(byte_3, file);
    putc(byte_4, file);
}

int main(){

	char fileName[255], create[255];
	cout << "File name > "<< endl;
	cin >> fileName;
	cout << "Create name > "<< endl;
	cin >> create;

	FILE* file = fopen(fileName, "rb");
	FILE* created = fopen(create, "wb");

	BITMAPFILEHEADER header;

	header.bfType = read_u16(file);
	header.bfSize = read_u32(file);
	header.bfReserved1 = read_u16(file);
	header.bfReserved2 = read_u16(file);
	header.bfOffBits = read_u32(file);

	write_u16(created, header.bfType);
	write_u32(created, header.bfSize);
	write_u16(created, header.bfReserved1);
	write_u16(created, header.bfReserved2);
	write_u32(created, header.bfOffBits);

	BITMAPINFOHEADER info;

	info.biSize = read_u32(file);
	info.biWidth = read_s32(file);
	info.biHeight = read_s32(file);
	info.biPlanes = read_u16(file);
	info.biBitCount = read_u16(file);
	info.biCompression = read_u32(file);
	info.biSizeImage = read_u32(file);
	info.biXPelsPerMeter = read_s32(file);
	info.biYPelsPerMeter = read_s32(file);
	info.biClrUsed = read_u32(file);
	info.biClrImportant = read_u32(file);

	write_u32(created, info.biSize);
	write_s32(created, info.biWidth);
	write_s32(created, info.biHeight);
	write_u16(created, info.biPlanes);
	write_u16(created, info.biBitCount);
	write_u32(created, info.biCompression);
	write_u32(created, info.biSizeImage);
	write_s32(created, info.biXPelsPerMeter);
	write_s32(created, info.biYPelsPerMeter);
	write_u32(created, info.biClrUsed);
	write_u32(created, info.biClrImportant);

	RGBQUAD** rgb = new RGBQUAD*[info.biWidth];
    for(int i=0; i<info.biWidth; i++)
    {
    	rgb[i] = new RGBQUAD[info.biHeight];
	}

	for(int i = 0; i < info.biWidth; i++)
	{
		for(int j = 0; j < info.biHeight; j++)
		{
			rgb[i][j].rgbBlue = getc(file);
			rgb[i][j].rgbGreen = getc(file);
			rgb[i][j].rgbRed = getc(file);

			putc(rgb[i][j].rgbBlue, created);
			putc(rgb[i][j].rgbGreen, created);
			putc(rgb[i][j].rgbRed, created);
		}
	}


	cout << "Enter scale > ";
	int scale;
	cin >> scale;
	int n = sqrt(scale);
	FILE* scaled = fopen("scaled", "wb");

	write_u16(scaled, header.bfType);
	write_u32(scaled, header.bfSize);
	write_u16(scaled, header.bfReserved1);
	write_u16(scaled, header.bfReserved2);
	write_u32(scaled, header.bfOffBits);

	write_u32(scaled, info.biSize*scale*scale);
	write_s32(scaled, info.biWidth * scale);
	write_s32(scaled, info.biHeight * scale);
	write_u16(scaled, info.biPlanes);
	write_u16(scaled, info.biBitCount * scale*scale);
	write_u32(scaled, info.biCompression);
	write_u32(scaled, info.biSizeImage * scale*scale);
	write_s32(scaled, info.biXPelsPerMeter);
	write_s32(scaled, info.biYPelsPerMeter);
	write_u32(scaled, info.biClrUsed);
	write_u32(scaled, info.biClrImportant);

	for(int i = 0; i < info.biWidth; i++)
	{
		for(int j = 0; j < info.biHeight; j++)
		{
            for(int k = 0;k < scale*scale;k++)
            {
                putc(rgb[i][j].rgbBlue, created);
                putc(rgb[i][j].rgbGreen, created);
                putc(rgb[i][j].rgbRed, created);
            }
		}
	}


	FILE* reversed = fopen("reversed", "wb");

	write_u16(reversed, header.bfType);
	write_u32(reversed, header.bfSize);
	write_u16(reversed, header.bfReserved1);
	write_u16(reversed, header.bfReserved2);
	write_u32(reversed, header.bfOffBits);

	write_u32(reversed, info.biSize);
	write_s32(reversed, info.biWidth);
	write_s32(reversed, info.biHeight);
	write_u16(reversed, info.biPlanes);
	write_u16(reversed, info.biBitCount);
	write_u32(reversed, info.biCompression);
	write_u32(reversed, info.biSizeImage);
	write_s32(reversed, info.biXPelsPerMeter);
	write_s32(reversed, info.biYPelsPerMeter);
	write_u32(reversed, info.biClrUsed);
	write_u32(reversed, info.biClrImportant);


	for(int i = 0; i < info.biWidth; i++)
	{
		for(int j = 0; j < info.biHeight/2; j++)
		{
		    RGBQUAD buff = rgb[i][j];
		    rgb[i][j] = rgb[i][info.biHeight-j];
		    rgb[i][info.biHeight-j] = buff;
		}
	}

	for(int i = 0; i < info.biWidth; i++)
	{
		for(int j = 0; j < info.biHeight; j++)
		{
			putc(rgb[i][j].rgbBlue, reversed);
			putc(rgb[i][j].rgbGreen, reversed);
			putc(rgb[i][j].rgbRed, reversed);
		}
	}








	fclose(file);



	return 0;
}
