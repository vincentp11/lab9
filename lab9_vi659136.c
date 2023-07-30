// Vincent Phillips
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

struct Node
{
    struct RecordType data;
    struct Node *next;
};

// HashType with array of pointers to Node
struct HashType
{
    struct Node **array;
    int size;
};

// Compute the hash function
int hash(int x, int size)
{
    return x % size;
}

// parses input file to an integer array
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

// prints the records
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

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray)
{
    int i;
    struct Node *currentNode;

    for (i = 0; i < pHashArray->size; ++i)
    {
        currentNode = pHashArray->array[i]; 
        if (currentNode != NULL)
        {
            printf("Index %d -> ", i);
            while (currentNode != NULL) // Traverse the linked list
            {
                printf("id: %d, name: %c, order: %d -> \n", 
                       currentNode->data.id, currentNode->data.name, currentNode->data.order);
                currentNode = currentNode->next;
            }
            printf("NULL\n");
        }
    }
}

void insertIntoHashTable(struct HashType* hashTable, struct RecordType record)
{
    int index = hash(record.id, hashTable->size);

    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = record;
    newNode->next = hashTable->array[index];
    hashTable->array[index] = newNode;
}

struct HashType* createHashTable(int size)
{
    struct HashType* hashTable = (struct HashType*)malloc(sizeof(struct HashType));
    hashTable->size = size;
    hashTable->array = (struct Node**)malloc(sizeof(struct Node*) * size);
    
    for (int i = 0; i < size; ++i)
    {
        hashTable->array[i] = NULL;
    }

    return hashTable;
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);

    struct HashType* hashTable = createHashTable(recordSz);

    for (int i = 0; i < recordSz; ++i)
    {
        insertIntoHashTable(hashTable, pRecords[i]);
    }

    displayRecordsInHash(hashTable);

    free(pRecords);
    for (int i = 0; i < hashTable->size; ++i)
    {
        struct Node* currentNode = hashTable->array[i];
        struct Node* temp;
        while (currentNode != NULL)
        {
            temp = currentNode;
            currentNode = currentNode->next;
            free(temp);
        }
    }
    free(hashTable->array);
    free(hashTable);

    return 0;
}
