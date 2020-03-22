#include "task2.h"

#include <windows.h>
#include <thread>
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <chrono>

using namespace std;

void DO(DWORD duration){ Beep(261, duration); }
void RE(DWORD duration){ Beep(293, duration); }
void MI(DWORD duration){ Beep(329, duration); }
void FA(DWORD duration){ Beep(349, duration); }
void SOL(DWORD duration){ Beep(392, duration); }
void LA(DWORD duration){ Beep(440, duration); }
void SI(DWORD duration){ Beep(493, duration); }

void playGamma()
{
    DO(200);
    RE(200);
    MI(200);
    FA(200);
    SOL(200);
    LA(200);
    SI(200);
}

void playAccord()
{
    thread th1(FA,1500);
    th1.detach();
    thread th2(SI,1500);
    th2.detach();
    thread th3(SOL,1500);
    th3.detach();
}

void Task2()
{
    playGamma();

    playAccord();
    this_thread::sleep_for(chrono::milliseconds(3000));
}
