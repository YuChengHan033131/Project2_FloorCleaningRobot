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
    //testing function
    void print()
    {
        for (int i = 0; i < H; i++)
        {
            for (int j = 0; j < W; j++)
            {
                if (this->_data(i, j) ==2)
                {
                    cout << " R";
                }
                else
                {   
                    if(this->_data(i, j) >=0) cout << " " ;
                    cout << this->_data(i, j) ;
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
        int _h(){
            return h;
        }
        int _w(){
            return w;
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
    int size(){
        Node *p=head;
        int size=0;
        while(p!=NULL){
            size++;
            p=p->_next();
        }
        return size;
    }
    void printAll(){
        Node *p=head;
        int steps=0;
        while(p!=NULL){
            steps++;
            cout << p->_h() << " " << p->_w() << endl ;
            p=p->_next();
        }
    }
        
};
queue q;
class Robot{
    private:
        int h;
        int w;
        int battery;
    public:
        Robot(int initialH,int initialW,int battry):h(initialH),w(initialW),battery(battery){
            q.push(new Node(h,w));
        }
        bool walking(char direction){
            switch (direction)
            {
            case 'U':
                h--;
                if(field->_data(h,w)==1){
                    //robot can't walk to there
                    h++;
                    return false;
                }
                break;
            case 'R':
                w++;
                if(field->_data(h,w)==1){
                    //robot can't walk to there
                    w--;
                    return false;
                }
                break;
            case 'D':
                h++;
                if(field->_data(h,w)==1){
                    //robot can't walk to there
                    h--;
                    return false;
                }
                break;
            case 'L':
                w--;
                if(field->_data(h,w)==1){
                    //robot can't walk to there
                    w++;
                    return false;
                }
                break;
            default:
                cout << "robot moving error";
                break;
            }
            q.push(new Node(h,w));
            if(field->_data(h,w)==2){
                field->setData(h,w,field->_data(h,w));
            }else{
                field->setData(h,w,field->_data(h,w)-1);
            }
            return true;
        }
        bool goUp(){
            return walking('U');
        }
        bool goRight(){
            return walking('R');
        }
        bool goDown(){
            return walking('D');
        }
        bool goLeft(){
            return walking('L');
        }
};
Robot *bob;
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
                bob=new Robot(i,j,battery);
            }
            else
            {
                field->setData(i, j, temp - '0');
            }
        }
    }

    //testing
    bob->goRight();
    bob->goDown();
    bob->goLeft();
    bob->goUp();
    field->print();
    cout << q.size() << endl ;
    q.printAll();
    


    //algorithm 1
    
    
    


}