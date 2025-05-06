#include <iostream>
#include <string>
#include <fstream>
using namespace std;



int main() {
	int time;
	int num;
	int index = 1;
	int sleep=0;
	int end=0;
	int nlist[100];

	ifstream fin("batch.inp");
	ofstream fout("batch.out");
	fin >> time;
	for (int i = 0; i < time; i++)
	{

		while (true)
		{
			fin >> num;
			if (num == -1)
			{
				index = 1;
				break;
			}
			if (index % 2 == 0)
			{
				sleep += num;

			}
			
			end += num;
			index += 1;
		}
	}
	fout << sleep << " " << end << endl;


	fin.close();
	fout.close();


	return 0;
}