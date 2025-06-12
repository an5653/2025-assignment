#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;




int passTime(int time, const vector<vector<int>>& plist, const vector<int>& optime) {
	int pmin = 10000;
	int omin = 10000;

	for (int i = 0; i < plist.size(); i++) {
		if (plist[i][0] > time && plist[i][0] < pmin) {
			pmin = plist[i][0];
		}
	}

	for (int i = 0; i < optime.size(); i++) {
		if (optime[i] > time && optime[i] < omin) {
			omin = optime[i];
		}
	}

	int mi = min(pmin, omin);
	if (mi == 10000 || time > mi) {
		mi = time + 1;
	}
	return mi;
}

int prosessIn(const vector<int>& plist, vector<vector<int>>& memory, int index, int tmp) {
	int n = index;
	int num = plist[2];

	memory[n][0] += num;
	memory[n][2] -= num;

	int start = 0;
	if (index != 0)
		start = memory[index - 1][2] + memory[index - 1][0];
	memory.insert(memory.begin() + index, { start,plist[1],num ,tmp });
	return start;
}

bool checkHole(const vector<vector<int>>& memory, vector<int>& hole, vector<int>& hole_spot) {
	hole.clear();
	hole_spot.clear();
	for (int i = 0; i < memory.size(); i++) {
		if (memory[i][1] == -1) {
			hole.push_back(memory[i][2]);
			hole_spot.push_back(i);
		}
	}

	return !hole.empty();
}

int firstFit(const vector<int>& hole, const vector<int>& plist, vector<int>& hole_spot) {
	vector<int> index;
	vector<int> hole_size;
	vector<int> hole_index;
	for (int i = 0; i < hole.size(); i++)
	{
		if (hole[i] >= plist[2])
		{
			index.push_back(i);
			hole_size.push_back(hole[i]);
			hole_index.push_back(hole_spot[i]);
		}
	}
	if (index.empty())
	{
		return -1;
	}

	return hole_index[0];
}
int bestFit(const vector<int>& hole, const vector<int>& plist, vector<int>& hole_spot) {
	vector<int> index;
	vector<int> hole_size;
	vector<int> hole_index;
	for (int i = 0; i < hole.size(); i++)
	{
		if (hole[i] >= plist[2])
		{
			index.push_back(i);
			hole_size.push_back(hole[i]);
			hole_index.push_back(hole_spot[i]);

		}

	}

	if (index.empty())
		return -1;
	int closest = 10000;
	int num = -1;
	for (int i = 0; i < index.size(); ++i) {
		if (hole_size[i] >= plist[2] && hole_size[i] < closest) {
			closest = hole_size[i];
			num = i;
		}
	}
	if (num == -1)
		return -1;

	return hole_index[num];
}
int worstFit(const vector<int>& hole, const vector<int>& plist, vector<int>& hole_spot) {
	vector<int> index;
	vector<int> hole_size;
	vector<int> hole_index;
	for (int i = 0; i < hole.size(); i++)
	{
		if (hole[i] >= plist[2])
		{
			index.push_back(i);
			hole_size.push_back(hole[i]);
			hole_index.push_back(hole_spot[i]);
		}
	}
	if (index.empty())
		return -1;
	int max = -1;
	int num = -1;
	for (int i = 0; i < index.size(); i++)
	{
		if (max < hole_size[i])
		{
			max = hole_size[i];
			num = i;
		}
	}
	if (num == -1)
		return -1;
	return hole_index[num];
}
int findPros(const vector<vector<int>>& memory, int num) {
	int index = -1;
	for (int i = 0; i < memory.size(); i++)
	{
		if (memory[i][3] == num)
		{
			index = i;
		}
	}

	return index;
}
void closePros(vector<vector<int>>& memory, vector<int>& optime, int index, int num) {

	memory[index][1] = -1;
	optime[num] = -1;
	memory[index][3] = -1;

}

