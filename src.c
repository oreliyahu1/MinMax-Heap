#include <stdlib.h>
#include <stdio.h>

#define RIGHT(i) (2*i+1) //define function right
#define PARENT(i) (i-1)/2 //define function parent
#define LEFT(i) (2*i+2) //define function left

typedef enum { MAXHeap, MINHeap}HeapType; //type of heap
typedef int EKey; //key

typedef struct Element
{
	EKey key;
	int twin;//twin index
}Element, *PElement;

typedef struct UpHeap
{
	PElement MinHeap;//min heap
	PElement MaxHeap;//max heap
	int heapSizes;//elements on heps
	int maxSize;//max size of heaps
}UpHeap, *PUpHeap;

int menuChoose(int aFunc); //user menu choose
void Error_msg(char *func, char *msg, PUpHeap heap);//error message and release
EKey * getKeysFromUser(int *EkeysEleSize, int *EkeysEleIn);//get settings of arr from user

void releaseUpHeaps(PUpHeap heap);//free dynamic heaps
void swapElement(PElement heap1, PElement heap2, int i1, int i2);//swap between elements and indexs

void Build_Heap(HeapType ty, UpHeap heap);//build (typeheap) heap
void Heapify_Down(HeapType ty, UpHeap heap, int i); //heapify down of (typeheap) heap
void Heapify_Up(HeapType ty, UpHeap heap, int i);//heapify up of (typeheap) heap
void Del(HeapType ty, PUpHeap heap, int i);//del element i from (typeheap) heap

void Init(EKey* elmts, UpHeap heap);//initialization heaps from elmts arr
EKey Find_Max(UpHeap heap);//return max element from heaps
EKey Find_Min(UpHeap heap);//return min element from heaps
void Insert(PUpHeap heap, EKey key);//insert new key to heaps
EKey Del_Max(PUpHeap heap);//del max element from heaps
EKey Del_Min(PUpHeap heap);//del min element from heaps

void main()
{
	int userSelect, initFlag = 0; //User selection, initialization flag
	UpHeap heap = { NULL,NULL,0,0 };//max and min heap
	EKey * elements, temp;//arr elements from user
	fprintf(stdout, "	Data Structures HW2.Q9	Present by:	Or Eliyahu ID:x		Sami Odeh ID: x\n\
********************************************************************************************************************\n\n");

	elements = getKeysFromUser(&heap.maxSize, &heap.heapSizes);
	heap.MaxHeap = (PElement)malloc(sizeof(Element)*heap.maxSize);
	heap.MinHeap = (PElement)malloc(sizeof(Element)*heap.maxSize);
	if ((heap.MaxHeap == NULL) || (heap.MinHeap == NULL))
	{
		free(elements);
		Error_msg("main", "cannot create heaps", &heap);
	}

	do
	{
		userSelect = menuChoose(initFlag); //show menu and get select
		if ((!initFlag) && (userSelect))
		{
			if (userSelect == 1)
			{
				Init(elements, heap);//create max/min heaps
				free(elements);//no user arr needed
				elements = NULL;
				initFlag = 1;
			}
			else
				fprintf(stdout, "\nError: Cannot run any function until run '1'\n");
		}
		else
		{
			switch (userSelect) {
			case 1:
				fprintf(stdout, "\nError: Cannot run initialization function again.\n");
				break;
			case 2:
				fprintf(stdout, "\nEnter key to insert: ");
				fscanf(stdin, "%d", &temp);
				Insert(&heap, temp);
				break;
			case 3:
				if (temp = Find_Min(heap))
					fprintf(stdout, "the min key is: %d \n", temp);
				break;
			case 4:
				if (temp = Find_Max(heap))
					fprintf(stdout, "the max key is: %d \n", temp);
				break;
			case 5:
				if (temp = Del_Min(&heap))
					fprintf(stdout, "the min key that deleted is: %d \n", temp);
				break;
			case 6:
				if (temp = Del_Max(&heap))
					fprintf(stdout, "the max key that deleted is: %d \n", temp);
				break;
			case 0:
				fprintf(stdout, "Bye Bye.\n");
				break;
			default:
				fprintf(stdout, "Sorry, I dont find this function\n");
				break;
			}
		}
	} while (userSelect);

	releaseUpHeaps(&heap);//free max/min heap
	if (elements)//if arr is alive1
		free(elements);
}

