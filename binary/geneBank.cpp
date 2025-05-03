#include "geneBank.h"
using namespace std;

// =================================== PUBLIC FUNCTIONS =======================================================

GENE_BANK::GENE_BANK() {
    this->fileSize = 0;
    this->entryByte = sizeof(Sample);
}

GENE_BANK::~GENE_BANK() {
    cout << "Destructing ..." << endl;
}

void GENE_BANK::sort(Sample array[], int fileSize) {
    this->fileSize = fileSize; // store for later use
    if (fileSize != 0)
        this->p_timSort(array, fileSize);
    else
        throw MyException("ERROR: Empty file to sort!");
}

void GENE_BANK::indexSamples(Sample array[], int indexArray[]) {
    this->fileSize = 100000; // Make sure it's known

    for (int i = 0; i < 5; i++) indexArray[i] = -1;

    for (int i = 0; i < fileSize; i++) {
        int code = array[i].speciesCode;
        if (code >= 0 && code < 5 && indexArray[code] == -1) {
            indexArray[code] = i;
        }
    }
}

void GENE_BANK::displayResearcher(int speciesIndex, int offset, char* filename) {
    int actualIndex = speciesIndex + offset;

    fstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        throw MyException("ERROR: Cannot open file in displayResearcher.");
    }

    file.seekg(actualIndex * sizeof(Sample), ios::beg);
    Sample temp;
    file.read(reinterpret_cast<char*>(&temp), sizeof(Sample));

    if (file && temp.sampleID != -1) {
        cout << "Researcher: " << temp.researcher << endl;
    } else {
        cout << "Sample record doesn't exist! Can't display researcher name." << endl;
    }

    file.close();
}

bool GENE_BANK::searchSample(int speciesIndex, int offset, char* filename) {
    int actualIndex = speciesIndex + offset;

    fstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) return false;

    file.seekg(actualIndex * sizeof(Sample), ios::beg);
    Sample temp;
    file.read(reinterpret_cast<char*>(&temp), sizeof(Sample));
    file.close();

    return (file && temp.sampleID != -1);
}

void GENE_BANK::updateResearcher(int speciesIndex, int offset, char* newName, char* filename) {
    if (searchSample(speciesIndex, offset, filename)) {
        p_updateResearcher(speciesIndex, offset, newName, filename);
    } else {
        cout << "Sample record to be updated doesn't exist!" << endl;
    }
}

void GENE_BANK::deleteSample(int speciesIndex, int offset, char* filename) {
    if (searchSample(speciesIndex, offset, filename)) {
        p_deleteSample(speciesIndex, offset, filename);
    } else {
        cout << "Sample record to be deleted doesn't exist!" << endl;
    }
}

void GENE_BANK::printSampleRange(int speciesIndex, int start, int end, char* filename) {
    if (start > end) throw MyException("ERROR: start index is larger than end index!");
    p_printRange(speciesIndex, start, end, filename);
}

// =================================== PRIVATE FUNCTIONS =======================================================

void GENE_BANK::p_timSort(Sample array[], int fileSize) {
    const int RUN = 32;
    for (int i = 0; i < fileSize; i += RUN)
        p_insertionSort(array, i, min(i + RUN - 1, fileSize - 1));

    for (int size = RUN; size < fileSize; size *= 2) {
        for (int left = 0; left < fileSize; left += 2 * size) {
            int mid = left + size - 1;
            int right = min((left + 2 * size - 1), (fileSize - 1));
            if (mid < right)
                p_merge(array, left, mid, right);
        }
    }
}

void GENE_BANK::p_insertionSort(Sample array[], int beg, int end) {
    for (int i = beg + 1; i <= end; i++) {
        Sample key = array[i];
        int j = i - 1;
        while (j >= beg && array[j].speciesCode > key.speciesCode) {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = key;
    }
}

void GENE_BANK::p_merge(Sample array[], int beg, int mid, int end) {
    int len1 = mid - beg + 1, len2 = end - mid;
    Sample* left = new Sample[len1];
    Sample* right = new Sample[len2];

    for (int i = 0; i < len1; i++) left[i] = array[beg + i];
    for (int i = 0; i < len2; i++) right[i] = array[mid + 1 + i];

    int i = 0, j = 0, k = beg;
    while (i < len1 && j < len2) {
        if (left[i].speciesCode <= right[j].speciesCode)
            array[k++] = left[i++];
        else
            array[k++] = right[j++];
    }

    while (i < len1) array[k++] = left[i++];
    while (j < len2) array[k++] = right[j++];

    delete[] left;
    delete[] right;
}

void GENE_BANK::p_updateResearcher(int speciesIndex, int offset, char* newName, char* filename) {
    int actualIndex = speciesIndex + offset;

    fstream file(filename, ios::in | ios::out | ios::binary);
    if (!file.is_open()) {
        throw MyException("ERROR: Unable to open file for update.");
    }

    file.seekg(actualIndex * sizeof(Sample), ios::beg);
    Sample temp;
    file.read(reinterpret_cast<char*>(&temp), sizeof(Sample));

    if (file && temp.sampleID != -1) {
        strncpy(temp.researcher, newName, MAX_RESEARCHER_NAME);
        file.seekp(actualIndex * sizeof(Sample), ios::beg);
        file.write(reinterpret_cast<char*>(&temp), sizeof(Sample));
    }

    file.close();
}

void GENE_BANK::p_deleteSample(int speciesIndex, int offset, char* filename) {
    int actualIndex = speciesIndex + offset;

    fstream file(filename, ios::in | ios::out | ios::binary);
    if (!file.is_open()) {
        throw MyException("ERROR: Unable to open file for deletion.");
    }

    file.seekg(actualIndex * sizeof(Sample), ios::beg);
    Sample temp;
    file.read(reinterpret_cast<char*>(&temp), sizeof(Sample));

    if (file && temp.sampleID != -1) {
        strcpy(temp.researcher, "");
        file.seekp(actualIndex * sizeof(Sample), ios::beg);
        file.write(reinterpret_cast<char*>(&temp), sizeof(Sample));
    }

    file.close();
}

void GENE_BANK::p_printRange(int speciesIndex, int start, int end, char* filename) {
    fstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        throw MyException("ERROR: Unable to open file.");
    }

    for (int i = start; i <= end; ++i) {
        int actualIndex = speciesIndex + i;
        file.seekg(actualIndex * sizeof(Sample), ios::beg);
        Sample temp;
        file.read(reinterpret_cast<char*>(&temp), sizeof(Sample));
        if (file && temp.sampleID != -1) {
            cout << "SampleID: " << temp.sampleID
                 << ", Purity: " << temp.purityScore
                 << ", Researcher: " << temp.researcher << endl;
        }
    }

    file.close();
}
