#include <iostream>
#include <iostream>
#include <fstream>
using namespace std;
class Field
{
private:
    int *head;
    int H;
    int W;

public:
    Field(int H, int W) : H(H), W(W)
    {
        head = new int[H * W];
    }
    int _data(int h, int w)
    {
        return head[h * W + w];
    }
    void print()
    {
        for (int i = 0; i < H; i++)
        {
            for (int j = 0; j < W; j++)
            {
                if (this->_data(i, j) == -1)
                {
                    cout << "R";
                }
                else
                {
                    cout << this->_data(i, j);
                }
            }
            cout << endl;
        }
    }
    void setData(int h, int w, int data)
    {
        head[h * W + w] = data;
    }
};
Field *field;
int main()
{
    //input testcase
    int H, W, battry;
    ifstream inf("floor.data");
    if (!inf)
    {
        cout << "file open failed";
        return 0;
    }
    inf >> H >> W >> battry;
    field = new Field(H, W);
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            char temp;
            inf >> temp;
            if (temp == 'R')
            {
                field->setData(i, j, -1);
            }
            else
            {
                field->setData(i, j, temp - '0');
            }
        }
    }

    //testing
    field->print();
}