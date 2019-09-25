#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <climits>
#include <fstream>
#include <streambuf>
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;
using std::stringstream;

unsigned int StringToInt(string input) //string pavertimas unsigned int (is interneto)
{
	return (unsigned int)input[0] | (unsigned int)(input[1] << 8) | (unsigned int)(input[2] << 16) | (unsigned int)(input[3] << 24);
}
unsigned int BitRotate(unsigned int value, int count) //bitwise shift algoritmas
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
void Hashas1(unsigned int &blokas, unsigned int A, unsigned int B, unsigned int C, unsigned int D, int rotate)
{
	blokas = hashA(A, B, C, D);
	blokas = BitRotate(blokas, rotate);
}
void Hashas2(unsigned int &blokas, unsigned int A, unsigned int B, unsigned int C, unsigned int D, int rotate)
{
	blokas = hashB(A, B, C, D) //703868952
	blokas = BitRotate(blokas, rotate);
}
void Hashas3(unsigned int &blokas, unsigned int A, unsigned int B, unsigned int C, unsigned int D, int rotate)
{
	blokas = hashC(A, B, C, D) + 0x25B6112A; //632688938 random reiksme, kad nesikartotu maisymas
	blokas = BitRotate(blokas, rotate);
}
void isvedimas(unsigned int * output)
{
	stringstream stream;
	for (int i = 0; i < 4; i++)
	{
		stream << std::hex << std::setw(8) << std::setfill('0') << output[i];
	}
	string Out;
	Out = stream.str();
	cout << std::hex << Out << endl;
}
int main(int argc, char *argv[])
{
	string input;
	if (argc == 2)
	{

		ifstream t(argv[1]); //viso failo skaitymas (is stackoverflow)
		t.seekg(0, std::ios::end);
		input.reserve(t.tellg());
		t.seekg(0, std::ios::beg);
		input.assign((std::istreambuf_iterator<char>(t)),
		             std::istreambuf_iterator<char>());
	}
	else
	{
		cout << "Iveskite duomenis: ";

		getline(cin, input);
	}

	string inputL = std::to_string((int)input.length()); //input'o ilgis
	input.append(64 - input.length() % 64, '\0');		 // padarom inputa 64 kartotiniu
	input.append(64, '\0');								 // padidinam paddinga dar 64 0liais

	unsigned int Key[4] = {0x25B6112A, 0x29F43018, 0x155CB7A7, 0X7232AAAF}; //random reiksmes
	for (int i = 0; i < input.length() / 64; i++)
	{

		unsigned int block[4];
		for (int j = 0; j < 4; j++)
		{
			block[j] = StringToInt(input.substr(64 * i + 4 * j, 4)); //nurodo i kokia pozicija det stringa ir kokio ilgio
		}
		unsigned int A = Key[0];
		unsigned int B = Key[1];
		unsigned int C = Key[2];
		unsigned int D = Key[3];

		Hashas1(A, B, C, D, block[0], 4);
		Hashas2(C, B, A, D, block[1], 4);
		Hashas3(B, C, A, D, block[2], 4);
		Hashas3(D, A, C, B, block[3], 4);

		Hashas3(block[0], B, D, C, A, 8);
		Hashas2(block[1], A, C, D, B, 8);
		Hashas1(block[2], D, B, C, A, 8);
		Hashas1(block[3], C, A, D, B, 8);

		Hashas2(B, D, block[0], C, A, 12);
		Hashas3(D, B, block[1], A, C, 12);
		Hashas1(A, D, block[2], B, C, 12);
		Hashas3(A, D, block[3], B, C, 12);

		Hashas3(B, D, C, block[0], A, 16);
		Hashas2(D, B, A, block[1], C, 16);
		Hashas1(A, D, B, block[2], C, 16);
		Hashas2(A, D, B, block[3], C, 16);

		Key[0] += D;
		Key[1] += B;
		Key[2] += A;
		Key[3] += C;
	}
	isvedimas(Key);
	return 0;
}