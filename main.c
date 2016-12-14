#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "utils/cmdline.h"
#include "driverlib/interrupt.h"
#include "driverlib/fpu.h"
#include "driverlib/timer.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/rom.h"

static char g_cInput[128];

#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

uint8_t ui8PinCH[8] = {0xC0,0x40,0x60,0x20,0x30,0x10,0x90,0x80};//Clockwise Half Step
uint8_t ui8PinACH[8] = {0x80,0x90,0x10,0x30,0x20,0x60,0x40,0xC0}; //AntiClockwise Half Step
uint8_t ui8PinCF[4] = {0xC0,0x60,0x30,0x90};//Clockwise Full Step
uint8_t ui8PinACF[4] = {0x90,0x30,0x60,0xC0};//Anti CLockwise Full Step
int seconds,no;
int yo=0;
uint32_t ui32speed1;
uint32_t ui32angle1;
uint32_t ui32loops1=0;
//Default-> full step clockwise
uint8_t check1; //full step or half  step
uint8_t check2; //Direction
uint8_t ui32flag=0;
uint32_t ui32loops=0, ui32k=0, ui32k1=0,ui32k2=0;
int i=0;
uint32_t ui32control2;
uint32_t angle;

volatile uint32_t value=0;
volatile uint32_t value1=0;
uint32_t status=0;
uint32_t status1=0;
uint32_t state=0;
uint32_t state1=0;
//uint32_t value=0;
//uint8_t prev=0;
//uint8_t curr=0;
//int curr=0;

