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
                if (this->_data(i, j) ==2)
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
class Node{
    private:
        int h;
        int w;
        Node *next=NULL;
    public:
        Node(int h,int w):h(h),w(w){}
        Node *_next(){
            return next;
        }
        void setNext(Node *next){
            this->next=next;
        }
};
class queue{
    private:
        Node *head=NULL;
        Node *bottom=NULL;
    public:
    Node *_head(){
        return head;
    }
    Node *_bottom(){
        return bottom;
    }
    void push(Node *node){
        if(head!=NULL){
            bottom->setNext(node);
            bottom=node;
        }else{
            head=node;
            bottom=node;
        }
    }
        
};
class Robot{
    private:
        int h;
        int w;
        int battery;
    public:
        Robot(int initialH,int initialW,int battry):h(initialH),w(initialW),battery(battery){}
};
Field *field;
int main()
{
    //input testcase
    int H, W, battery;
    ifstream inf("floor.data");
    if (!inf)
    {
        cout << "file open failed";
        return 0;
    }
    inf >> H >> W >> battery;
    field = new Field(H, W);
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            char temp;
            inf >> temp;
            if (temp == 'R')
            {
                field->setData(i, j, 2);
                Robot bob(i,j,battery);

            }
            else
            {
                field->setData(i, j, temp - '0');
            }
        }
    }

    //testing
    field->print();

    //algorithm 1
    queue q;
    
    


}