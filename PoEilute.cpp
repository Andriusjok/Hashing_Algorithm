#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <climits>
#include <fstream>
#include <streambuf>
#include <chrono>
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;
using std::stringstream;

class Timer {
private:
	// panaudojame using
	using hrClock = std::chrono::high_resolution_clock;
	using durationDouble = std::chrono::duration<double>;
	std::chrono::time_point<hrClock> start;
public:
	Timer() : start{ hrClock::now() } {}
	void reset() {
		start = hrClock::now();
	}
	double elapsed() const {
		return durationDouble (hrClock::now() - start).count();
	}
};

unsigned int StringToInt(string input) //string pavertimas unsigned int (is interneto)
{
	return (unsigned int)input[0] | (unsigned int)(input[1] << 8) | (unsigned int)(input[2] << 16) | (unsigned int)(input[3] << 24);
}
unsigned int BitRotate(unsigned int value, int count) //bitwise shift algoritmas is interneto
{
	const unsigned int mask = CHAR_BIT * sizeof(value) - 1;
	count &= mask;
	return (value << count) | (value >> (-count & mask));
}
unsigned int hashA(unsigned int A, unsigned int B, unsigned int C, unsigned int D)
{
	return ((B * A) & (C * D));
}

unsigned int hashB(unsigned int A, unsigned int B, unsigned int C, unsigned int D)
{
	return ((C + B) ^ (D + A));
}

unsigned int hashC(unsigned int A, unsigned int B, unsigned int C, unsigned int D)
{
	return ((A + C) ^ (D + B));
}

void Hashas1(unsigned int &blokas, unsigned int A, unsigned int B, unsigned int C, unsigned int D, int s)
{
	blokas = hashA(A, B, C, D);
	blokas = BitRotate(blokas, s);
}
void Hashas2(unsigned int &blokas, unsigned int A, unsigned int B, unsigned int C, unsigned int D, int s)
{
	blokas = hashB(A, B, C, D) + 0x29F43018;
	blokas = BitRotate(blokas, s);
}
void Hashas3(unsigned int &blokas, unsigned int A, unsigned int B, unsigned int C, unsigned int D, int s)
{
	blokas = hashC(A, B, C, D) + 0x25B6112A;
	blokas = BitRotate(blokas, s);
}
void isvedimas(unsigned int * output, ofstream & failas)
{
	stringstream stream;
	for (int i = 0; i < 4; i++)
	{
		stream << std::hex << std::setw(8) << std::setfill('0') << output[i];
	}
	string Out;
	Out = stream.str();
	failas << std::hex << Out<< endl;
}
void Hashstring(string input, ofstream & failas)
{


	string inputL = std::to_string((int)input.length()); //input'o ilgis
	input.append(64 - input.length() % 64, '\0');		 // padarom inputa 64 kartotiniu
	input.append(64 - inputL.length(), '\0');			 // uzpildom appendinta vieta nuliais
	input.append(inputL);								 // padvigubinam paddinga

	unsigned int Key[4] = {0x25B6112A, 0x29F43018, 0x155CB7A7, 0X7232AAAF}; //random reiksmes
	for (int i = 0; i < input.length() / 64; i++)
	{

		unsigned int block[16];
		for (int j = 0; j < 16; j++)
		{
			block[j] = StringToInt(input.substr(64 * i + 4 * j, 4));
		}
		unsigned int A = Key[0];
		unsigned int B = Key[1];
		unsigned int C = Key[2];
		unsigned int D = Key[3];

		Hashas1(A, B, C, D, block[0], 3);
		Hashas2(C, B, A, D, block[1], 3);
		Hashas3(B, C, A, D, block[2], 3);

		Hashas3(D, B, C, A, block[3], 7);
		Hashas2(A, B, C, D, block[4], 7);
		Hashas1(B, C, A, D, block[5], 7);

		Hashas2(C, B, A, D, block[6], 11);
		Hashas1(D, B, A, C, block[7], 11);
		Hashas3(A, C, B, D, block[8], 11);

		Hashas3(B, D, C, A, block[9], 15);
		Hashas1(C, B, A, D, block[10], 15);
		Hashas2(D, C, A, B, block[11], 15);

		Hashas3(A, B, C, D, block[12], 19);
		Hashas3(B, D, A, C, block[13], 19);
		Hashas3(C, D, A, B, block[14], 19);
		Hashas3(D, A, B, C, block[15], 19);
		Key[0] += D;
		Key[1] += B;
		Key[2] += A;
		Key[3] += C;
	}

	isvedimas(Key, failas);
}
int main(int argc, char *argv[])
{
	string input;
		Timer T;
	ofstream failas ("rez.txt");
	if (argc == 2)
	{

		ifstream t(argv[1]); //viso failo skaitymas (is stackoverflow)
		while (!t.eof())
		{
			string inputas;
			getline(t, inputas);
			Hashstring(inputas, failas);
		}
	}
	else
	{
		cout << "Iveskite duomenis: ";

		getline(cin, input);
	}
	failas<<T.elapsed()<<endl;	
	return 0;
}