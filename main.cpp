//////////////////////////////////////////////////////////////////////////
//  Copyright (c) <2023> <Brian Tuffy>                                  //
//                                                                      //
//  Permission is hereby granted, free of charge, to any person         //
//  obtaining a copy of this software and associated documentation      //
//  files (the "Software"), to deal in the Software without             //
//  restriction, including without limitation the rights to use, copy,  //
//  modify, merge, publish, distribute, sublicense, and/or sell copies  //
//  of the Software, and to permit persons to whom the Software is      //
//  furnished to do so, subject to the following conditions:            //
//                                                                      //
//  The above copyright notice and this permission notice shall be      //
//  included in all copies or substantial portions of the Software.     //
//                                                                      //
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,     //
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES     //
//  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND            //
//  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT         //
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,        //
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING        //
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR       //
//  OTHER DEALINGS IN THE SOFTWARE.                                     //
//////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <stdio.h>
#include "SFile.h"
#include <fstream>
#include "ctime"

using namespace std;
vector<short int> buffer;
DWORD start = 0;
DWORD res = 0;

//Windows functions for pressing keyboard buttons
bool Keypress(char &Key)
{
    INPUT_RECORD Event;
    DWORD NumberOfEvents, EventsRead, EventCounter;

    GetNumberOfConsoleInputEvents
    (GetStdHandle(STD_INPUT_HANDLE), &NumberOfEvents);

    if (NumberOfEvents == 0)
        return false;

    for (EventCounter = 0; EventCounter < NumberOfEvents; EventCounter++)
    {
        PeekConsoleInput(GetStdHandle
        (STD_INPUT_HANDLE), &Event, 1, &EventsRead);
        
        if ((Event.EventType == KEY_EVENT) && ((Event.Event.KeyEvent.bKeyDown)))
        {
            ReadConsoleInput
            (GetStdHandle(STD_INPUT_HANDLE), &Event, 1, &EventsRead);
            
            Key = Event.Event.KeyEvent.wVirtualKeyCode;
            if (!(FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE))))
                exit(0);
            return true;
        }
        else
            ReadConsoleInput
            (GetStdHandle(STD_INPUT_HANDLE), &Event, 1, &EventsRead);
    }

    return false;
}
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
int main() {
    srand (time(NULL)); //for Awesome Points
    
   int ComPort=1;        //Communications port. ComPort=8 for GC computer. 3 for my PC.
   SFile sf;   
   fstream of( "output.txt", fstream::out );
   ofstream log;
    
  // log.open ("Xenon_Log.txt", ios::out | ios::app);
   
   			  
   			   
   //Temperary .txt file to read bytes (called output.txt).
    
       if (!sf.Open(ComPort, 9600))       //Open COM port with 9600 bitrate
          {
          cerr << "Failed to open COM port. \nSerial COM port "<<ComPort<<
          " should be connected to the Lamp."
          " Error code = " << GetLastError() << endl;
          system("pause");
          return 1;
          }

	while(1==1)                              //Repeat program indefinitely.
	{ 
 

                
                
        int LampRunMin=180;                  //3 h default run time
        cout<<"Tuffy Timer 2000. Automatically start and stop xenon lamp 2. \n"
        "Times are loged in Xenon_Log.txt.\n \nEnter minutes to run lamp?"<<endl; cin>>LampRunMin;
        int LampRunSec=LampRunMin*60;

        int DelayMin=660;            //11 h default delay (18:00 to 05:00)
        cout<<"Enter minutes to delay start of lamp?"<<endl; cin>>DelayMin;
        int DelaySec=DelayMin*60;
        system("pause");


        for(time_t DelayEndTime = time(NULL)+DelaySec;time(NULL)<DelayEndTime;)
            { 
                time_t MinRemain = DelayEndTime - time(NULL);
                tm *ltm3 = localtime(&MinRemain);
                system("CLS");
                cout<<"waiting "<<ltm3->tm_hour-1<<":"<<ltm3->tm_min <<
                ":"<<ltm3->tm_sec << " to start lamp"<< endl;

                char key = 0;
                 if (Keypress(key))    
                    {                 //If you press Esc, the program quits.
                        if (key == VK_ESCAPE) 
                            {
                                PostQuitMessage(0);
                                Sleep(0);
                                return 1;
                            }
                    }
                    Sleep(500);      
                                    
            } //End of FOR loop for delay timer before lamp start.
            

       
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

        sf.SetReadTimeOut(3000);                                                //important if there is no bytes to read
        //If no byte to read from the port, the program times out (msec).

		char cmd1[] = "START\r";           //Lamp ignition                  \\Only Start and Stop Commands are in use mow
		char cmd2[] = "AMPS?\r";           //Current of lamp
		char cmd3[] = "VOLTS?\r";          //Voltage of lamp
		char cmd4[] = "WATTS?\r";          //Power of lamp e.g. 300 W
		char cmd5[] = "LAMP HRS?\r";       //Lamp running hours
		char cmd6[] = "RSTHRS\r";          //Reset lamp running hours

		DWORD written;
		sf.Write(cmd1, 6, written);
        //Lamp START command with # of characters of command (6 for START\r).

		time_t StartTime = time(NULL); 
        //Timer Start time in seconds since 1970.

                log.open ("Xenon_Log.txt", ios::out | ios::app);
                log<<"Start Code: \n";
                
                Sleep(4100);
                BYTE b; //Bytes sent from the xenon lamp e.g. error codes
                     while (sf.ReadByte(b)) 
				{ 
                      log << b;                                                    //At least the start code works ;(
                } 
				log<<" \n";
		
		for (time_t EndTime = StartTime + LampRunSec; time(NULL) < EndTime; ) 
		{
			time_t Difference=EndTime-time(NULL);
			tm *ltm = localtime(&Difference);
			system("CLS"); 
			{cout<<ltm->tm_hour-1<< ":";               //Hours
			cout<<ltm->tm_min << ":";                  //Mins
			cout<<ltm->tm_sec<<" remaining"<<endl;}    //Seconds
   
			char key = 0; 
			if (Keypress(key))
			{
				if (key == VK_BACK) 
                // If you press the Backspace key during the timer, ...
                //... it skips the timer and runs the command.
				{
					break;            
				}
            
				if (key == VK_ESCAPE)
                //If you press the Esc key, ...
                //... the program quits without running the command.
				{
					PostQuitMessage(0);
					Sleep(0);
					return 1;
				}
				
                //Printing the timer countdown:
                tm *ltm2 = localtime(&StartTime);
                
                //Pressing any other key pauses the timer readout.
                cout<<"The timer is still running but the program must "
                "be in countdown mode to turn off the lamp. "<<endl;
                
                //Countdown started at hh:mm:ss.
                cout<<"Timer started at "<<ltm2->tm_hour<< ":"; 
                cout<<ltm2->tm_min << ":";
                cout<<ltm2->tm_sec<<":\nReading codes from the Lamp...";
                     
                //Reading bytes from lamp power supply.           //Reading Bytes after Lamp Start
                //e.g. "ESR01" = operation successful.
				while (sf.ReadByte(b))   
				{ 
                      cout << b;
                } 
				cout<<"\n";
			
				system("pause");
				
			}
			Sleep(1000);
		}
		
		
		   
		   
		//Sends the stop command to turn off the lamp.
		char cmd0[] = "STOP\r"; 
		sf.Write(cmd0, 5, written); 
		
        time_t StopTime = time(NULL);                    //Real Lamp Running ON time
        time_t LampOnTime = StopTime-StartTime;  

        log<<" Stop Code:";
 Sleep(4500);       
		char key = 0;
		while (sf.ReadByte(b)) 
		{
			if (Keypress(key))
			{
				if (key == VK_ESCAPE)
				{
					PostQuitMessage(0);
					Sleep(0);
					return 1;
				}
            system("pause");
			}
        
        cout << b;          //Bytes sent from the power supply
        log << b;
		}
		cout<<" \n";
		log<<" \n";
		
        //Countdown started at hh:mm:ss.
        tm *ltm2 = localtime(&StartTime);                                       //Need to do my processing for ltm2 (program and log) before ltm3
        cout<<"Timer started at "<<ltm2->tm_hour<< ":";
        cout<<ltm2->tm_min << ":";
        cout<<ltm2->tm_sec<<": \n";
        
        //Log output of parameters
        double Points=((double)rand()/RAND_MAX)*100;                            //stupid random number for the Awesome points
        
        
        log <<"Lamp2 Start: "<<ltm2->tm_mday << "/";                             //Writing Lamp Start Date to Log
        log <<ltm2->tm_mon+1 << "/";
        log <<ltm2->tm_year +1900<< "_";  
        log <<ltm2->tm_hour<< ":";                                              //Writing Lamp Start Time to Log
        log <<ltm2->tm_min << ":";
        log <<ltm2->tm_sec<<" " ;
        
    //    log <<"Stop code: "<<b<<" .";                                         //Writing error codes to Log
       log <<" You gained "<<(int)Points<<" Awesome points! \n";                 //points are magical!
        
	//	log<<"Lamp on for "<<LampOnTime<< " seconds. \n";          
	//	cout<<"Lamp was on for "<<LampOnTime<< " seconds. \n";            
       
       tm *ltm4 = localtime(&StopTime);                                         //Here i define ltm3 and output to program and log         
        log <<"Lamp2  Stop: "<<ltm4->tm_mday << "/";                              //Writing Lamp Start Date to Log
        log <<ltm4->tm_mon+1 << "/";
        log <<ltm4->tm_year +1900<< "_"; 
        log <<ltm4->tm_hour<< ":";
        log <<ltm4->tm_min << ":";
        log <<ltm4->tm_sec<<" ";
        
        
        tm *ltm3 = localtime(&LampOnTime);                                        //Here i define ltm3 and output to program and log         
        cout<<"Lamp was on for "<<ltm3->tm_hour-1<< ":";
        cout<<ltm3->tm_min << ":";
        cout<<ltm3->tm_sec<<": \n";
        
        log<<"Lamp on for "<<ltm3->tm_hour-1<< ":";
        log<<ltm3->tm_min << ":";
        log<<ltm3->tm_sec<<" \n";
        log<<"======================================================= \n";  
          
        log.close();
        
        cout<<"======================================================= \n"; 

      
 
	}   //End of program
  
	sf.Close();                        //Close Scan_file "output.txt"
	log.close();                       //Close Log file
    system("pause");
    PostQuitMessage(EXIT_SUCCESS);
    Sleep(0);
    return(EXIT_SUCCESS);
}
