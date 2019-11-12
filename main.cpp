#include <iostream>
#include <iostream>
#include <fstream>
using namespace std;
class Node{
    private:
        int h;
        int w;
        Node *next=NULL;
    public:
        Node(int h,int w):h(h),w(w){}
        Node(){}
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
        void setData(int h,int w){
            this->h=h;
            this->w=w;
        }
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Field
{
private:
    int *head;//R=-1 wall=1 >1=unclean <-1=clean
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
    int _data(Node *node){
        return head[node->_h() * W + node->_w()];
    }

    //test function
    void print()
    {
        for (int i = 0; i < H; i++)
        {
            for (int j = 0; j < W; j++)
            {
                if (this->_data(i, j) ==-1)
                {
                    cout << " R";
                }
                else
                {   
                    if(this->_data(i, j) >=0) cout << " " ;
                    cout << this->_data(i, j) ;
                }
            }
            cout << endl ;
        }
        cout << endl;
    }
    void setData(int h, int w, int data)
    {
        head[h * W + w] = data;
    }
    void setData(Node *node,int data){
        head[node->_h() * W + node->_w()] = data;
    }
};
Field *field;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <class T>
class queue{
    private:
        T *head=NULL;
        T *bottom=NULL;
    public:
    T *_head(){
        return head;
    }
    T *_bottom(){
        return bottom;
    }
    void push(T *node){
        if(head!=NULL){
            bottom->setNext(node);
            bottom=node;
        }else{
            head=node;
            bottom=node;
        }
    }
    void pop(){
        head=head->_next();
    }
    int size(){
        T *p=head;
        int size=0;
        while(p!=NULL){
            size++;
            p=p->_next();
        }
        delete(p);
        return size;
    }
    void printAll(){
        T *p=head;
        int steps=0;
        while(p!=NULL){
            steps++;
            cout << p->_h() << " " << p->_w() << endl ;
            p=p->_next();
        }
        delete(p);
    }
};
queue <Node> q;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Robot{
    private:
        int h;
        int w;
        int direction=0;//the direction robot heading.0=left,1=up,2=right,3=down
        int battery;
        int fullBattery;
    public:
        Robot(int initialH,int initialW,int battery):h(initialH),w(initialW),battery(battery),fullBattery(battery){
            q.push(new Node(h,w));
        }
        void findNearestDirtBlock(){
            cout << "findNearestDirtBlock" << endl;
        }
        void moveOneStep(){
            direction=(direction+3)%4;
            for(int i=0;i<4;i++){
                int _h=direction%2*(direction-2),_w=(direction+1)%2*(direction-1);//amount of left offset according to direction 
                if(field->_data(h+_h,w+_w)==0){//should change to >1
                    //can walk
                    h+=_h;
                    w+=_w;
                    q.push(new Node(h,w));
                    if(field->_data(h,w)!=-1){
                        battery--;
                        field->setData(h,w,-2+field->_data(h,w)*-1);//-2 mush delete
                    }else{
                        battery=fullBattery;
                    }
                    return;
                }
                direction=(direction+1)%4;
            }
            //no dirty block to walk
            findNearestDirtBlock();
        }
        //test function 
        void jump(int h,int w){
            this->h=h;
            this->w=w;
        }
};
Robot *bot;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class maxheap{
    private:
        Node **head;
        int size=0;
    public:
    maxheap(){}
    maxheap(int size){
        head=new Node* [size];
        for(int i=0;i<size;i++){
            head[i]=NULL;
        }
    }
    void insert(Node *node){
        head[size]=node;
        size++;
    }
    void heapify(){
        for(int i=size/2-1;i>=0;i--){
            int largerIndex=-1;
            if(i*2+1<size&&i*2+2>=size){
                if(field->_data(head[i])<field->_data(head[i*2+1])){
                    largerIndex=i*2+1;
                }
            }else if(i*2+1<size&&i*2+2<size){
                bool i_smaller_l=field->_data(head[i])<field->_data(head[i*2+1]),
                     i_smaller_r=field->_data(head[i])<field->_data(head[i*2+2]),
                     l_smaller_r=field->_data(head[i*2+1])<field->_data(head[i*2+2]);
                    if(i_smaller_r||i_smaller_l){
                        if(l_smaller_r){
                            largerIndex=i*2+2;
                        }else{
                            largerIndex=i*2+1;
                        }
                    }
            }
            else{
                continue;
            }
            if(largerIndex!=-1){
                Node *temp=head[i];
                head[i]=head[largerIndex];
                head[largerIndex]=temp;
            }
        }
    }
    int root(){
        return field->_data(head[0]);
    }
    void pop(){//delete root
        if(size<1){
            return ;
        }
        head[0]=head[size-1];
        head[size-1]=NULL;
        size--;
        heapify();
    }
    //test function
    void print(){//from big to small .will delete maxheap
        int n=size;
        heapify();
        for(int i=0;i<n;i++){
            cout << root() << " " ;
            pop();
        }
    }
    void printMatrix(){
        for(int i=0;i<size;i++){
            cout << field->_data(head[i]);
        }
        cout << endl;
        
    }
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main()
{
    //input testcase
    
    int H, W, battery,WalkableBlockNumbers=0;
    Node *charger;
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
                field->setData(i, j, -1);
                bot=new Robot(i,j,battery);
                WalkableBlockNumbers++;
                charger=new Node(i,j);
            }
            else
            {
                field->setData(i, j, temp - '0');
                if(temp=='0'){    
                    WalkableBlockNumbers++;
                }
            }
        }
    }
    
    //algorithm 1
    //counting all dsitance to charger(by BFS) and enter maxheap
    queue <Node> BFS_q;
    maxheap m(WalkableBlockNumbers);
    BFS_q.push(charger);
    field->setData(charger,1);//set R=1 temporary 
    while(BFS_q._head()!=NULL){
        Node *temp=BFS_q._head(),*temp2;
        m.insert(temp);
        BFS_q.pop();
        //visit left
        temp2= new Node(temp->_h(),temp->_w()-1);
        if(field->_data(temp2)==0){//0 means unvisited
            field->setData(temp2,field->_data(temp)+1);
            BFS_q.push(temp2);
        }
        //visit up
        temp2= new Node(temp->_h()-1,temp->_w());
        if(field->_data(temp2)==0){//0 means unvisited
            field->setData(temp2,field->_data(temp)+1);
            BFS_q.push(temp2);
        }
        //visit right
        temp2= new Node(temp->_h(),temp->_w()+1);
        if(field->_data(temp2)==0){//0 means unvisited
            field->setData(temp2,field->_data(temp)+1);
            BFS_q.push(temp2);
        }
        //visit down
        temp2= new Node(temp->_h()+1,temp->_w());
        if(field->_data(temp2)==0){//0 means unvisited
            field->setData(temp2,field->_data(temp)+1);
            BFS_q.push(temp2);
        }
    }
    field->setData(charger,-1);//recover R=-1




    //testing
    field->print();
    m.print();

    /*cout << q.size() << endl ;
    q.printAll();*/
    


}