void Timer0IntHandler(void)
{
	  TimerIntClear( TIMER0_BASE, TIMER_TIMA_TIMEOUT );
	  if(ui32flag==0)//default
	  {
	  if(check1==1 && check2==1)//Full Step Clockwise
	  {
		  if(i==4)
			  i=0;
      GPIOPinWrite( GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, ui8PinCF[i++] );
	  }
	  if(check1==1 && check2==0)//Full Step Anticlockwise
	  {
		  if(i==4)
			  i=0;
		  GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, ui8PinACF[i++] );
	  }
	  if(check1==0 && check2==1)//Half Step Clockwise
	  {
		  if(i==8)
			  i=0;
		  GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, ui8PinCH[i++] );
	  }
	  if(check1==0 && check2==0)//Half Step Anticlockwise
	  {
		  if(i==8)
			  i=0;
		  GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, ui8PinACH[i++] );
	  }
	  }

	  if(ui32flag==1)
	  {
		  if(check1==1 && check2==1)//Full Step Clockwise
		  {
			  if(i==4)
				  i=0;
	      GPIOPinWrite( GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, ui8PinCF[i++] );
		  }
		  if(check1==1 && check2==0)//Full Step Anticlockwise
		  {
			  if(i==4)
				  i=0;
			  GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, ui8PinACF[i++] );
		  }
		  if(check1==0 && check2==1)//Half Step Clockwise
		  {
			  if(i==8)
				  i=0;
			  GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, ui8PinCH[i++] );
		  }
		  if(check1==0 && check2==0)//Half Step Anticlockwise
		  {
			  if(i==8)
				  i=0;
			  GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, ui8PinACH[i++] );
		  }
		  ui32k++;
	      if(ui32k==ui32loops/2)
	    	  {
	    	//  ui32flag=1;
	    	  //ui32k=0;
	    	 if(check2==1)
	    	  On(ui32speed1,check1,0);
	    	 else
	    		 On(ui32speed1,check1,1);
	    	  }
	      if(ui32k==ui32loops)
	      {
	    	  ui32flag=0;
	    	  ui32k=0;
	    	  TimerDisable(TIMER0_BASE, TIMER_A);
	      }
	  }
	  if(ui32flag==2)
	  {
		  if(check1==1 && check2==1)//Full Step Clockwise
		  		  {
		  			  if(i==4)
		  				  i=0;
		  	      GPIOPinWrite( GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, ui8PinCF[i++] );
		  		  }
		  		  if(check1==1 && check2==0)//Full Step Anticlockwise
		  		  {
		  			  if(i==4)
		  				  i=0;
		  			  GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, ui8PinACF[i++] );
		  		  }
		  		  if(check1==0 && check2==1)//Half Step Clockwise
		  		  {
		  			  if(i==8)
		  				  i=0;
		  			  GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, ui8PinCH[i++] );
		  		  }
		  		  if(check1==0 && check2==0)//Half Step Anticlockwise
		  		  {
		  			  if(i==8)
		  				  i=0;
		  			  GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, ui8PinACH[i++] );
		  		  }
		  		  ui32k1++;
		  		if(ui32k1==ui32loops1)
		  		{
		  			//SysCtlDelay(7000000);
		  			//SysCtlDelay(7000000);
		  			yo++;
		  			if(yo<seconds)
		  			ui32k1=0;
		  			else if(yo==seconds || yo >seconds)
		  				{
		  				ui32flag=0;
		  				ui32k1=0;
		  				TimerDisable(TIMER0_BASE, TIMER_A);
		  				}
		  		}

	  }
	  if(ui32flag==3)
	  {
		  if(check1==1 && check2==1)//Full Step Clockwise
				  {
					  if(i==4)
						  i=0;
			      GPIOPinWrite( GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, ui8PinCF[i++] );
				  }
				  if(check1==1 && check2==0)//Full Step Anticlockwise
				  {
					  if(i==4)
						  i=0;
					  GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, ui8PinACF[i++] );
				  }
				  if(check1==0 && check2==1)//Half Step Clockwise
				  {
					  if(i==8)
						  i=0;
					  GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, ui8PinCH[i++] );
				  }
				  if(check1==0 && check2==0)//Half Step Anticlockwise
				  {
					  if(i==8)
						  i=0;
					  GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, ui8PinACH[i++] );
				  }
				  ui32k2++;
				 		  		if(ui32k2==ui32loops1)
				 		  		{

				 		  			//SysCtlDelay(7000000);
				 		  			//SysCtlDelay(7000000);
				 		  			yo++;//no of seconds
				 		  			if(yo<5)
				 		  			ui32k1=0;
				 		  			else if(yo==5 || yo >5)
				 		  				{
				 		  				ui32flag=0;
				 		  				ui32k1=0;
				 		  				TimerDisable(TIMER0_BASE, TIMER_A);
				 		  				GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_4);
				 		  				}
				 		  		}
	  }
}
void PortFIntHandler(){
	  status1 = GPIOIntStatus(GPIO_PORTF_BASE,true);

	  GPIOIntClear(GPIO_PORTF_BASE,status1);

	  if(status1==16)//!(status1 & GPIO_INT_PIN_6 == GPIO_INT_PIN_6)
	  {
	      //curr=15;//Then there was a Button pin interrupt
	      //uint8_t value=0;

	      value1= GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4);

	      if( value1==0)
	        state1^=GPIO_PIN_2;

	      GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2, state1);
	      ui32flag=3;//fucntion 3
	     if(ui32control2==45)
	     {
	    	 yo=0;
	      On(ui32speed1,check1, 0);
	     }//half step mode
	     else if(ui32control2==90)
	     {
	    	 yo=0;
	    	 On(ui32speed1,check1,1);
	     }
	    	 SysCtlDelay(7000000);
	  }
}
void control(uint32_t ui32control1)
{
	ui32control2= ui32control1;
	while(1)
	{
	GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4);
	GPIOIntRegister(GPIO_PORTC_BASE,PortFIntHandler);
	}

}
void On(uint32_t ui32speed, uint8_t ui8step, uint8_t ui8rev)
{
//TimerDisable(TIMER0_BASE,TIMER_A);
SysCtlDelay(200000);
ui32speed1=ui32speed;
GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2, GPIO_PIN_2);
	if(ui8step==1 && ui8rev==1)
	{
		check1=1;
	    check2=1;
	}
	if(ui8step==1 && ui8rev==0)
		{
			check1=1;
		    check2=0;
		}
	if(ui8step==0 && ui8rev==1)
		{
			check1=0;
		    check2=1;
		}
	if(ui8step==0 && ui8rev==0)
		{
			check1=0;
		    check2=0;
		}
	TimerLoadSet( TIMER0_BASE, TIMER_A, SysCtlClockGet()/(2*ui32speed1) );
    IntEnable( INT_TIMER0A );
    TimerIntEnable( TIMER0_BASE, TIMER_TIMA_TIMEOUT );
	IntMasterEnable();
	TimerEnable( TIMER0_BASE, TIMER_A );
	//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
}

