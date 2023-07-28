#include <stdio.h>
#include <stdlib.h>


// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};


struct Node
{
	struct RecordType record;
	struct Node* next;
};


// Fill out this structure
struct HashType
{
	struct Node* head;
};


// Compute the hash function
int hash(int x, char name, int hashSz, int att)
{
    int hashVal = 0;
    int nameVal = name; // Convert char to int

    // Combine the ID and name to generate the hash value
    hashVal = (x + nameVal + att) % hashSz;

    return hashVal;
}


void insert(struct Node** head, struct RecordType record)
{
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if(newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    newNode->record = record;
    newNode->next = *head;
    *head = newNode;
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

	if(inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if(*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for(i = 0; i < dataSz; ++i)
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
	for(i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}


// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for(i=0;i<hashSz;++i)
	{
		// if index is occupied with any records, print all
		printf("Index %d -> ", i);
        struct Node* curr = pHashArray[i].head;
        while(curr != NULL)
        {
            printf("%d %c %d -> ", curr->record.id, curr->record.name, curr->record.order);
            curr = curr->next;
        }
        printf("\n");
    }
	
}


int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;
	int hashSz = 31;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation

	struct HashType* hashTable = (struct HashType*)malloc(sizeof(struct HashType) * hashSz);
    
	
	for(int i = 0; i < hashSz; ++i)
    {
        hashTable[i].head = NULL; // Initialize the head to NULL
		
    }
	
	for(int i = 0; i < recordSz; ++i)
    {
        int att = 0;
        int index = hash(pRecords[i].id, pRecords[i].name, hashSz, att);

        while(hashTable[index].head != NULL)
        {
            att++;
            index = hash(pRecords[i].id, pRecords[i].name, hashSz, att);
        }

        insert(&(hashTable[index].head), pRecords[i]);
    }
	
    // Display the records in the hash table
    displayRecordsInHash(hashTable, hashSz);

    free(pRecords);
    // Free the memory used by the linked lists in the hash table
    for(int i = 0; i < hashSz; ++i)
    {
        struct Node* current = hashTable[i].head;
        while(current != NULL)
        {
            struct Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(hashTable);
    return 0;
	
}