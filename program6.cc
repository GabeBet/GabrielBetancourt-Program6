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
#include "cdk.h"

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 15
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
  binRecord.read((char *) myRecord, sizeof(BinaryFileRecord));

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

  string magic = "Magic: " + myHeader->magicNumber;
  setCDKMatrixCell(myMatrix, 1, 1, magic.c_str());

  string version = "Version: " + myHeader->versionNumber;
  setCDKMatrixCell(myMatrix, 1, 2, version.c_str());

  string records = "NumRecords: " + myHeader->numRecords;
  setCDKMatrixCell(myMatrix, 1, 3, records.c_str());

  int size = myHeader->numRecords;
  for(int i = 0; i < 4 || i < size; i++){
    string length = "strlen: " + myRecord->strLength;
    setCDKMatrixCell(myMatrix, 2+i, 1, length.c_str());
    setCDKMatrixCell(myMatrix, 2+i, 2, myRecord->stringBuffer);
  }

  drawCDKMatrix(myMatrix, true); // required 

  unsigned char x;
  cin >> x;

  endCDK();
  binHeader.close();
  binRecord.close();
}
