/*
 * Gabriel Betancourt
 * geb160130@utdallas.edu
 * CS 3377.502
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdint.h>
#include <string>
#include <inttypes.h>
#include "cdk.h"

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 20
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;

class BinaryFileHeader
{
public:
 
  uint32_t magicNumber;
  uint32_t versionNumber;
  uint64_t numRecords;
};

const int maxRecordStringLength = 25;

class BinaryFileRecord
{
public:
  uint8_t strLength;
  char stringBuffer[maxRecordStringLength];
};

int main()
{

  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix; //CDK Screen Matrix

  const char 		*columnTitles[MATRIX_WIDTH+1] = {"C0", "a", "b", "c"};
  const char 		*rowTitles[MATRIX_HEIGHT+1] = {"R0", "a", "b", "c", "d", "e"};
  int		boxWidths[MATRIX_WIDTH+1] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[MATRIX_WIDTH+1] = {vMIXED, vMIXED, vMIXED, vMIXED};


  BinaryFileHeader *myHeader = new BinaryFileHeader();
  ifstream binHeader ("cs3377.bin", ios::in | ios::binary);
  binHeader.read((char *) myHeader, sizeof(BinaryFileHeader));

  BinaryFileRecord *myRecord = new BinaryFileRecord();
  ifstream binRecord ("cs3377.bin", ios::in | ios::binary);
  binRecord.read((char *) myRecord, sizeof(BinaryFileHeader));

  window = initscr();
  cdkscreen = initCDKScreen(window);

  initCDKColor();

  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  // display matrix
  drawCDKMatrix(myMatrix, true);

  char magic[10];
  sprintf(magic, "Magic: 0x%8X", myHeader->magicNumber);
  setCDKMatrixCell(myMatrix, 1, 1, magic);

  char version[10];
  sprintf(version, "Version: %u", myHeader->versionNumber);
  setCDKMatrixCell(myMatrix, 1, 2, version);

  char records[10];
  sprintf(records, "NumRecords: %lu", myHeader->numRecords);
  setCDKMatrixCell(myMatrix, 1, 3, records);

  int size = myHeader->numRecords;
  for(int i = 0; i < 4 || i < size; i++){
    binRecord.read((char *) myRecord, sizeof(BinaryFileRecord));
    char length[10];
    sprintf(length, "strlen: %d", myRecord->strLength);
    setCDKMatrixCell(myMatrix, 2+i, 1, length);

    char str[10];
    sprintf(str, "%s", myRecord->stringBuffer);
    setCDKMatrixCell(myMatrix, 2+i, 2, str);
    }

  drawCDKMatrix(myMatrix, true); // required 

  unsigned char x;
  cin >> x;

  endCDK();
  binHeader.close();
  binRecord.close();
}