bool mergePoint(const vector<vector<int>>& vec, vector<int>& outRange) {
	outRange.clear();

	if (vec.size() < 2) return false;

	int start = -1;

	for (int i = 1; i < vec.size(); ++i) {
		if (vec[i][1] == -1 && vec[i - 1][1] == -1) {
			if (start == -1) {
				start = i - 1;
			}
		}
		else {
			if (start != -1) {
				outRange.push_back(start);
				outRange.push_back(i - 1);
				return true;
			}
		}
	}

	if (start != -1) {
		outRange.push_back(start);
		outRange.push_back(static_cast<int>(vec.size() - 1));
		return true;
	}

	return false;
}

void mergeHole(vector<vector<int>>& memory, const vector<int>outRange) {
	vector<int> tmp(4);
	int start = outRange[0];
	int end = outRange[1];
	tmp[1] = -1;
	tmp[3] = -1;
	tmp[0] = memory[outRange[0]][0];

	for (int i = start; i <= end; ++i)
	{
		tmp[2] += memory[i][2];
	}
	memory[outRange[0]] = tmp;
	memory.erase(memory.begin() + start + 1, memory.begin() + end + 1);

}

vector<int> convert(const vector<int>& plist, int num) {
	vector<int> list(3);

	list[0] = plist[1];
	list[1] = num;
	list[2] = plist[2];

	return list;
}