//menuChoose
//get aFunc(int) - 1/0 if '1' function was runed
//show to user the options of the program
//return (int) the user select option
int menuChoose(int aFunc)
{
	int userSelect;
	fprintf(stdout, "\n\n\
========================================================MENU========================================================\
\nOptions	| Description							| Status\
\n-------------------------------------------------------------------------------------------------------------------\
\n+ (1)	| Init(S)							| %s\
\n+ (2)	| Insert(x)							| %s\
\n+ (3)	| Find-min							| %s\
\n+ (4)	| Find-max							| %s\
\n+ (5)	| Del-min							| %s\
\n+ (6)	| Del-max							| %s\
\n+ (0)	| End Program							| Available\
\n-------------------------------------------------------------------------------------------------------------------\
%s\
\n\n\nEnter your choice: \
",
(!aFunc) ? "Initialization Function" : "Blocked",
(!aFunc) ? "Temporarily Unavailable" : "Available",
(!aFunc) ? "Temporarily Unavailable" : "Available",
(!aFunc) ? "Temporarily Unavailable" : "Available",
(!aFunc) ? "Temporarily Unavailable" : "Available",
(!aFunc) ? "Temporarily Unavailable" : "Available",
(!aFunc) ? "\n	!!!Reminder: Before start any function you need to initialization program with function '1'!!!	" : "");

	fscanf(stdin, "%d", &userSelect);
	return userSelect;
}

//Error_msg
//get func(char*) - function name error, msg(char*) - message to user, heap(PUpHeap) - heaps release
//release heap and show to user message & run exit(1)
void Error_msg(char *func, char *msg, PUpHeap heap)
{
	if (heap)
		releaseUpHeaps(heap);
	fprintf(stdout, "Error(%s): %s\n", func, msg);
	exit(1);
}

//getKeysFromUser
//get EkeysEleSize(int*) - size of arr, EkeysEleIn(int*) - count of cells filled
//ask user arr size and keys
//return (EKey *) dynamic arr of keys
EKey * getKeysFromUser(int *EkeysEleSize, int *EkeysEleIn)
{
	int i, arrSize;
	EKey* arr = NULL;
	fprintf(stdout, "Enter size of arr (size > 1): ");
	fscanf(stdin, "%d", &arrSize);
	if (arrSize < 2)
	{
		fprintf(stdout, "\nError: please enter positive number bigger than 1 \n");
		return getKeysFromUser(EkeysEleSize, EkeysEleIn); //ask again
	}
	*EkeysEleSize = arrSize; //update arr size
	if (arr = (EKey*)malloc(sizeof(EKey)*arrSize))
	{
		do
		{
			printf("Enter the number of elements to enter arr: ");
			fscanf(stdin, "%d", &i);
			if ((i < 1) || (i > arrSize))
			{
				fprintf(stdout, "\nError: please enter positive number (0 < i <= %d) \n", arrSize);
				i = 0;
			}
		} while (i == 0);
		*EkeysEleIn = i;//update count of celss filled
		printf("Enter the elements keys(Separated by Enter): ");
		for (i = 0; i < *EkeysEleIn; i++)
			scanf("%d", &arr[i]);
	}
	else
		Error_msg("getKeysFromUser", "cannot create arr", NULL);
	return arr;
}

