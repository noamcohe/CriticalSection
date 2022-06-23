#include "pch.h"


#define NUM_OF_THREADS 5
#define MAX_RUNING 1000000


CRITICAL_SECTION firstLock;
CRITICAL_SECTION secondLock;


DWORD WINAPI Threadproc(LPVOID Iparam)
{
	// wait until the right side will open:
	EnterCriticalSection(&firstLock);

	// Check if the left side is open or close.
	// Return TRUE if it's open, FALSE otherwise:
	BOOL success = TryEnterCriticalSection(&secondLock);

	// If the right side and the left side:
	if(success)
	{
		// Critical Section is here:

		// The serial number of the current thread:
		PINT serialNumber = (PINT)Iparam;

		// The amount of times a philosopher eats:
		INT count = 0;

		// Run a million times:
		for (INT i = 0; i < MAX_RUNING; i++)
		{
			count++;
		}

		printf("Philosopher %d ate a million times\n", *serialNumber);

		// Open lock:
		LeaveCriticalSection(&secondLock);
	}

	// If success is FALSE:
	LeaveCriticalSection(&firstLock);

	return 1;
}


int main()
{
	// Measuring time:
	clock_t initialTime = clock();

	// Create locks:
	InitializeCriticalSection(&firstLock);
	InitializeCriticalSection(&secondLock);

	// Create array of threads:
	HANDLE hThreadArray[NUM_OF_THREADS];

	// Serial number for each thread:
	INT serialNumber[NUM_OF_THREADS] = { 1, 2, 3, 4, 5 };

	// Get Thread identifier from each of threads that was created:
	DWORD dwThreadIdArray[NUM_OF_THREADS];

	for (INT i = 0; i < NUM_OF_THREADS; i++)
	{
		// Create new thread:
		hThreadArray[i] = CreateThread(
			NULL,						// Default security attributes
			0,							// Default stack size
			Threadproc,					// Thread function name
			&serialNumber[i],			// Thread param
			0,							// Default creation flags
			&dwThreadIdArray[i]			// Return thread identifier
		);

		// Check the return value for success.
		// If CreateThread fails, terminate execution. This will automatically clean up threads and memory:
		if (hThreadArray[i] == NULL)
		{
			ExitProcess(3);
		}
	}

	WaitForMultipleObjects(NUM_OF_THREADS, hThreadArray, TRUE, INFINITE);

	for (INT i = 0; i < NUM_OF_THREADS; i++)
	{
		// Close all threads:
		CloseHandle(hThreadArray[i]);
	}

	// Delete locks:
	DeleteCriticalSection(&firstLock);
	DeleteCriticalSection(&secondLock);

	clock_t currentTime = clock();
	FLOAT deltaSec = FLOAT(currentTime - initialTime) / CLOCKS_PER_SEC;
	printf("run time: %f sec\n", deltaSec);

	return 0;
}