bool allDone(const vector<int>& optime) {
	for (int x : optime) {
		if (x != -1) {
			return false;
		}
	}
	return true;
}
int main() {


	int n;
	ifstream fin;
	fin.open("allocation.inp");
	ofstream fout;
	fout.open("allocation.out");
	string line;
	getline(fin, line);
	n = stoi(line);

	vector<vector<int>> plist;
	vector<int> optime(n);
	vector<int> sleeptime;
	vector<vector<int>> memory;
	memory.push_back({ 0,-1,1000,-1 });
	vector<int> hole;
	vector<vector<int>> sleep;
	vector<int> outRange;
	vector<int> hole_spot;
	vector<int> p_index(n);
	vector<int> last(n);

	vector<int> numbers;
	string space = "\n";
	while (getline(fin, line)) {
		istringstream iss(line);
		vector<int> row;
		int num;
		while (iss >> num) {
			row.push_back(num);
		}
		plist.push_back(row);
	}
	int time = -1;


	for (int i = 0; i < n; i++)
	{
		optime[i] = -2;
		p_index[i] = -2;
		last[i] = -2;
	}
	while (true)	
	{
		int index = -1;
		time = passTime(time, plist, optime);
		if (checkHole(memory, hole, hole_spot) == true)
		{
			for (int i = 0; i < plist.size(); i++)
			{
				if (time == plist[i][0])
				{
					index = firstFit(hole, plist[i], hole_spot);
					if (index != -1)
					{
						last[i] = prosessIn(plist[i], memory, index, i);
						optime[i] = time + plist[i][1];
						p_index[i] = index;
					}
					else

						sleep.push_back(convert(plist[i], i));
				}
			}

		}
		for (int i = 0; i < optime.size(); i++)
		{
			int index = -1;
			if (time == optime[i])
			{
				index = findPros(memory, i);
				closePros(memory, optime, index, i);

			}
		}


		while (mergePoint(memory, outRange) == true)
		{
			mergeHole(memory, outRange);
		}
		if (!sleep.empty()) {
			vector<vector<int>> tmp_sleep;

			for (int i = 0; i < sleep.size(); ++i) {
				checkHole(memory, hole, hole_spot);
				int tmp_index = firstFit(hole, sleep[i], hole_spot);
				if (tmp_index != -1) {
					int original_index = sleep[i][1];
					last[original_index] = prosessIn(plist[original_index], memory, tmp_index, original_index);
					optime[original_index] = time + plist[original_index][1];
					p_index[original_index] = tmp_index;

				}
				else {
					tmp_sleep.push_back(sleep[i]);
				}
			}
			sleep = tmp_sleep;

		}

		if (allDone(optime))
			break;

	}
	line = to_string(last[n - 1]);
	fout.write(line.c_str(), line.size());
	fout.write(space.c_str(), space.size());




	time = -1;
	for (int i = 0; i < n; i++)
	{
		optime[i] = -2;
		p_index[i] = -2;
		last[i] = -2;
	}
	memory.clear();
	memory.push_back({ 0, -1, 1000, -1 });
	sleep.clear();

	while (true)
	{
		int index = -1;
		time = passTime(time, plist, optime);

		if (checkHole(memory, hole, hole_spot) == true)
		{
			for (int i = 0; i < plist.size(); i++)
			{
				if (time == plist[i][0])
				{

					index = bestFit(hole, plist[i], hole_spot);

					if (index != -1)
					{
						last[i] = prosessIn(plist[i], memory, index, i);
						optime[i] = time + plist[i][1];
						p_index[i] = index;

					}
					else

						sleep.push_back(convert(plist[i], i));
				}
			}

		}
		for (int i = 0; i < optime.size(); i++)
		{
			int index = -1;
			if (time == optime[i])
			{
				index = findPros(memory, i);
				closePros(memory, optime, index, i);

			}
		}


		while (mergePoint(memory, outRange) == true)
		{

			mergeHole(memory, outRange);
		}
		if (!sleep.empty()) {
			vector<vector<int>> tmp_sleep;

			for (int i = 0; i < sleep.size(); ++i) {
				checkHole(memory, hole, hole_spot);
				int tmp_index = bestFit(hole, sleep[i], hole_spot);
				if (tmp_index != -1) {
					int original_index = sleep[i][1];
					last[original_index] = prosessIn(plist[original_index], memory, tmp_index, original_index);
					optime[original_index] = time + plist[original_index][1];
					p_index[original_index] = tmp_index;

				}
				else {
					tmp_sleep.push_back(sleep[i]);
				}
			}
			sleep = tmp_sleep;

		}

		if (allDone(optime))
			break;

	}
	line = to_string(last[n - 1]);
	fout.write(line.c_str(), line.size());
	fout.write(space.c_str(), space.size());



	time = -1;
	for (int i = 0; i < n; i++)
	{
		optime[i] = -2;
		p_index[i] = -2;
		last[i] = -2;
	}

	memory.clear();
	memory.push_back({ 0, -1, 1000, -1 });
	sleep.clear();
	while (true)
	{
		int index = -1;
		time = passTime(time, plist, optime);

		if (checkHole(memory, hole, hole_spot) == true)
		{
			for (int i = 0; i < plist.size(); i++)
			{
				if (time == plist[i][0])
				{

					index = worstFit(hole, plist[i], hole_spot);

					if (index != -1)
					{
						last[i] = prosessIn(plist[i], memory, index, i);
						optime[i] = time + plist[i][1];
						p_index[i] = index;

					}
					else

						sleep.push_back(convert(plist[i], i));
				}
			}

		}
		for (int i = 0; i < optime.size(); i++)
		{
			int index = -1;
			if (time == optime[i])
			{
				index = findPros(memory, i);
				closePros(memory, optime, index, i);

			}
		}


		while (mergePoint(memory, outRange) == true)
		{

			mergeHole(memory, outRange);
		}

		if (!sleep.empty()) {
			vector<vector<int>> tmp_sleep;

			for (int i = 0; i < sleep.size(); ++i) {
				checkHole(memory, hole, hole_spot);
				int tmp_index = worstFit(hole, sleep[i], hole_spot);
				if (tmp_index != -1) {
					int original_index = sleep[i][1];
					last[original_index] = prosessIn(plist[original_index], memory, tmp_index, original_index);
					optime[original_index] = time + plist[original_index][1];
					p_index[original_index] = tmp_index;

				}
				else {
					tmp_sleep.push_back(sleep[i]);
				}
			}
			sleep = tmp_sleep;

		}


		if (allDone(optime))
			break;

	}
	line = to_string(last[n - 1]);
	fout.write(line.c_str(), line.size());
	fout.write(space.c_str(), space.size());

	fin.close();
	fout.close();
	return 0;
}