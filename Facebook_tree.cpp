#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <vector>

using namespace std;
const unsigned long numVertices = 4039; //vertex numbering goes from 0 to 4038
const unsigned long numEdges = 88234; //umber of lines in dataset, will be inly used to display progress bar

const char filePath[] = "assignment 2/disjoint dataset/facebook_combined.txt";

class disjointSetUnion
{
    vector<int> parent;
    vector<int> size;

public:
    disjointSetUnion();
    int find_set(int x);
    void setUnion(int x, int y);
};

disjointSetUnion::disjointSetUnion()
{
    parent = vector<int>(numVertices);

    for (int i = 0; i < parent.size(); i++)
        parent[i] = i;

    size = vector<int>(numVertices, 1);
}

int disjointSetUnion::find_set(int x)
{
    if (parent[x] != x)
        parent[x] = find_set(parent[x]);

    return parent[x];
}

void disjointSetUnion::setUnion(int x, int y)
{
    int parX = find_set(x);
    int parY = find_set(y);

    if (parX != parY)
    {
        if (size[x] < size[y])
            swap(x, y);

        // now confirmed x is the bigger tree

        parent[y] = x;
        size[x] += size[y];
    }
}

pair<int, int> getNextEdge(ifstream &fin)
{
    if (!fin.eof())
    {
        char line[100];
        fin.getline(line, 100);

        if (line[0] == '#')
        {
            while (line[0] == '#')
                fin.getline(line, 100);

            stringstream str_strm;
            str_strm << line; //convert the string s into stringstream

            char num[10];

            str_strm >> num;
            int val1 = atoi(num);
            str_strm >> num;
            int val2 = atoi(num);

            return make_pair(val1, val2);
        }
    }
}

int main()
{
    float start_time = clock();

    disjointSetUnion d;

    ifstream fin(filePath, ios::in);

    pair<int, int> edge;
    for (int i = 0; i < numEdges; i++)
    {
        edge = getNextEdge(fin);
        d.setUnion(edge.first, edge.second);

        cout << (float)i / numEdges * 100 << "% operation done \r";
    }

    fin.close();

    float end_time = clock();

    float time_taken = (end_time - start_time) * 1000 / CLOCKS_PER_SEC;
    cout << "Time Taken: " << time_taken << "ms" << endl;
}