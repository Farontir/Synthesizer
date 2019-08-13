#include <iostream>

#include "NoiseMaker.h"

atomic<double> dFrequencyOutput = 0.0;

double w(double dHertz)
{
	return dHertz * 2.0 * PI;
}

double osc(double dHertz, double dTime, int nType)
{
	switch (nType)
	{
		case 0: // Sine Wave
			return sin(w(dHertz) * dTime);
		case 1: // Square Wave
			return sin(w(dHertz) * dTime) > 0.0 ? 1.0 : -1.0;
		case 2: // Triangle Wave
			return asin(sin(w(dHertz) * dTime)) * 2.0 / PI;

		default:
			return 0;
	}
}

double MakeNoise(double dTime)
{
	double dOutput = osc(dFrequencyOutput, dTime, 2); // change the osc here 
	return dOutput * 0.5;
}

int main()
{
	std::wcout << "Synthesizer" << endl;

	// Get all sound hardware
	vector<std::wstring> devices = NoiseMaker<short>::Enumerate();

	for (auto d : devices) std::wcout << "Found Output Device: " << d << endl;

	// Display a keyboard
	wcout << endl <<
		"|   |   |   |   |   | |   |   |   |   | |   | |   |   |   |" << endl <<
		"|   | S |   |   | F | | G |   |   | J | | K | | L |   |   |" << endl <<
		"|   |___|   |   |___| |___|   |   |___| |___| |___|   |   |__" << endl <<
		"|     |     |     |     |     |     |     |     |     |     |" << endl <<
		"|  W  |  X  |  C  |  V  |  B  |  N  |  ,  |  ;  |  :  |  !  |" << endl <<
		"|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|" << endl << endl;


	// Create Sound Machine
	NoiseMaker<short> sound(devices[0], 44100, 1, 8, 512);

	// Link noise function with Noise Maker object
	sound.SetUserFunction(MakeNoise);

	double dOctaveBaseFrequency = 110.0; // A2
	double d12thRootOf2 = pow(2.0, 1.0 / 12.0);

	while (1)
	{
		// Add keyboard 

		bool bKeyPressed = false;

		for (int k = 0; k < 15; k++)
		{
			if (GetAsyncKeyState((unsigned char)("WSXCFVGBNJ?K\xbcL\xbe"[k])) & 0x8000)
			{
				dFrequencyOutput = dOctaveBaseFrequency * pow(d12thRootOf2, k);
				bKeyPressed = true;
			}
		}

		if (!bKeyPressed)
		{
			dFrequencyOutput = 0.0;
		}
	}

	return 0;
}