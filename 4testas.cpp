 #include<bits/stdc++.h>  
using namespace std;  
  
void combinationUtil(char arr[], char data[],  
                    int start, int end,  
                    int index, int r, ofstream &failas);  
 
void printCombination(char arr[], int n, int r, ofstream & failas)  
{  
    char data[r];   
    combinationUtil(arr, data, 0, n-1, 0, r, failas);  
}  
void combinationUtil(char arr[], char data[],  
                    int start, int end,  
                    int index, int r,ofstream & failas)  
{
    if (index == r)  
    {  
        for (int j = 0; j < r; j++)  
            failas << data[j];  
        failas << endl;  
        return;  
    }  
    for (int i = start; i <= end &&  
        end - i + 1 >= r - index; i++)  
    {  
        data[index] = arr[i];  
        combinationUtil(arr, data, i+1,  
                        end, index+1, r, failas);  
    }  
}   
int main()  
{   ofstream failas("4testas.txt");
    char arr[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
    '!','#'};  
    int r = 4;  
    int n = sizeof(arr)/sizeof(arr[0]);  
    printCombination(arr, n, r,failas);  
}  