//releaseUpHeaps
//get heap(PUpHeap) - max/min heaps
//free all dynamic heaps
void releaseUpHeaps(PUpHeap heap)
{
	if (heap->MaxHeap) //Prevents a situation free null
		free(heap->MaxHeap);
	if (heap->MinHeap) //Prevents a situation free null
		free(heap->MinHeap);
	heap->MaxHeap = heap->MinHeap = NULL;
	heap->MaxHeap = heap->heapSizes = 0;
}

//swapElement
//get heap1(PElement) - swap elements arr, heap2(PElement) - update twin index, i1(int)/i2(int)-swap index
//swap between index i1 and i2 and update twin index on heap2
void swapElement(PElement heap1, PElement heap2, int i1, int i2)
{
	Element tempEl;
	int tempIn;
	//exchange A[i1] <-> A[i2]
	tempEl = heap1[i1];
	heap1[i1] = heap1[i2];
	heap1[i2] = tempEl;

	//exchange twin index
	tempIn = heap2[heap1[i1].twin].twin;
	heap2[heap1[i1].twin].twin = heap2[heap1[i2].twin].twin;
	heap2[heap1[i2].twin].twin = tempIn;
}

//Build_Heap
//get ty(HeapType) - heap type, heap(UpHeap) - max/min heaps
//build (ty) heap
void Build_Heap(HeapType ty, UpHeap heap)
{
	int i;
	for (i = heap.heapSizes / 2; i >= 0; i--)
		Heapify_Down(ty, heap, i);//fix heap
}

//Heapify_Down
//get ty(HeapType) - heap type, heap(UpHeap) - max/min heaps
//make Heapify_Down to (ty) heap and updated the twin index of parallel heap
void Heapify_Down(HeapType ty, UpHeap heap, int i)
{
	PElement workHeap = (ty == MAXHeap) ? heap.MaxHeap : heap.MinHeap;//main heap to work
	PElement twinHeap = (ty == MAXHeap) ? heap.MinHeap : heap.MaxHeap;//twin heap to update
	int l = LEFT(i), r = RIGHT(i), largsmall = i;
	switch (ty) {
	case MAXHeap:
		if ((l < heap.heapSizes) && (workHeap[l].key > workHeap[i].key))
			largsmall = l;
		if ((r < heap.heapSizes) && (workHeap[r].key > workHeap[largsmall].key))
			largsmall = r;
		break;
	case MINHeap:
		if ((l < heap.heapSizes) && (workHeap[l].key <= workHeap[i].key))
			largsmall = l;
		if ((r < heap.heapSizes) && (workHeap[r].key <= workHeap[largsmall].key))
			largsmall = r;
		break;
	default:
		break;
	}

	if (largsmall != i)
	{
		swapElement(workHeap, twinHeap, i, largsmall);//swap elements
		Heapify_Down(ty, heap, largsmall);//Continue fix heap
	}
}

//Heapify_Up
//get ty(HeapType) - heap type, heap(UpHeap) - max/min heaps
//make Heapify_Up to (ty) heap and updated the twin index of parallel heap
void Heapify_Up(HeapType ty, UpHeap heap, int i)
{
	PElement workHeap = (ty == MAXHeap) ? heap.MaxHeap : heap.MinHeap;//main heap to work
	PElement twinHeap = (ty == MAXHeap) ? heap.MinHeap : heap.MaxHeap;//twin heap to update
	int p = PARENT(i), largsmall = i;

	switch (ty) {
	case MAXHeap:
		if ((i > 0) && (workHeap[i].key > workHeap[p].key))
			largsmall = p;
		break;
	case MINHeap:
		if ((i > 0) && (workHeap[i].key <= workHeap[p].key))
			largsmall = p;
		break;
	default:
		break;
	}

	if (largsmall != i)
	{
		swapElement(workHeap, twinHeap, i, largsmall);//swap elements
		Heapify_Up(ty, heap, largsmall);//Continue fix heap
	}
}

