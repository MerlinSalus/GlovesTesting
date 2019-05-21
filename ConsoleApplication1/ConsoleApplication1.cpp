#include "Manus.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include "conio.h" // Note: not ISO C (DOS/Windows Only)



// Handle a manus error return code.
bool Error(manus_ret_t status, int& retval)
{
	printf("Error %d: %s\n", status, ManusErrorString(status));
	printf("Press q to exit, or r to resume.\n");

	char c = 0;

	while (c != 'q')
	{
		// Get keyboard input.
		if (_kbhit())
		{
			c = _getch();
		}

		// Resume.
		if (c == 'r')
		{
			return true;
		}
	}

	retval = EXIT_FAILURE;
	return false;
}

// Application entry point.
int main()
{
	// Initialise the Manus VR SDK.
	// Note: It takes a fraction of a second for the Manus SDK to start after this call.
	// Manus SDK functions will be available after that.
	printf("Initialising Manus VR SDK.\n");

	manus_session_t session;
	manus_ret_t status = ManusInit(&session);
	if (status != MANUS_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	// Main loop.
	bool run = true;
	int retval = EXIT_SUCCESS;

	while (run)
	{
		printf("Now taking input.\n");
		printf("Press g to start getting data: \n");
		printf("Press q to quit.\n");

		manus_ret_t status = MANUS_SUCCESS;
		char c = 0;
		bool run2 = true;

		// Run the input loop.
		while (run2)
		{
			// Get keyboard input.
			if (_kbhit())
			{
				c = _getch();
			}
			// Handle input.
			switch (c)
			{
			case 'g':
			{
				// Get glove data. Data is of the wrist rotation. Neg side is right, pos it left
				manus_hand_t lhand;

				manus_ret_t left = ManusGetHand(session, GLOVE_LEFT, &lhand);
				void sleep(unsigned milliseconds);
				{
					Sleep(50);
				}
				double temp = left == MANUS_SUCCESS ? lhand.raw.imu[0] : 0.0f;
				double newTemp = temp * 10;
				if (newTemp > 1)
				{
					printf("You are turning in a positive direction: [%f]\n", newTemp);
				}
				else if (newTemp < -1) {
					printf("You are turning in a negative direction: [%f]\n", newTemp);
				}
				else {
					printf("You are at resting position: [%f]\n", newTemp);
				}
				break;
			}
			case 'q':
				// Quit.
				run2 = false;
				break;
			default:
				break;
			} // switch(c)
			// Check if an error occurred.
			switch (status)
			{
			case MANUS_SUCCESS:
				break;
			case MANUS_DISCONNECTED:
				printf("A command failed because a device was disconnected.\n");
				status = MANUS_SUCCESS;
				break;
			default:
				run2 = false;
				break;
			} // switch(status)
		} // while (run)
		if (status != MANUS_SUCCESS)
		{
			run = Error(status, retval);
		}
		else
		{
			run = false;
		}

	}
	
	// Shut down the Manus VR SDK.
	printf("Shutting down Manus VR SDK.\n");

	status = ManusExit(session);
	if (status != MANUS_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	return retval;
}