#include "geneBank.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "ERROR!!\nUsage: ./a fileName" << endl;
        return 1;
    }

    cout << "Starting Gene Bank program with file: " << argv[1] << endl;

    GENE_BANK mySample;
    fstream inputBinary, sortedBinary;
    int entryByte = sizeof(Sample), fileSize = 0;
    Sample* smpl;
    Sample temp;
    int indexArray[5];
    char newName[20], fileStr[25];

    // Step 1: Load the binary file and count records
    inputBinary.open(argv[1], ios::in | ios::binary);
    inputBinary.seekg(0, ios::end);
    fileSize = inputBinary.tellg() / entryByte;
    inputBinary.seekg(0, ios::beg);

    cout << "Loaded " << fileSize << " sample records from binary file." << endl;

    smpl = new Sample[fileSize];
    for (int i = 0; i < fileSize; ++i)
        inputBinary.read((char*)&smpl[i], sizeof(Sample));
    inputBinary.close();

    // Step 2: Sort and write to new file
    mySample.sort(smpl, fileSize);
    string str = "Sorted_";
    str += argv[1];
    strcpy(fileStr, str.c_str());

    sortedBinary.open(fileStr, ios::out | ios::binary);
    for (int i = 0; i < fileSize; ++i)
        sortedBinary.write((char*)&smpl[i], sizeof(Sample));
    sortedBinary.close();

    // Step 3: Indexing
    mySample.fileSize = fileSize;  // Add this line before indexing

    mySample.indexSamples(smpl, indexArray);

    cout << "\nSpecies Indexes:\n";
    for (int i = 0; i < 5; ++i)
        cout << "Species " << i << ": starts at index " << indexArray[i] << endl;

    // Step 4: Sizes
    int size[5];
    for (int i = 0; i < 4; ++i)
        size[i] = indexArray[i+1] - indexArray[i];
    size[4] = fileSize - indexArray[4];

    // Step 5: Printing samples
    cout << "\n-----------Printing the 53rd Sample from each species code\n";
    for (int i = 0; i < 5; ++i)
        mySample.displayResearcher(indexArray[i], 53, fileStr);

    cout << "\n-----------Printing the 102nd Sample from each species code\n";
    for (int i = 0; i < 5; ++i)
        mySample.displayResearcher(indexArray[i], 102, fileStr);

    cout << "\n---Updating 102nd Sample to 'Carol' and printing\n";
    strcpy(newName, "Carol");
    for (int i = 0; i < 5; ++i) {
        mySample.updateResearcher(indexArray[i], 102, newName, fileStr);
        mySample.displayResearcher(indexArray[i], 102, fileStr);
    }

    cout << "\n-----------Printing the 13th Sample from each species code\n";
    for (int i = 0; i < 5; ++i)
        mySample.displayResearcher(indexArray[i], 13, fileStr);

    cout << "\n---Deleting 13th Sample and printing again\n";
    for (int i = 0; i < 5; ++i) {
        mySample.deleteSample(indexArray[i], 13, fileStr);
        mySample.displayResearcher(indexArray[i], 13, fileStr);
    }

    cout << "\n---Last 20 records from each species code\n";
    for (int i = 0; i < 5; ++i) {
        int start = indexArray[i] + size[i] - 20;
        int end = indexArray[i] + size[i] - 1;
        mySample.printSampleRange(indexArray[i], start, end, fileStr);
        cout << endl;
    }

    delete[] smpl;
    return 0;
}
