#include <string>
#include <fstream>
#include <random>
#include <chrono>

using namespace std;



string generate(int max_length){
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    string possible_characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!#@$^&*()_+-:;,./<>?@|";
	std::default_random_engine generator (seed);
    std::mt19937 engine(generator());
	std::uniform_real_distribution<double> distribution (0.0,100.0);
    string ret = "";
    for(int i = 0; i < max_length; i++){
        int random_index = distribution(engine()); //get index between 0 and possible_characters.size()-1
        ret += possible_characters[random_index];
    }
    return ret;
}
int main ()
{
	ofstream failas ("stringpair.txt");
	for(int i=0; i<100;i++)
	{
	failas<<generate(60)<<endl;
	}
	return 9;
}