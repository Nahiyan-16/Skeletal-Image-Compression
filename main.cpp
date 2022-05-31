#include <iostream>
#include <fstream> 
#include <string>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;

class ImageCompression {
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int newMinVal;
    int newMaxVal;
    int** ZFAry;
    int** skeletonAry;
    int nAry[9]; //Neighborhood
  
   public: ImageCompression(ifstream& inFile, int** sAry, int** zAry, int numR, int numC, int minV, int maxV, int newMin, int newMax) {
      ZFAry = zAry;
      skeletonAry = sAry;
      numRows = numR;
      numCols = numC;
      minVal = minV;
      maxVal = maxV;
      newMinVal = newMin;
      newMaxVal = newMax;
    }

    public: void setZero(int** ary) {
      for(int i = 0; i < numRows+2; i++) {
        for(int j = 0; j < numCols+2; j++) {
          ary[i][j] = 0;
        }
      }
    }

    public: void loadImage(ifstream& imgFile, int** ary) {
      int img;
      for(int i = 1; i <= numRows; i++){
        for(int j = 1; j <= numCols; j++){
          imgFile >> img;
          ary[i][j] = img;
        }
      }
    }
    
    public: void loadSkeleton(ifstream& skeletonFile, int** ZFAry) {
      int value, r, c, val;
      if(skeletonFile>>value) numRows = value;
      if(skeletonFile>>value) numCols = value;
      if(skeletonFile>>value) minVal = value;
      if(skeletonFile>>value) maxVal = value;
      while(skeletonFile>>r>>c>>val) {
        ZFAry[r][c] = val;
      }
    }
    
    public: void compute8Distance(int** ZFAry, ofstream& outFile) {
      firstPass8Distance(ZFAry);
      outFile << "First Pass 8 Distance:" << "\n";
      reformatPrettyPrint(ZFAry, outFile);
      secondPass8Distance(ZFAry);
      outFile << "Second Pass 8 Distance:" << "\n";
      reformatPrettyPrint(ZFAry, outFile);
    }
    public: void firstPass8Distance(int** ZFAry) {
      string checkPass = "First";
      int count = 0;
      for(int i = 1; i <= numRows; i++) {
        for(int j = 1; j <= numCols; j++) {
          if(ZFAry[i][j] > 0) {
            ZFAry[i][j] = findMin(checkPass, i, j);
          }
          if(count == 0) {
            newMinVal = ZFAry[i][j];
            newMaxVal = ZFAry[i][j];
            count++;
          }
          else {
            if(newMinVal > ZFAry[i][j]) newMinVal = ZFAry[i][j];
            if(newMaxVal > ZFAry[i][j]) newMaxVal = ZFAry[i][j];
          }
        }
      }
    }

    public: void secondPass8Distance(int** ZFAry) {
      string checkPass = "Second";
      int count = 0;
      for(int i = numRows; i >= 1; i--) {
        for(int j = numCols; j >= 1; j--) {
          if(ZFAry[i][j] > 0) {
            ZFAry[i][j] = findMin(checkPass, i, j);
          }
          if(count == 0) {
            newMinVal = ZFAry[i][j];
            newMaxVal = ZFAry[i][j];
            count++;
          }
          else {
            if(newMinVal > ZFAry[i][j]) newMinVal = ZFAry[i][j];
            if(newMaxVal > ZFAry[i][j]) newMaxVal = ZFAry[i][j];
          }
        }
      }
    }
    
    public: int findMin(string c, int i, int j) {
    string f = "First";
      if(c == f) {
        nAry[0] = ZFAry[i-1][j-1] + 1;
        nAry[1] = ZFAry[i-1][j] + 1;
        nAry[2] = ZFAry[i-1][j+1] + 1;
        nAry[3] = ZFAry[i][j-1] + 1;
        int min = nAry[0];
        for(int x = 1; x < 4; x++) {
          if(min > nAry[x]) min = nAry[x];
        }
        return min;
      }
      else {
        nAry[4] = ZFAry[i][j+1] + 1;
        nAry[5] = ZFAry[i+1][j-1] + 1;
        nAry[6] = ZFAry[i+1][j] + 1;
        nAry[7] = ZFAry[i+1][j+1] + 1;
        nAry[8] = ZFAry[i][j];
        int min = nAry[4];
        for(int y = 4; y <= 8; y++) {
          if(min > nAry[y]) min = nAry[y];
        }
        return min;
      }
    }

