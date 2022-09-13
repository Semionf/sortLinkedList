#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#pragma warning (disable:4996)

typedef struct Item {
	int num;
	struct Item* next;
	struct Item* prev;
}Item;

void add(int);
void randomNumbers();
void printList();
void replace(Item*);
void shakeSort();
void bubbleSort();
void releaseAll();
void Log(char[]);
void LogError(char[]);
void LogEvent(char[]);

Item* head, * tail;
int numOfNodes = 0;

int main()
{
	int start, end, diff;
	char str[100];
	time_t t;
	time(&t);
	struct tm* timeInfo;
	timeInfo = localtime(&t);
	start = timeInfo->tm_sec;
	int num;
	LogEvent("Program started.");
	randomNumbers();
	bubbleSort();
	printf("After sort: \n");
	printList();
	releaseAll();
	end = timeInfo->tm_sec;
	diff = start - end;
	sprintf(str, "Time between start and end is: %d", diff);
	LogEvent(str);
	LogEvent("End of program");
}

void add(int num)
{
	char str[100];
	Item* node = (Item*)malloc(sizeof(struct Item));
	if (!node)
	{
		LogError("Allocation Error");
		exit(1);
	}
	numOfNodes++;
	sprintf(str, "New node added: %d", num);
	LogEvent(str);
	node->next = node->prev = NULL;
	node->num = num;
	if (!head)
	{
		head = tail = node;
		return;
	}
	else {
		node->next = head;
		head->prev = node;
		head = node;
		return;
	}
}

void printList()
{
	LogEvent("List has printed.");
	Item* curr = head;
	char str[10];
	while (curr)
	{
		sprintf(str, "%d", curr->num);
		LogEvent(str);
		printf("%d ", curr->num);
		curr = curr->next;
	}
	printf("\n");
}

void replace(Item* rep)
{
	if (!rep) // no nodes.
	{
		LogEvent("Nothing to replace, list is empty");
		return;
	}
	if (!rep->next) // one node or last node.
	{
		LogEvent("Nothing to replace (one node or last node)");
		return;
	}
	char str[100];
	sprintf(str, "Replacing %d with %d", rep->num, rep->next->num);
	LogEvent(str);
	Item* temp1 = rep, *temp2 = rep->next;
	if (temp2->next && temp1->prev)  //2 middle nodes.
	{
		temp2->next->prev = temp1; 
		temp1->prev->next = temp2;
		temp1->next = temp2->next;
		temp2->prev = temp1->prev;
		temp1->prev = temp2;
		temp2->next = temp1;
		return;
	}
	else if(!temp1->prev && !temp2->next) {//only 2 nodes.
		temp2->next = temp1;
		temp1->prev = temp2;
		temp2->prev = NULL;
		temp1->next = NULL;
		head = temp2;
		tail = temp1;
		return;
	}
	else if (!temp2->next) // two last nodes.
	{
		temp2->prev = temp1->prev;
		temp1->prev->next = temp2;
		temp1->prev = temp2;
		temp1->next = NULL;
		temp2->next = temp1;
		tail = temp1;
		return;
	}
	else {// two first nodes.
		temp1->next = temp2->next;
		temp2->next->prev = temp1;
		temp1->prev = temp2;
		temp2->next = temp1;
		temp2->prev = NULL;
		head = temp2;
		return;
	}
	return;
}

void bubbleSort()
{
	Item* curr = head;
	if (!curr)
	{
		LogEvent("The list is empty.");
		return;
	}
	LogEvent("Sorting List");
	int i = 0;
	int j = 0;
	while (curr && i < numOfNodes)
	{
		curr = head;
		while (curr && j < numOfNodes - 1 - i)
		{
			if (curr->num > curr->next->num)
			{
				replace(curr);
				curr = curr->prev;
			}
			curr = curr->next;
			j++;
		}
		i++;
		j = 0;
	}
}

void shakeSort()
{
	Item* curr = head;
	if (!curr)
	{
		LogEvent("The list is empty.");
		return;
	}
	LogEvent("Sorting List");
	int i = 0;
	int j = 0;
	while(curr && i < numOfNodes)
	{
		while(curr && j < numOfNodes - 1 - i)
		{
			if (curr->num > curr->next->num)
			{
				replace(curr);
				curr = curr->prev;
			}
			curr = curr->next;
			j++;
		}
		curr = curr->prev;
		while (j > i + 1)
		{
			curr = curr->prev;
			if (curr->num > curr->next->num)// 1 3 2 4 6 5 7 9 8 10
			{
				replace(curr);
				curr = curr->prev;
			}
			j--;
		}
		i++;
		curr = curr->next;
	}
}
void randomNumbers()
{
	int num;
	srand(time(NULL));
	for (int i = 0; i < 10; i++)
	{
		num = rand();
		add(num);
	}
}

void releaseAll() 
{
	LogEvent("Release Event");
	Item* curr = head;
	while (head)
	{
		curr = head;
		head = head->next;
		free(curr);
	}
}

void Log(char str[])
{
	char Log[100];
	strcpy(Log, str);
	time_t t;
	time(&t);
	struct tm* timeInfo;
	timeInfo = localtime(&t);
	sprintf(Log, "%d %d %d: %02d:%02d:%02d- %s\n", timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday, timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec, str);
	FILE* f = fopen("myProg.log", "a");
	if (!f)
	{
		printf("File Error");
		exit(1);
	}
	fputs(Log, f);
	fclose(f);
}

void LogError(char str[])
{
	char Error[100];
	sprintf(Error, "Error - %s", str);
	Log(str);
}

void LogEvent(char str[])
{
	char Event[100];
	sprintf(Event, "Event - %s", str);
	Log(Event);
}