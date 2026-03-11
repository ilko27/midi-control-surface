#include "MIDIUSB.h"

void controlChange(byte channel, byte control, byte value)
{
    midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
    MidiUSB.sendMIDI(event);
}


const int MUX[] = {2, 3, 4, 5};
const int MUX_INPUT = 10;
const int NUMBER_OF_POTS = 14;

int pots[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int lastPots[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void readMux()
{
    for (int i = 0; i < NUMBER_OF_POTS; i++)
    {
        digitalWrite(MUX[0], i & 0x01);
        digitalWrite(MUX[1], (i >> 1) & 0x01);
        digitalWrite(MUX[2], (i >> 2) & 0x01);
        digitalWrite(MUX[3], (i >> 3) & 0x01);

        delayMicroseconds(50);

        // analogRead(MUX_INPUT);
        int value = analogRead(MUX_INPUT);
        value = map(value, 0, 1023, 0, 127);
        pots[i] = value;
    }
}

void printPots()
{
    char prBuff[200];
    sprintf(prBuff, "0:%5d | 1:%5d | 2:%5d | 3:%5d | 4:%5d | 5:%5d | 6:%5d | 7:%5d | 8:%5d | 9:%5d | 10:%5d | 11:%5d | 12:%5d | 13:%5d", pots[0], pots[1], pots[2], pots[3], pots[4], pots[5], pots[6], pots[7], pots[8], pots[9], pots[10], pots[11], pots[12], pots[13]);
    Serial.println(prBuff);
}

void setup()
{
    Serial.begin(115200);

    pinMode(MUX[0], OUTPUT);
    pinMode(MUX[1], OUTPUT);
    pinMode(MUX[2], OUTPUT);
    pinMode(MUX[3], OUTPUT);
}

void loop()
{
    readMux();
    // // printPots();

    for (int i = 0; i < NUMBER_OF_POTS; i++)
    {
        if (pots[i] != lastPots[i])
        {
            controlChange(2, i, pots[i]);
            lastPots[i] = pots[i];
            MidiUSB.flush();
            
            printPots();
        }
    }
}