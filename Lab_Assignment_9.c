#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h> // Include necessary headers

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order; 
};

// Fill out this structure
#define HASH_SIZE 100 // Choose a suitable hash table size
struct HashType
{
    struct RecordType* records;
};

// Compute the hash function
int hash(int x)
{
    return x % HASH_SIZE; // Simple modulo hashing
}

// Parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// Prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// Display records in the hash structure
// Skip the indices which are free
// The output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    int i, j;
    for (i = 0; i < hashSz; ++i)
    {
        if (pHashArray[i].records != NULL) // Check if index is occupied
        {
            printf("Index %d -> ", i);
            struct RecordType* records = pHashArray[i].records;
            for (j = 0; records[j].id != 0; ++j)
            {
                printf("%d %c %d -> ", records[j].id, records[j].name, records[j].order);
            }
            printf("\n");
        }
    }
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input_lab_9.txt", &pRecords); // Change input filename here
    printRecords(pRecords, recordSz);
    
    // Your hash implementation
    struct HashType hashArray[HASH_SIZE] = {0}; // Initialize hash table

  
    for (int i = 0; i < recordSz; ++i)
    {
        int index = hash(pRecords[i].id); 
        struct RecordType* records = hashArray[index].records;
        int j;
        for (j = 0; records != NULL && records[j].id != 0; ++j); 
        if (records == NULL)
        {
            records = (struct RecordType*)malloc(sizeof(struct RecordType) * (j + 2)); 
            hashArray[index].records = records;
        }
        records[j] = pRecords[i]; 
        records[j + 1].id = 0; 
    }

    
    displayRecordsInHash(hashArray, HASH_SIZE);

    
    for (int i = 0; i < HASH_SIZE; ++i)
    {
        if (hashArray[i].records != NULL)
        {
            free(hashArray[i].records);
        }
    }

   
    free(pRecords);

    return 0;
}

