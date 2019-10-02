#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <climits>
#include <fstream>
#include <streambuf>
#include <chrono>
#include <vector>
#include <bitset>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;
using std::stringstream;

class Timer {
private:
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

unsigned int MakeStringInt(string input) //string pavertimas unsigned int (is interneto)
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
	return ((B * A) ^ (C * D));
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
	blokas = hashB(A, B, C, D) + 0x155CB7A7;
	blokas = BitRotate(blokas, rotate);
}
void Hashas3(unsigned int &blokas, unsigned int A, unsigned int B, unsigned int C, unsigned int D, int rotate)
{
	blokas = hashC(A, B, C, D);
	blokas = BitRotate(blokas, rotate);
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
	failas << std::hex << Out << endl;
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
void HashstringFile(char *pav, ofstream& failas)
{
	string input;
	ifstream t(pav); //viso failo skaitymas (is stackoverflow)
	t.seekg(0, std::ios::end);
	input.reserve(t.tellg());
	t.seekg(0, std::ios::beg);
	input.assign((std::istreambuf_iterator<char>(t)),
	             std::istreambuf_iterator<char>());


	Timer T;

	input.append(64 - input.length() % 64 + 64, '\0');		 // padarom inputa 64 kartotiniu ir padidinam 64 nuliais dar
	unsigned int Key[4] = {0x25B6112A, 0x29F43018, 0x155CB7A7, 0X7232AAAF}; //random reiksmes
	for (int i = 0; i < input.length() / 64; i++)
	{

		unsigned int block[16];
		for (int j = 0; j < 16; j++)
		{
			block[j] = MakeStringInt(input.substr(64 * i + 4 * j, 4)); //nurodo i kokia pozicija det stringa ir kokio ilgio
		}
		unsigned int A = Key[0];
		unsigned int B = Key[1];
		unsigned int C = Key[2];
		unsigned int D = Key[3];



		Hashas3(B, D, C, block[0], A, 16);
		Hashas2(D, B, A, block[1], C, 16);
		Hashas1(A, D, B, block[2], C, 16);
		Hashas2(A, D, B, block[3], C, 16);

		Hashas1(A, B, C, D, block[4], 4);
		Hashas2(C, B, A, D, block[5], 4);
		Hashas3(B, C, A, D, block[6], 4);
		Hashas3(D, A, C, B, block[7], 4);

		Hashas3(A, block[8], B, D, C, 8);
		Hashas2(A, block[9], C, D, B, 8);
		Hashas1(D, block[10], B, C, A, 8);
		Hashas1(C, block[11], A, D, B, 8);

		Hashas2(B, D, block[12], C, A, 12);
		Hashas3(D, B, block[13], A, C, 12);
		Hashas1(A, D, block[14], B, C, 12);
		Hashas3(A, D, block[15], B, C, 12);
		Key[0] += D;
		Key[1] += B;
		Key[2] += A;
		Key[3] += C;
	}

	auto laikas = T.elapsed();

	isvedimas(Key, failas);
	failas << endl;
}
void Hashstring(char *pav, ofstream & failas)
{
	ifstream t(pav);
	std::vector<string> v;
	v.reserve(5000);
	while (!t.eof())
	{
		string inputas;
		getline(t, inputas);
		v.push_back(inputas);
	}
	Timer T;
	for (int o = 0; o < v.size(); o++)
	{
		unsigned int Key[4] = {0x25B6112A, 0x29F43018, 0x155CB7A7, 0X7232AAAF}; //632688938 , 703868952, 358397863, 1915923119
		v[o].append(64 - v[o].length() % 64 + 64, '\0');		 // padarom inputa 64 kartotiniu ir padidinam paddinga dar 64 0liais
		for (int i = 0; i < v[o].length() / 64; i++)
		{

			unsigned int block[4];
			for (int j = 0; j < 4; j++)
			{
				block[j] = MakeStringInt(v[o].substr(64 * i + 4 * j, 4)); //nurodo i kokia pozicija det stringa ir kokio ilgio
			}
			unsigned int A = Key[0];
			unsigned int B = Key[1];
			unsigned int C = Key[2];
			unsigned int D = Key[3];


			Hashas3(B, D, C, block[0], A, 16);
			Hashas2(D, B, A, block[1], C, 16);
			Hashas1(A, D, B, block[2], C, 16);
			Hashas2(A, D, B, block[3], C, 16);

			Hashas1(A, B, C, D, block[4], 4);
			Hashas2(C, B, A, D, block[5], 4);
			Hashas3(B, C, A, D, block[6], 4);
			Hashas3(D, A, C, B, block[7], 4);

			Hashas3(A, block[8], B, D, C, 8);
			Hashas2(A, block[9], C, D, B, 8);
			Hashas1(D, block[10], B, C, A, 8);
			Hashas1(C, block[11], A, D, B, 8);

			Hashas2(B, D, block[12], C, A, 12);
			Hashas3(D, B, block[13], A, C, 12);
			Hashas1(A, D, block[14], B, C, 12);
			Hashas3(A, D, block[15], B, C, 12);
			Key[0] += D;
			Key[1] += B;
			Key[2] += A;
			Key[3] += C;

		}

		isvedimas(Key, failas);
	}
	auto laikas = T.elapsed();
	failas << laikas << " s" << endl;
	failas << endl;
	cout << laikas << " s" << endl;
}
int bitdifference(unsigned int A[], unsigned int B[])
{
	int bitdifference = 0;
	for (int i = 0; i < 4; i++) {
		int temp = 0;
		for (int j = 0; j < 32; j++) {
			if (((A[i] >> j) & 1) != ((B[i] >> j) & 1))
			{
				temp++;
			}
		}
		bitdifference += temp;
	}

	return bitdifference;
}
unsigned int* hashforCompare(string & v, unsigned int Key[])
{
	v.append(64 - v.length() % 64 + 64, '\0');		 // padarom inputa 64 kartotiniu ir padidinam paddinga dar 64 0liais
	for (int i = 0; i < v.length() / 64; i++)
	{

		unsigned int block[4];
		for (int j = 0; j < 4; j++)
		{
			block[j] = MakeStringInt(v.substr(64 * i + 4 * j, 4)); //nurodo i kokia pozicija det stringa ir kokio ilgio
		}
		unsigned int A = Key[0];
		unsigned int B = Key[1];
		unsigned int C = Key[2];
		unsigned int D = Key[3];


		Hashas3(B, D, C, block[0], A, 16);
		Hashas2(D, B, A, block[1], C, 16);
		Hashas1(A, D, B, block[2], C, 16);
		Hashas2(A, D, B, block[3], C, 16);

		Hashas1(A, B, C, D, block[4], 4);
		Hashas2(C, B, A, D, block[5], 4);
		Hashas3(B, C, A, D, block[6], 4);
		Hashas3(D, A, C, B, block[7], 4);

		Hashas3(A, block[8], B, D, C, 8);
		Hashas2(A, block[9], C, D, B, 8);
		Hashas1(D, block[10], B, C, A, 8);
		Hashas1(C, block[11], A, D, B, 8);

		Hashas2(B, D, block[12], C, A, 12);
		Hashas3(D, B, block[13], A, C, 12);
		Hashas1(A, D, block[14], B, C, 12);
		Hashas3(A, D, block[15], B, C, 12);
		Key[0] += D;
		Key[1] += B;
		Key[2] += A;
		Key[3] += C;

	}
	return Key;

}
void compare(char * pav)
{
	ifstream failas(pav);
	int sutampa=0;
	int a=0;
	string input1,input2;
	while (failas>>input1>>input2)
	{
		if(input1==input2)
		sutampa++;
	a++;
	}
	cout<<"is "<<a<<" poru "<<sutampa<<" sutampa."<<endl;
}
void Bitcomparison() {
	int amount = 0;
	int totalskirt = 0;
	int min = 128;
	int max = 0;
	std::ifstream failas ("4testas.txt");
	string line;

	unsigned int Key1[4] = {0x25B6112A, 0x29F43018, 0x155CB7A7, 0X7232AAAF};
	unsigned int Key2[4] = {0x25B6112A, 0x29F43018, 0x155CB7A7, 0X7232AAAF};
	unsigned int *now;
	unsigned int *next;

	while (getline(failas, line))
	{
		std::istringstream iss(line);
		string input1, input2;
		iss >> input1 >> input2;
		now = hashforCompare(input1, Key1);
		next = hashforCompare(input2, Key2);
		int skirtumas = bitdifference(now, next);

		if (skirtumas < min)
			min = skirtumas;
		if (skirtumas > max)
			max = skirtumas;
		totalskirt += skirtumas;
		amount++;
	}

	cout << totalskirt / amount / 1.28 << "%" << " Vidurkis" << endl;
	cout << max / 1.28 << "%" << " daugiausiai skyresi " << min / 1.28 << " %" << " maziausiai skyresi" << endl;
}
int main(int argc, char *argv[])
{

	ofstream failas("rez.txt");
	if (argc > 1)
	{
		for (int i = 1; i < argc; i += 2)
		{
			cout << argv[i + 1] << endl;
			if (argv[i + 1] == string("eilute"))
			{
				Hashstring(argv[i], failas);
			}
			else if (argv[i + 1] == string("compare"))
			{
				compare(argv[i]);

			}
			else if (argv[i + 1] == string("failas"))
			{
				HashstringFile(argv[i], failas);
			}
			else if (argv[i + 1] == string("testas"))
			{
				Bitcomparison();
			}

		}
	}
	else
	{
		cout << "Iveskite duomenis: ";
		string input;
		getline(cin, input);
		input.append(64 - input.length() % 64 + 64, '\0');// padidinam paddinga dar 64 0liais

		unsigned int Key[4] = {0x25B6112A, 0x29F43018, 0x155CB7A7, 0X7232AAAF}; //random reiksmes
		for (int i = 0; i < input.length() / 64; i++)
		{

			unsigned int block[4];
			for (int j = 0; j < 4; j++)
			{
				block[j] = MakeStringInt(input.substr(64 * i + 4 * j, 4)); //nurodo i kokia pozicija det stringa ir kokio ilgio
			}
			unsigned int A = Key[0];
			unsigned int B = Key[1];
			unsigned int C = Key[2];
			unsigned int D = Key[3];


			Hashas3(B, D, C, block[0], A, 16);
			Hashas2(D, B, A, block[1], C, 16);
			Hashas1(A, D, B, block[2], C, 16);
			Hashas2(A, D, B, block[3], C, 16);

			Hashas1(A, B, C, D, block[4], 4);
			Hashas2(C, B, A, D, block[5], 4);
			Hashas3(B, C, A, D, block[6], 4);
			Hashas3(D, A, C, B, block[7], 4);

			Hashas3(A, block[8], B, D, C, 8);
			Hashas2(A, block[9], C, D, B, 8);
			Hashas1(D, block[10], B, C, A, 8);
			Hashas1(C, block[11], A, D, B, 8);

			Hashas2(B, D, block[12], C, A, 12);
			Hashas3(D, B, block[13], A, C, 12);
			Hashas1(A, D, block[14], B, C, 12);
			Hashas3(A, D, block[15], B, C, 12);
			Key[0] += D;
			Key[1] += B;
			Key[2] += A;
			Key[3] += C;
		}
		isvedimas(Key);
	}

	return 0;
}