    public: void imageCompression(int** ZFAry, int** skeletonAry, ofstream& skeletonFile, ofstream& outFile) {
      computeLocalMaxima(ZFAry, skeletonAry);
      outFile << "Skeleton Extraction:" << "\n";
      reformatPrettyPrint(skeletonAry, outFile);
      extractSkeleton(skeletonAry, skeletonFile);
    }

    public: bool isLocalMaxima(int** ZFAry, int i, int j) {
      if(ZFAry[i][j] >= ZFAry[i-1][j-1]
         && ZFAry[i][j] >= ZFAry[i-1][j]
         && ZFAry[i][j] >= ZFAry[i-1][j+1]
         && ZFAry[i][j] >= ZFAry[i][j-1]
         && ZFAry[i][j] >= ZFAry[i][j+1]
         && ZFAry[i][j] >= ZFAry[i+1][j-1]
         && ZFAry[i][j] >= ZFAry[i+1][j]
         && ZFAry[i][j] >= ZFAry[i+1][j+1]) {
          return 1;
      }
      else 
          return 0;
    }

   public: void computeLocalMaxima(int** ZFAry, int** skeletonAry) {
      for(int i = 1; i <= numRows; i++) {
        for(int j = 1; j <= numCols; j++) {
          if(isLocalMaxima(ZFAry, i, j) == 1){
            skeletonAry[i][j] = ZFAry[i][j];
          }
          else {
            skeletonAry[i][j] = 0;
          }
        }
      }
    }

    public: void extractSkeleton(int** skeletonAry, ofstream& skeletonFile) {
      skeletonFile << numRows << " " << numCols << " " << newMinVal << " " << newMaxVal << "\n";
      for(int i = 1; i <= numRows; i++) {
        for(int j = 1; j <= numCols; j++) {
          if(skeletonAry[i][j] > 0) {
            skeletonFile << i << " " << j << " " << skeletonAry[i][j] << "\n";
          }
        }
      }
    }

    public: void imageDecompression(int** ZFAry, ifstream& skeletonFile, ofstream& outFile) {
      setZero(ZFAry);
      loadSkeleton(skeletonFile,ZFAry);
      firstPassExpansion(ZFAry);
      outFile << "First Pass Skeleton Expansion:\n";
      reformatPrettyPrint(ZFAry, outFile);
      secondPassExpansion(ZFAry);
      outFile << "Second Pass Skeleton Expansion:\n";
      reformatPrettyPrint(ZFAry, outFile);
    }

    public: int findMax(string c, int i, int j) {
      string f = "First";
        if(c == f) {
        nAry[0] = ZFAry[i-1][j-1] - 1;
        nAry[1] = ZFAry[i-1][j] - 1;
        nAry[2] = ZFAry[i-1][j+1] - 1;
        nAry[3] = ZFAry[i][j-1] - 1;
        nAry[4] = ZFAry[i][j+1] - 1;
        nAry[5] = ZFAry[i+1][j-1] - 1;
        nAry[6] = ZFAry[i+1][j] - 1;
        nAry[7] = ZFAry[i+1][j+1] - 1;
        int max = nAry[0];
        for(int x = 1; x < 8; x++) {
          if(max < nAry[x]) max = nAry[x];
        }
        return max;
      }
      else {
        nAry[0] = ZFAry[i-1][j-1];
        nAry[1] = ZFAry[i-1][j];
        nAry[2] = ZFAry[i-1][j+1];
        nAry[3] = ZFAry[i][j-1];
        nAry[4] = ZFAry[i][j+1];
        nAry[5] = ZFAry[i+1][j-1];
        nAry[6] = ZFAry[i+1][j];
        nAry[7] = ZFAry[i+1][j+1];
        int max = nAry[0];
        for(int y = 1; y < 8; y++) {
          if(max < nAry[y]) max = nAry[y];
        }
        return max;
      }
    }

