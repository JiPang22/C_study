#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#pragma pack(push, 1)

typedef struct {
	unsigned short filetype;
	unsigned int filesize;
	unsigned short Reserved1;
	unsigned short Reserved2;
	unsigned int pixeldata;
} BMPFileHeader;

typedef struct {
	unsigned int size;
	unsigned int width;
	unsigned int height;
	unsigned short planes;
	unsigned short bitcount;
	unsigned int compression;
	unsigned int imagesize;
	int xPelsPerMeter;
	int yPelsPerMeter;
	unsigned int clrUsed;
	unsigned int clrImportant;
} BMPInfoHeader;

typedef struct {
	unsigned char blue;
	unsigned char green;
	unsigned char red;
} PIXEL;

#pragma pack(pop)

int fun2() {
	FILE* file;
	errno_t err = fopen_s(&file, "./image.bmp", "rb");

	if (err != 0 || file == NULL) {
		printf("파일 열기 실패\n");
		return 1;
	}

	BMPFileHeader fileHeader;
	BMPInfoHeader infoHeader;

	fread(&fileHeader, sizeof(BMPFileHeader), 1, file);
	fread(&infoHeader, sizeof(BMPInfoHeader), 1, file);

	if (fileHeader.filetype != 0x4D42) { // 'BM'
		printf("BMP 파일이 아닙니다.\n");
		fclose(file);
		return 1;
	}

	printf("가로: %d, 세로: %d, 비트 수: %d\n", infoHeader.width, infoHeader.height, infoHeader.bitcount);

	int width = infoHeader.width;
	int height = infoHeader.height;
	int rowSize = ((infoHeader.bitcount * width + 31) / 32) * 4;

	unsigned char* row = (unsigned char*)malloc(rowSize);
	fseek(file, fileHeader.pixeldata, SEEK_SET);

	for (int y = 0; y < height; y++) {
		fread(row, 1, rowSize, file);
		// row에는 한 줄의 픽셀 데이터가 들어있음
		// 예: row[0], row[1], row[2]는 (B, G, R)
	}

	free(row);
	fclose(file);
	return 0;
}