void Off(void)
{
	ui32flag=0;
	TimerDisable(TIMER0_BASE, TIMER_A);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2,0);
	GPIOPinWrite(GPIO_PORTC_BASE,  GPIO_PIN_4| GPIO_PIN_5 |GPIO_PIN_6 |GPIO_PIN_7, 0);
}
void anticlockwise(void)
{
 	//TimerDisable(TIMER0_BASE, TIMER_A);
    SysCtlDelay(200000);
	check2=0;
    On(ui32speed1,check1,check2);
}
void clockwise(void)
{
 	//TimerDisable(TIMER0_BASE, TIMER_A);
    SysCtlDelay(200000);
	check2=1;
    On(ui32speed1,check1,check2);
}

void StepperSpeed(uint32_t ui32speed)
{
	TimerDisable(TIMER0_BASE, TIMER_A);
	SysCtlDelay(200000);
	ui32speed1=ui32speed;
	On(ui32speed1,check1,check2);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3,0);

}

void half(void)
{
	check1=0;
	On(ui32speed1,check1,check2);

}
void full(void)
{
	check1=1;
	On(ui32speed1,check1,check2);

}

void hey(void)
{
	ui32flag=2;
	angle=90;
	seconds=5;
	ui32loops1 = (angle*48)/360;
	On(ui32speed1,check1,check2);
}
void
ConfigureUART(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}
void StepperAngle(uint32_t ui32angle)
{
	ui32angle1=ui32angle;
	ui32flag=1;
	TimerDisable(TIMER0_BASE, TIMER_A);
	SysCtlDelay(200000);

	//Write a flag to check if half step or full step.
	if(check1==1)//full step
	{
		ui32loops = (ui32angle1*48*2)/360;//Double value so that it comes back to its original position
		On(ui32speed1,check1,check2);

	}
		if(check1==0)//half step
		{
			ui32loops = (ui32angle1*48*4)/360;
			On(ui32speed1,check1,check2);
		}

	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3,0);
}
int main(void) {

	uint32_t ui32Status;
    uint32_t ui32ResetCause;
    int32_t i32CommandStatus;

    FPUEnable();
    FPUStackingEnable();

    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |SYSCTL_OSC_MAIN);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
      GPIOPadConfigSet(GPIO_PORTF_BASE ,GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);//before it was 2ma
      GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_4, GPIO_RISING_EDGE);
  //  GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4| GPIO_PIN_5| GPIO_PIN_6| GPIO_PIN_7);

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    SysCtlPeripheralEnable( SYSCTL_PERIPH_TIMER0 );
    TimerConfigure( TIMER0_BASE, TIMER_CFG_PERIODIC );
    ConfigureUART();

    UARTprintf("Welcome to the Tiva C Series TM4C123G LaunchPad!\n");
    UARTprintf("Type 'help' for a list of commands\n");
    UARTprintf("> ");

    while(1)
        {

            UARTprintf("\n>");


            //
            // Peek to see if a full command is ready for processing
            //
            while((UARTgets =='\r') )//== -1
            {
                //
                // millisecond delay.  A SysCtlSleep() here would also be OK.
                //
                SysCtlDelay(SysCtlClockGet() / (1000 / 3));

                //
                // Check for change of mode and enter hibernate if requested.
                // all other mode changes handled in interrupt context.
                //

            }

            //
            // a '\r' was detected get the line of text from the user.
            //
            UARTgets(g_cInput,sizeof(g_cInput));

            //
            // Pass the line from the user to the command processor.
            // It will be parsed and valid commands executed.
            //
            i32CommandStatus = CmdLineProcess(g_cInput);

            //
            // Handle the case of bad command.
            //
            if(i32CommandStatus == CMDLINE_BAD_CMD)
            {
                UARTprintf("Bad command!\n");
            }

            //
            // Handle the case of too many arguments.
            //
            else if(i32CommandStatus == CMDLINE_TOO_MANY_ARGS)
            {
                UARTprintf("Too many arguments for command processor!\n");
            }
        }


	return 0;
}
