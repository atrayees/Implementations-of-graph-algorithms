#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>

using namespace std;

const unsigned long num_vertex = 4039;
const unsigned long num_edges = 88234;

const char file_path[] = "assignment 2/disjoint dataset/facebook_combined.txt";

class CC;
class Node;
class Edge;

class Node {
public:
    Node* next;
    CC* rep;
    unsigned long data;

    Node(unsigned long data) {
        this->data = data;
        next = nullptr;
        rep = nullptr;
    }

    friend class UnionFind;
};

class Edge {
private:
    unsigned long value1;
    unsigned long value2;
public:
    Edge() {
        value1 = 0;
        value2 = 0;
    }

    friend class UnionFind;
};

class CC {
public:
    Node* head;
    Node* tail;
    CC* next;
    CC* prev;
    unsigned long len;

    CC() {
        head = nullptr;
        tail = nullptr;
        next = nullptr;
        prev = nullptr;
        len = 0;
    }
    
    friend class UnionFind;
};

class UnionFind {
private:
    CC start;

public:

    ~UnionFind() {
        CC* temp = &start;
        while (temp != nullptr) {
            CC* next = temp->next;
            Node* node_temp = temp->head;
            while (node_temp != nullptr) {
                Node* next = node_temp->next;
                delete node_temp;
                node_temp = next;
            }
            delete temp;
            temp = next;
        }
    }

    void make_set(unsigned long x) {
        CC* temp = start.next;

        CC* new_CC = new CC();
        new_CC->len = 1;
        new_CC->next = temp;
        start.next = new_CC;
        new_CC->prev = &start;

        if (temp != nullptr)
            temp->prev = new_CC;

        new_CC->head = new Node(x);
        new_CC->tail = new_CC->head;
        new_CC->head->rep = new_CC;
    }

    CC* find_set(unsigned long val) {
        CC* CC_iterator = start.next;

        while (CC_iterator != nullptr) {
            Node* node_iterator = CC_iterator->head;

            while (node_iterator != nullptr) {
                if (node_iterator->data == val)
                    return node_iterator->rep;

                node_iterator = node_iterator->next;
            }
            CC_iterator = CC_iterator->next;
        }
        return nullptr;
    }

    void link(CC* x, CC* y) {
        if (x != y) {
            CC* max, * min;

            if (x->len > y->len) {
                max = x;
                min = y;
            }
            else {
                max = y;
                min = x;
            }

            max->len += min->len;

            Node* changin_pt = min->head;

            max->tail->next = min->head;
            max->tail = min->tail;

            while (changin_pt != nullptr) {
                changin_pt->rep = max;
                changin_pt = changin_pt->next;
            }

            if (min->prev != nullptr)
                min->prev->next = min->next;

            if (min->next != nullptr)
                min->next->prev = min->prev;

            delete min;
        }
    }

    void union_op(Edge temp) {
        if (temp.value1 != temp.value2)
        link(find_set(temp.value1), find_set(temp.value2));
    }

    void update_edge(Edge& given_edge, std::ifstream& fin) {
        if (!fin.eof()){
            char line[10];
            fin.getline(line, 10);

            stringstream str_strm;
            str_strm << line; //convert the string s into stringstream

            char num[5];

            str_strm >> num;
            given_edge.value1 = atoi(num);

            str_strm >> num;
            given_edge.value2 = atoi(num);

            // cout << given_edge.value1 << " " << given_edge.value2 << endl;
        }
    }
};

unsigned int num_iter = 10;

int main()
{
    float avg_time = 0;

    for (int master = 0; master < num_iter; master++)
    {
        float start_time = clock();

        UnionFind base;

        for (int i = 0; i < num_vertex; i++)
            base.make_set(i);

        ifstream fin(file_path, ios::in);

        Edge temp;
        for (int i = 0; i < num_edges; i++)
        {
            base.update_edge(temp, fin);
            base.union_op(temp);

            cout << (float)i / (num_edges - 1) * 100 << "% done \r";
        }

        fin.close();

        float end_time = clock();

        float time_taken = (end_time - start_time) * 1000 / CLOCKS_PER_SEC;
        avg_time += time_taken;
        cout << "Time Taken: " << time_taken << "ms" << endl;
    }

    avg_time /= num_iter;
    cout << "Average Time Taken: " << avg_time << "ms" << endl;

    return 0;
}