//Del
//get ty(HeapType) - heap type, heap(UpHeap) - max/min heaps, i(int) - index to del from (ty) heap
//del index (i) from (ty) heap and updated the twin index of parallel heap
void Del(HeapType ty, PUpHeap heap, int i)
{
	PElement workHeap = (ty == MAXHeap) ? heap->MaxHeap : heap->MinHeap;//main heap to work
	PElement twinHeap = (ty == MAXHeap) ? heap->MinHeap : heap->MaxHeap;//twin heap to update
	if (heap->heapSizes > 0)
	{
		twinHeap[workHeap[heap->heapSizes - 1].twin].twin = i;
		workHeap[i] = workHeap[heap->heapSizes - 1];
		Heapify_Down(ty, *heap, i);//fix heap
		Heapify_Up(ty, *heap, i);//fix heap
	}
	else
		fprintf(stdout, "\nError: heap underflow \n");
}

//Find_Max
//get heap(UpHeap) - max/min heaps
//return (EKey) the max value from heaps
EKey Find_Max(UpHeap heap)
{
	if (heap.heapSizes)
		return heap.MaxHeap[0].key;
	else
		fprintf(stdout, "\nError: heap underflow \n");
	return NULL;
}

//Find_Max
//get heap(UpHeap) - max/min heaps
//return (EKey) the min value from heaps
EKey Find_Min(UpHeap heap)
{
	if (heap.heapSizes)
		return heap.MinHeap[0].key;
	else
		fprintf(stdout, "\nError: heap underflow \n");
	return NULL;
}

//Insert
//get heap(PUpHeap) - max/min heaps, key(EKey) - key to insert
//insert key to heaps
void Insert(PUpHeap heap, EKey key)
{
	Element in;
	if (heap->heapSizes < heap->maxSize)
	{
		heap->MaxHeap[heap->heapSizes].key = heap->MinHeap[heap->heapSizes].key = key;
		heap->MaxHeap[heap->heapSizes].twin = heap->MinHeap[heap->heapSizes].twin = heap->heapSizes;
		heap->heapSizes++;
		Heapify_Up(MINHeap, *heap, heap->heapSizes - 1);//fix min heap
		Heapify_Up(MAXHeap, *heap, heap->heapSizes - 1);//fix max heap
	}
	else
		fprintf(stdout, "\nError: heap overflow \n");
}

//Init
//get heap(UpHeap) - max/min heaps, elmts(EKey*) - arr of keys
//initialization heaps from elmnts arr
void Init(EKey* elmts, UpHeap heap)
{
	int i;
	for (i = 0; i < heap.heapSizes; i++)
	{
		heap.MaxHeap[i].key = elmts[i];
		heap.MaxHeap[i].twin = i;
		heap.MinHeap[i].key = elmts[i];
		heap.MinHeap[i].twin = i;
	}
	Build_Heap(MAXHeap, heap, Heapify_Down);
	Build_Heap(MINHeap, heap, Heapify_Down);
}

//Del_Max
//get heap(PUpHeap) - max/min heaps
//del max value from heaps
//return deleted value (max key) (EKey)
EKey Del_Max(PUpHeap heap)
{
	EKey max = NULL;
	if (heap->heapSizes)
	{
		max = Find_Max(*heap); //return max key
		Del(MINHeap, heap, heap->MaxHeap[0].twin); //del twin index of max key from min heap
		Del(MAXHeap, heap, 0); //del max value
		heap->heapSizes--;
	}
	else
		fprintf(stdout, "\nError: heap underflow \n");
	return max;
}

//Del_Min
//get heap(PUpHeap) - max/min heaps
//del min value from heaps
//return deleted value (min key) (EKey)
EKey Del_Min(PUpHeap heap)
{
	EKey min = NULL;
	if (heap->heapSizes)
	{
		min = Find_Min(*heap); //return min key
		Del(MAXHeap, heap, heap->MinHeap[0].twin); //del twin index of min key from max heap
		Del(MINHeap, heap, 0); //del min value
		heap->heapSizes--;
	}
	else
		fprintf(stdout, "\nError: heap underflow \n");
	return min;
}