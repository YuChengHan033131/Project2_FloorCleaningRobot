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
                    if(this->_data(i, j)<10 && this->_data(i, j)>0) cout << " " ;
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
class queue{
    private:
        Node **head;
        int _front=-1;
        int _back=-1;
        int _size;
    public:
    queue(){};
    queue(int size):_size(size){
        head = new Node* [_size];
        for(int i=0;i<_size;i++){
            head[i]=NULL;
        }
    }
    void setSize(int size){
        _size=size;
        head = new Node* [_size];
        for(int i=0;i<_size;i++){
            head[i]=NULL;
        }
    }
    Node *front(){
        if(_front==-1){
            cout << "error! queue is empty" ;
        }
        return head[_front];
    }
    Node *back(){
        if(_front==-1){
            cout << "error! queue is empty" ;
        }
        return head[_back];
    }
    void push(Node *node){
        if(_front==-1){
            _front=0;
            _back=0;
        }else if((_back+1)%_size!=_front){
            _back=(_back+1)%_size;
        }else{
            cout << "queue is full! " ;
            return;
        }
        head[_back]=node;
    }
    void pop(){
        if(_front==-1){
            cout << "error!nothing to pop" << endl;
        }
        else if(_front==_back){
            _front=-1;
            _back=-1;
        }else{
            _front=(_front+1)%_size;
        }
    }
    int size(){
        if(_front==-1){
            return 0;
        }else if(_front<=_back){
            return _back-_front+1;
        }else{
            return _back-_front+_size+1;
        }
       
    }
    void printAll(){
        if(_front==-1){
            cout << "error! nothing to print" << endl ;
            return ;
        }
       int temp=_front;
       while(1){
           cout << head[temp]->_h() << " " << head[temp]->_w() << endl ;
           temp=(temp+1)%_size;
           if(temp==(_back+1)%_size){
               break;
           }
       }
    }
};
queue q;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Robot{
    private:
        int h;
        int w;
        int direction=0;//the direction robot heading.0=left,1=up,2=right,3=down
        int battery;
        int fullBattery;
    public:
        Robot(int initialH,int initialW,int battery):h(initialH),w(initialW),battery(battery),fullBattery(battery){        }
        void findNearestDirtBlock(){
            cout << "findNearestDirtBlock" << endl;
        }
        void moveOneStep(){
            direction=(direction+3)%4;
            for(int i=0;i<4;i++){
                int _h=direction%2*(direction-2),_w=(direction+1)%2*(direction-1);//amount of left offset according to direction 
                if(field->_data(h+_h,w+_w)>1){
                    //can walk
                    h+=_h;
                    w+=_w;
                    q.push(new Node(h,w));
                    if(field->_data(h,w)!=-1){
                        battery--;
                        field->setData(h,w,field->_data(h,w)*-1);
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
        void findShortestPath(Node *to,Node *from){

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
    q.setSize(WalkableBlockNumbers);
    queue BFS_q(WalkableBlockNumbers);
    q.push(charger);
    maxheap m(WalkableBlockNumbers);
    BFS_q.push(charger);
    field->setData(charger,1);//set R=1 temporary 
    while(BFS_q.size()!=0){
        Node *temp=BFS_q.front(),*temp2;
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

    //move to afrest block in shortest path


    //testing
    field->print();
    for(int i=0;i<10;i++){
        bot->moveOneStep();
    }
    field->print();
    q.printAll();
    m.print();

    /*cout << q.size() << endl ;
    q.printAll();*/
    


}