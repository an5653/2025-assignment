#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class Page {
private:
    int* pros;
    int* intime;
    int size;
    int hit = 0;
    int fault = 0;
public:
    Page(int n) {

        size = n;
        pros = new int[size];
        intime = new int[size];
        for (int i = 0; i < size; ++i) {
            pros[i] = -1;
            intime[i] = -1;
        }
    }

    void setPros(int index, int value) {
        if (index >= 0 && index < size) {
            pros[index] = value;
        }
        else {
            cout << "setpros: 잘못된 인덱스입니다: " << index << endl;
        }
    }

    void setIntime(int index, int value) {
        if (index >= 0 && index < size) {
            intime[index] = value;
        }
        else {
            cout << "setArray2: 잘못된 인덱스입니다: " << index << endl;
        }
    }

    int getPros(int index) const {
        if (index >= 0 && index < size) {
            return pros[index];
        }
        else {
            cout << "getPros: 잘못된 인덱스입니다: " << index << endl;
            return -1; // 오류 표시용 값
        }
    }

    int getIntime(int index) const {
        if (index >= 0 && index < size) {
            return intime[index];
        }
        else {
            cout << "getArray2: 잘못된 인덱스입니다: " << index << endl;
            return -1; // 오류 표시용 값
        }
    }

    void printArrays() const {
        cout << "Pros: ";
        for (int i = 0; i < size; ++i)
            cout << pros[i] << " ";
        cout << "\nIntime: ";
        for (int i = 0; i < size; ++i)
            cout << intime[i] << " ";
        cout << endl;
    }
    int isBlank() {
        for (int i = 0; i < size; i++)
        {
            if (getPros(i) == -1)
            {
                return i;
            }

        }
        return -1;
    }
    void setAll(int index, int num, int time) {
        if (index >= 0 && index < size) {
            pros[index] = num;
            intime[index] = time;
        }
        else {
            cout << "setAll: 잘못된 인덱스입니다: " << index << endl;

        }
    }
    int checkHit(int num) {
        for (int i = 0; i < size; i++)
        {
            if (num == pros[i])
            {
                return i;
            }
        }
        return -1;
    }
    int Fifo() {
        int min = intime[0];
        int mindex = 0;
        for (int i = 0; i < size; i++)
        {
            if (min > intime[i])
            {
                min = intime[i];
                mindex = i;
            }
        }
        return mindex;
    }
    void pageClear() {
        for (int i = 0; i < size; ++i) {
            pros[i] = -1;
            intime[i] = -1;
        }
        hit = 0;
        fault = 0;
    }
    void intimeClear() {
        for (int i = 0; i < size; ++i) {
            intime[i] = 0;
        }
    }
    void hitUp() {
        hit++;
    }
    void faultUp() {
        fault++;
    }
    int getHit() {
        return hit;
    }
    int getFault() {
        return fault;
    }
    void Opt(int index, int num, int* list) {
        for (int i = 0; i < size; i++)
        {
            int count = 0;
            while (true)
            {
                if (pros[i] == list[index + count])
                {
                    intime[i] = count;
                    break;
                }
                count++;
                if (count == num)
                {
                    intime[i] = num;
                    break;
                }
            }

        }
    }
    int Opt() {
        int max = intime[0];
        int mindex = 0;
        for (int i = 0; i < size; i++)
        {
            if (max < intime[i])
            {
                max = intime[i];
                mindex = i;
            }
        }
        return mindex;
    }
    ~Page() {
        delete[] pros;
        delete[] intime;
    }
};

int countFault(int* hit, int index) {
    int count = 0;
    for (int i = 0; i < index; i++)
    {
        if (hit[i] == 0)
        {
            count++;
        }
    }
    return count;
}
int list[10000];

int main() {

    int n;
    ifstream fin;
    fin.open("page.inp");
    ofstream fout;
    fout.open("page.out");
    string line;
    getline(fin, line);
    n = stoi(line);
    int size = n;
    int index = 0;

    while (true)
    {
        getline(fin, line);
        n = stoi(line);
        if (n == -1)
            break;
        list[index] = n;

        index++;
    }
    Page page(size);

    for (int i = 0; i < index; i++)
    {
        int tmp = page.isBlank();
        if (tmp != -1)
        {
            if (page.checkHit(list[i]) != -1)
            {
                page.hitUp();
            }
            else
            {
                page.faultUp();
                page.setAll(tmp, list[i], i);
            }

        }

        else
        {
            if (page.checkHit(list[i]) != -1)
            {
                page.hitUp();
            }

            else
            {
                page.faultUp();
                int tmp2 = page.Fifo();
                page.setAll(tmp2, list[i], i);

            }

        }
    }
    int countFifo = page.getFault();

    page.pageClear();

    for (int i = 0; i < index; i++)
    {
        int tmp = page.isBlank();
        if (tmp != -1)
        {
            if (page.checkHit(list[i]) != -1)
            {
                page.hitUp();
                page.setIntime(page.checkHit(list[i]), i);
            }
            else
            {
                page.faultUp();
                page.setAll(tmp, list[i], i);
            }

        }

        else
        {
            if (page.checkHit(list[i]) != -1)
            {
                page.hitUp();
                page.setIntime(page.checkHit(list[i]), i);
            }

            else
            {
                page.faultUp();
                int tmp2 = page.Fifo();
                page.setAll(tmp2, list[i], i);

            }

        }
    }

    int countLRU = page.getFault();

    page.pageClear();

    for (int i = 0; i < index; i++)
    {
        int tmp = page.isBlank();
        if (tmp != -1)
        {
            if (page.checkHit(list[i]) != -1)
            {
                page.hitUp();
            }
            else
            {
                page.faultUp();
                page.setAll(tmp, list[i], 0);
            }

        }

        else
        {
            if (page.checkHit(list[i]) != -1)
            {
                page.hitUp();
            }

            else
            {
                page.faultUp();
                page.Opt(i, index, list);
                int tmp2 = page.Opt();
                page.setAll(tmp2, list[i], page.getIntime(tmp2));
            }

        }

    }
    int countOPT = page.getFault();

    string result1 = "FIFO: ", result2 = "LRU: ", result3 = "OPT: ";
    line = to_string(countFifo);
    result1 += line;
    result1 += "\n";
    line = to_string(countLRU);
    result2 += line;
    result2 += "\n";
    line = to_string(countOPT);
    result3 += line;


    fout.write(result1.c_str(), result1.size());
    fout.write(result2.c_str(), result2.size());
    fout.write(result3.c_str(), result3.size());

    fin.close();
    fout.close();
    return 0;
}