    public: void firstPassExpansion(int** ZFAry) {
      string checkPass = "First";
      for(int i = 1; i <= numRows; i++) {
        for(int j = 1; j <= numCols; j++) {
          if(ZFAry[i][j] == 0) {
            if(ZFAry[i][j] < findMax(checkPass, i, j)){
              ZFAry[i][j] = findMax(checkPass, i, j);
            }
          }
        }
      }
    }

    public: void secondPassExpansion(int** ZFAry) {
      string checkPass = "Second";
      for(int i = numRows; i >= 1; i--) {
        for(int j = numCols; j >= 1; j--) {
          if(ZFAry[i][j] < findMax(checkPass, i, j)){
            ZFAry[i][j] = findMax(checkPass, i, j)-1;
          }
        }
      }
    }

    public: void threshold(int** ZFAry, ofstream& decompressFile) {
      decompressFile << numRows << " " << numCols << " " << newMinVal << " " << newMaxVal << "\n";
      for(int i = 1; i <= numRows; i++) {
        for(int j = 1; j <= numCols; j++) {
          if(ZFAry[i][j] >= 1) {
            decompressFile << "1 ";
          }
          else {
            decompressFile << "0 ";
          }
        }
        decompressFile << "\n";
      }
    }

    public: void reformatPrettyPrint(int** ZFAry, ofstream& outFile) {
      for(int i = 1; i <= numRows; i++) {
        for(int j = 1; j <= numCols; j++) {
          if(ZFAry[i][j] == 0) {
            outFile << ". ";
          }
          else {
            outFile << ZFAry[i][j] << " ";
          }
        }
        outFile << "\n";
        }
      }
    }
;
int main(int argc, char* argv[]) {
  ifstream inFile;
  ofstream outFile1, skeletonFile, decompressedFile;

  inFile.open(argv[1]);
  outFile1.open(argv[2]);

  int numRows, numCols, minVal, maxVal, newMinVal, newMaxVal;
  string inHeader;
  getline(inFile, inHeader);
  string inHeaderAry[4];
  stringstream is(inHeader);
  for(int i = 0; i < 4 && is.good(); i++){
        is >> inHeaderAry[i];
      }
  numRows = stoi(inHeaderAry[0]);
  numCols = stoi(inHeaderAry[1]);
  minVal = stoi(inHeaderAry[2]);
  maxVal = stoi(inHeaderAry[3]);

  int** ZFAry;
  int** skeletonAry;
  ZFAry = new int*[numRows+2];
  skeletonAry = new int*[numRows+2];
  for(int i = 0; i < numRows+2; i++) {
    ZFAry[i] = new int[numCols+2];
    skeletonAry[i] = new int[numCols+2];
  }

  string skeletonFileName = argv[1];
  string decompressedFileName = argv[1];
  skeletonFile.open(skeletonFileName + "_skeleton.txt");
  decompressedFile.open(decompressedFileName + "_decompressed.txt");

  ImageCompression comp(inFile, skeletonAry, ZFAry, numRows, numCols, minVal, maxVal, newMinVal, newMaxVal);
  
  comp.setZero(ZFAry);
  comp.setZero(skeletonAry);
  comp.loadImage(inFile, ZFAry);
  comp.compute8Distance(ZFAry, outFile1);
  comp.imageCompression(ZFAry, skeletonAry, skeletonFile, outFile1);
  skeletonFile.close();
  ifstream skeletonFile2;
  comp.setZero(ZFAry);
  skeletonFile2.open(skeletonFileName + "_skeleton.txt");
  comp.imageDecompression(ZFAry, skeletonFile2, outFile1);
  comp.threshold(ZFAry, decompressedFile);

  inFile.close();
  outFile1.close();
  skeletonFile2.close();
  decompressedFile.close();
}