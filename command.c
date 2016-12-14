/*
 * command.c
 *
 *  Created on: Nov 23, 2016
 *      Author: Kirthi
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "utils/ustdlib.h"
#include "utils/uartstdio.h"
#include "utils/cmdline.h"
#include "command.h"

    uint32_t ui32speed;
	uint32_t ui32angle;

tCmdLineEntry g_psCmdTable[] =
{
    {"help",     CMD_help,      " : Display list of commands" },
    {"on",      CMD_on,       " : Switch on the stepper motor"},
    {"off",     CMD_off,      " : Switch off the stepper motor"},
    {"anti",   CMD_anti, " : Anticlockwise"},
    {"clock",   CMD_clock, " : Clockwise"},
    {"half",      CMD_half,       " : Half step"},
	{"full",      CMD_full,       " : Full step"},
	{"speed",     CMD_speed,      " : Set Speed"},
	{"angle",     CMD_angle,      " : Set Angle"},
	{"90",       CMD_90,          " : Set for 90 every 5 seconds"},
	{"control",  CMD_control,     " : Set Angle"},
    { 0, 0, 0 }
};

int
CMD_control(int argc, char **argv)
{
	uint32_t ui32control1;
	if(argc == 2)
	    {
	        //
	        // Extract the required color from the command line parameter.
	        //
	        ui32control1 = ustrtoul(argv[1], 0, 10);
	    control(ui32control1);
	    }
return(0);
}
int
CMD_help(int argc, char **argv)
{
    int32_t i32Index;

    (void)argc;
    (void)argv;

    //
    // Start at the beginning of the command table
    //
    i32Index = 0;

    //
    // Get to the start of a clean line on the serial output.
    //
    UARTprintf("\nAvailable Commands\n------------------\n\n");

    //
    // Display strings until we run out of them.
    //
    while(g_psCmdTable[i32Index].pcCmd)
    {
      UARTprintf("%17s %s\n", g_psCmdTable[i32Index].pcCmd,
                 g_psCmdTable[i32Index].pcHelp);
      i32Index++;
    }

    //
    // Leave a blank line after the help strings.
    //
    UARTprintf("\n");

    return (0);
}

int
CMD_on(int argc, char **argv)
{
    //
    // Keep the compiler happy.
    //
    (void)argc;
    (void)argv;

    //
    // Enter hibernate state.
    //
   On(5,1,1);

    return (0);
}

//*****************************************************************************
//
// Command: rand
//
// Starts the automatic light sequence immediately.
//
//*****************************************************************************
int
CMD_off(int argc, char **argv)
{
    //
    // Keep the compiler happy.
    //
    (void)argc;
    (void)argv;

    //
    // Turn on automatic mode.
    //
    Off();

    return (0);
}

//*****************************************************************************
//
// Command: intensity
//
// Takes a single argument that is between zero and one hundred. The argument
// must be an integer.  This is interpreted as the percentage of maximum
// brightness with which to display the current color.
//
//*****************************************************************************
int
CMD_anti(int argc, char **argv)//Delay between blinking
{
   // uint32_t ui32delay; //ui32Intensity
    (void)argc;
    (void)argv;
    //
    // This command requires one parameter.
    //
   // if(argc == 2)
    //{
        //
        // Extract the intensity from the command line parameter.
        //
      //  ui32delay = ustrtoul(argv[1], 0, 10); //10 for decimal input

        //
        // Convert the value to a fractional floating point value.
        //


        //
        // Set the intensity of the RGB LED.
        //
        anticlockwise();


    return(0);

}
int
CMD_clock(int argc, char **argv)//Delay between blinking
{
   // uint32_t ui32delay; //ui32Intensity
    (void)argc;
    (void)argv;
    //
    // This command requires one parameter.
    //
   // if(argc == 2)
    //{
        //
        // Extract the intensity from the command line parameter.
        //
      //  ui32delay = ustrtoul(argv[1], 0, 10); //10 for decimal input

        //
        // Convert the value to a fractional floating point value.
        //


        //
        // Set the intensity of the RGB LED.
        //
        clockwise();


    return(0);

}

//*****************************************************************************
//
// Command: rgb
//
// Takes a single argument that is a string between 000000 and FFFFFF.
// This is the HTML color code that should be used to set the RGB LED color.
//
// http://www.w3schools.com/html/html_colors.asp
//
//*****************************************************************************
int CMD_half(int argc, char **argv)
{
	half();
	return(0);
}
int CMD_full(int argc, char **argv)
{
	full();
	return(0);
}
int
CMD_speed(int argc, char **argv)
{

    //
    // This command requires one parameter.
    //
    if(argc == 2)
    {
        //
        // Extract the required color from the command line parameter.
        //
        ui32speed = ustrtoul(argv[1], 0, 10);//Before it was 16 for FFFFFFFF -i.e Hexadecimal type input

        //
        // Decompose teh color into red, green and blue components.
        //
        //
        // Turn off automatic mode and set the desired LED color.
        //

        StepperSpeed(ui32speed);
    }

    return (0);

}
int CMD_angle(int argc, char **argv)
{
    if(argc == 2)
    {
        //
        // Extract the required color from the command line parameter.
        //
        ui32angle = ustrtoul(argv[1], 0, 10);//Before it was 16 for FFFFFFFF -i.e Hexadecimal type input

        //
        // Decompose teh color into red, green and blue components.
        //
        //
        // Turn off automatic mode and set the desired LED color.
        //

        StepperAngle(ui32angle);
    }

    return (0);
}
int CMD_90(int argc, char **argv)
{
	hey();
	return(0);
}
