#include <iostream>
#include <iostream>
#include <fstream>
using namespace std;
int abs(int num){
    if(num<0){
        return num*-1;
    }else{
        return num;
    }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Tile{
    private:
        bool cleaned;
        int data;//cleaness: 1=wall,smaller the number is, cleaner the floor is
        int distance=0;
        int h;
        int w;
        Tile *previous=NULL;
    public:
        Tile(){}
        int _data(){
            return data;
        }
        void setup(int h,int w,int data){
            this->h=h;
            this->w=w;
            if(data>1){
                this->data=0;
                cleaned=false;
            }else if(data<-1||data==-1){
                this->data=-1;
                cleaned=true;
            }else{
                this->data=1;
            }
        }
        void setPrevious(Tile *node){
            previous=node;
        }
        void setData(int data){
            this->data=data;
        }
        void setDistance(int distance){
            this->distance=distance;
        }
        void updateData(Tile *node){
            //ever visited
            if(distance<node->distance+1){
                return;
            }else if(distance==node->distance+1){
                if(data<node->data){//means data is cleaner than node->data
                    data=node->data+(cleaned?-1:0);
                    previous=node;
                }
            }else{//distance is smaller
                distance=node->distance+1;
                previous=node;
            }
        }
        int _h(){
            return h;
        }
        int _w(){
            return w;
        }
        Tile *_previous(){
            return previous;
        }
        int _distance(){
            return distance;
        }
        bool _cleaned(){
            return cleaned;
        }
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class NodeB{
    private:
        int h;
        int w;
        bool visited=false;
    public:
        NodeB(int h,int w):h(h),w(w){}
        NodeB(Tile *tile){
            h=tile->_h();
            w=tile->_w();
        }
        NodeB(){}
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
        void setVisited(bool visited){
            this->visited=visited;
        }
        bool _visited(){
            return visited;
        }
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Node{
    private:
        int h;
        int w;
    public:
        Node(int h,int w):h(h),w(w){}
        Node(Tile *tile){
            h=tile->_h();
            w=tile->_w();
        }
        Node(){}
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
    Field(Field *field) : H(field->H), W(field->W){
        head = new int[H * W];
    }
    int _data(int h, int w)
    {
        return head[h * W + w];
    }
    int _data(int i)
    {
        return head[i];
    }
    int _data(Node *node){
        return head[node->_h() * W + node->_w()];
    }
    int _data(NodeB *node){
        return head[node->_h() * W + node->_w()];
    }
    int _H(){
        return H;
    }
    int _W(){
        return W;
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
                    cout << "  R";
                }
                else
                {   
                    if(this->_data(i, j)<10 && this->_data(i, j)>0) cout << " " ;
                    cout << " " << this->_data(i, j) ;
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
        T **head;
        int _front=-1;
        int _back=-1;
        int _size;
    public:
    queue(){};
    queue(int size):_size(size){
        head = new T* [_size];
        for(int i=0;i<_size;i++){
            head[i]=NULL;
        }
    }
    void setSize(int size){
        _size=size;
        head = new T* [_size];
        for(int i=0;i<_size;i++){
            head[i]=NULL;
        }
    }
    T *front(){
        if(_front==-1){
            cout << "error! queue is empty" ;
        }
        return head[_front];
    }
    T *back(){
        if(_front==-1){
            cout << "error! queue is empty" ;
        }
        return head[_back];
    }
    void push(T *node){
        if(_front==-1){
            _front=0;
            _back=0;
        }else if((_back+1)%_size!=_front){
            _back=(_back+1)%_size;
        }else{
            increaseSize();
            _back=(_back+1)%_size;
        }
        head[_back]=node;
    }
    void increaseSize(){
        _size*=5/4+1;
        T **newHead = new T* [_size];
        for(int i=0;i<_size;i++){
            if(size()!=0){
                newHead[i]=front();
                pop();
            }else{
                _front=0;
                _back=i-1;
                break;
            }
        }
        delete(head);
        head=newHead;
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
    void pull(){
        if(_front==-1){
            cout << "error!nothing to pull" << endl;
        }
        else if(_front==_back){
            _front=-1;
            _back=-1;
        }else{
            _back=(_back+_size-1)%_size;
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
    void reset(){
        delete(head);
        _front=-1;
        _back=-1;
        delete(_size);
    }
    void destroy(){
        while(_front!=-1){
            T *temp=head[_front];
            pop();
            delete(temp);
        }
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
        int houseSize;
        int cleanedBlock=0;
        bool power=true;
        Node *charger;
    public:
        Robot(int initialH,int initialW,int battery):h(initialH),w(initialW),battery(battery),fullBattery(battery){
            charger=new Node(initialH,initialW);
        }
        int _battery(){
            return battery;
        }
        void setHouseSize(int houseSize){
            this->houseSize=houseSize;
        }
        bool isNotDone(){
            if(cleanedBlock<houseSize){
                return true;
            }else if(cleanedBlock==houseSize){
                return false;
            }else{
                cout << "error!cleanedBlock is largger than houseSize" << endl ;
                return false;
            }
        }
        void knockedOff(){//back to charger
            moveTo(charger);
            power=false;
        }
        bool powerIsOn(){
            return power;
        }
        bool moveToNearestDirtyBlock(){//by BFS
            //copy field
            Field *tempfield=new Field(field);
            for(int i=0;i<field->_H();i++){
                for(int j=0;j<field->_W();j++){
                    if(field->_data(i,j)!=1){
                        tempfield->setData(i,j,0);
                    }else{
                        tempfield->setData(i,j,1);
                    }
                }
            }
            queue <Node> tempQ(houseSize);
            Node *temp=new Node(h,w),*largest=NULL;
            tempfield->setData(temp,2);//>1 visited 0=unvisted
            tempQ.push(temp);
            bool found=false;
            while(tempQ.size()!=0){
                temp=tempQ.front();
                tempQ.pop();
                //if find dirty block
                if(field->_data(temp)>1){
                    found=true;
                    if(largest!=NULL){
                        if(tempfield->_data(temp)>=tempfield->_data(largest)){//farthest block from start
                            if(field->_data(largest)<field->_data(temp)){//farthest block from charger
                                largest=temp;
                            }
                        }
                    }else{
                        largest=temp;
                    }
                    
                    largest=temp;
                }
                //visit left
                if(found==false&&tempfield->_data(temp->_h(),temp->_w()-1)==0){//means unvisited and not wall
                    tempQ.push(new Node(temp->_h(),temp->_w()-1));
                    tempfield->setData(temp->_h(),temp->_w()-1,tempfield->_data(temp)+1);
                }
                //visit up
                if(found==false&&tempfield->_data(temp->_h()-1,temp->_w())==0){//means unvisited and not wall
                    tempQ.push(new Node(temp->_h()-1,temp->_w()));
                    tempfield->setData(temp->_h()-1,temp->_w(),tempfield->_data(temp)+1);
                }
                //visit right
                if(found==false&&tempfield->_data(temp->_h(),temp->_w()+1)==0){//means unvisited and not wall
                    tempQ.push(new Node(temp->_h(),temp->_w()+1));
                    tempfield->setData(temp->_h(),temp->_w()+1,tempfield->_data(temp)+1);
                }
                //visit down
                if(found==false&&tempfield->_data(temp->_h()+1,temp->_w())==0){//means unvisited and not wall
                    tempQ.push(new Node(temp->_h()+1,temp->_w()));
                    tempfield->setData(temp->_h()+1,temp->_w(),tempfield->_data(temp)+1);
                }
            }
            if(found){
                if(!moveTo(largest)){//no enough battery
                    return false;
                }
                return true;
            }else{
                knockedOff();
                return false;
            }

        }
        /*void moveToNearestDirtyBlock(){//by BFS
            //find nearest dirty block
            NodeB *temp,*temp0,*temp1,*temp2,*temp3;
            Node *largest=NULL;
            queue <NodeB> tempQ(houseSize);
            tempQ.push(new NodeB(h,w));
            while(tempQ.size()!=0){
                temp=tempQ.front();
                temp->setVisited(true);
                //chenck if charger is in wall

                tempQ.pop();
                //visit left
                temp0=new NodeB(temp->_h(),temp->_w()-1);
                if(field->_data(temp0)!=1&&temp0->_visited()==false){
                    tempQ.push(temp0);
                    temp0->setVisited(true);
                }
                //visit up
                temp1=new NodeB(temp->_h()-1,temp->_w());
                if(field->_data(temp1)!=1&&temp1->_visited()==false){
                    tempQ.push(temp1);
                    temp1->setVisited(true);
                }
                //visit right
                temp2=new NodeB(temp->_h(),temp->_w()+1);
                if(field->_data(temp2)!=1&&temp2->_visited()==false){
                    tempQ.push(temp2);
                    temp2->setVisited(true);
                }
                //visit down
                temp3=new NodeB(temp->_h()+1,temp->_w());
                if(field->_data(temp3)!=1&&temp3->_visited()==false){
                    tempQ.push(temp3);
                    temp3->setVisited(true);
                }

                //find farest dirty block 
                NodeB *direction[4]={temp0,temp1,temp2,temp3};
                for(int i=0;i<4;i++){
                    if(field->_data(direction[i])>1){
                        if(largest==NULL||field->_data(largest)<field->_data(direction[i])){
                            largest=new Node(direction[i]->_h(),direction[i]->_w());
                        }
                    }
                }
                if(largest!=NULL){//found dirty block
                    break;
                }
            }
            //temp0=nearest dirty block
            if(tempQ.size()!=0){
                moveTo(largest);
            }else{//no more dirty block
                knockedOff();
            }
        }*/
        bool moveOneStepTo(Node *node){
            bool currentGridIsNotCleaned=false;
            int tempH=node->_h(),tempW=node->_w();
            if(field->_data(node)!=1&&
               ((tempH==h&&tempW==w-1)||//node is reachable and walkable
               (tempH==h-1&&tempW==w)||
               (tempH==h&&tempW==w+1)||
               (tempH==h+1&&tempW==w)))
            {
                h=tempH;
                w=tempW;
                q.push(node);
                if(field->_data(h,w)!=-1){
                    battery--;
                    if(field->_data(h,w)>1){//unclean
                        field->setData(h,w,field->_data(h,w)*-1);
                        currentGridIsNotCleaned=true;
                        cleanedBlock++;
                    }
                }else{
                        battery=fullBattery;
                }
            }else{
                cout << "error!robot can't goes there." << endl ;
            }
            if(batteryNotEnough()){
                //undo
                battery++;
                if(currentGridIsNotCleaned){
                    field->setData(q.back(),field->_data(q.back())*-1);
                }
                q.pull();
                h=q.back()->_h();
                w=q.back()->_w();
                return false;
            }else{
                return true;
            }
        }
        void moveOneStepAlongRight(){
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
                        if(field->_data(h,w)>1){//unclean
                            field->setData(h,w,field->_data(h,w)*-1);
                            cleanedBlock++;
                        }
                    }else{
                        battery=fullBattery;
                    }
                    return;
                }
                direction=(direction+1)%4;
            }
            //no dirty block to walk
            moveToNearestDirtyBlock();
        }
        queue <Node> *findShortestPath(Node *_from,Node *_to){//by BFS 
            //construct a matrix "similar" to field
           Tile **grid=new Tile* [field->_H()];
           for(int i=0;i<field->_H();i++){
               grid[i]=new Tile[field->_W()];
           }
           for(int i=0;i<field->_H();i++){
               for(int j=0;j<field->_W();j++){
                   grid[i][j].setup(i,j,field->_data(i,j));
               }
           }
            Tile *from=&grid[_from->_h()][_from->_w()];
            from->setPrevious(from);
            //counting path beside destination
            int count=0,paths=((grid[_to->_h()][_to->_w()-1]._data()==1)?0:1)+
                              ((grid[_to->_h()-1][_to->_w()]._data()==1)?0:1)+
                              ((grid[_to->_h()][_to->_w()+1]._data()==1)?0:1)+
                              ((grid[_to->_h()+1][_to->_w()]._data()==1)?0:1);

           //start finding
           queue <Tile> q(houseSize);
           q.push(from);
           while(q.size()!=0){
               Tile *temp=q.front(),*temp2;
                q.pop();
                //visit left
                temp2=&grid[temp->_h()][temp->_w()-1];
                if(temp2->_data()!=1){//can walk
                    if(temp2->_previous()==NULL){//never visited
                        temp2->setDistance(temp->_distance()+1);
                        temp2->setData(temp->_data()+(temp2->_cleaned()?-1:0));
                        temp2->setPrevious(temp);
                        q.push(temp2);
                    }else{//ever walk
                        temp2->updateData(temp);
                    }
                    if(temp2->_h()==_to->_h()&&temp2->_w()==_to->_w()){
                        count++;
                    }
                }
                //visit up
                temp2=&grid[temp->_h()-1][temp->_w()];
                if(temp2->_data()!=1){//can walk
                    if(temp2->_previous()==NULL){//never visited
                        temp2->setDistance(temp->_distance()+1);
                        temp2->setData(temp->_data()+(temp2->_cleaned()?-1:0));
                        temp2->setPrevious(temp);
                        q.push(temp2);
                    }else{//ever walk
                        temp2->updateData(temp);
                    }
                    if(temp2->_h()==_to->_h()&&temp2->_w()==_to->_w()){
                        count++;
                    }
                }
                //visit right
                temp2=&grid[temp->_h()][temp->_w()+1];
                if(temp2->_data()!=1){//can walk
                    if(temp2->_previous()==NULL){//never visited
                        temp2->setDistance(temp->_distance()+1);
                        temp2->setData(temp->_data()+(temp2->_cleaned()?-1:0));
                        temp2->setPrevious(temp);
                        q.push(temp2);
                    }else{//ever walk
                        temp2->updateData(temp);
                    }
                    if(temp2->_h()==_to->_h()&&temp2->_w()==_to->_w()){
                        count++;
                    }
                }
                //visit down
                temp2=&grid[temp->_h()+1][temp->_w()];
                if(temp2->_data()!=1){//can walk
                    if(temp2->_previous()==NULL){//never visited
                        temp2->setDistance(temp->_distance()+1);
                        temp2->setData(temp->_data()+(temp2->_cleaned()?-1:0));
                        temp2->setPrevious(temp);
                        q.push(temp2);
                    }else{//ever walk
                        temp2->updateData(temp);
                    }
                    if(temp2->_h()==_to->_h()&&temp2->_w()==_to->_w()){
                        count++;
                    }
                }
                //find destination enough times
                if(count==paths){
                    break;
                }
            }
            delete(&q);
            queue <Node> *q1=new queue<Node>(houseSize);
            //output path
            Tile *temp=&grid[_to->_h()][_to->_w()];
            while(temp!=from){
                q1->push(new Node(temp));
                temp=temp->_previous();
            }
            delete(grid);
            return q1;
        }
        bool moveTo(Node *_to){
            if(h==_to->_h()&&w==_to->_w()){//destination=start
                return true;
            }
            queue <Node> *tempQ=findShortestPath(new Node(h,w),_to);
            if(battery<tempQ->size()+abs(field->_data(_to))-1){//no enough battery to move to that place and go back
                return false;
            }
            while(tempQ->size()!=0){
                Node *temp=tempQ->back();
                tempQ->pull();
                moveOneStepTo(temp);
            }
            delete(tempQ);
            return true;
        }
        /*int DistanceTo(Node *node){
            queue <Node> *temp=findShortestPath(new Node(h,w),node);
            int dis=temp->size();
            temp->destroy();
            return dis;
        }*/
        bool batteryNotEnough(){//if there are enought battery back to charger
            int temp=field->_data(h,w);
            if(temp<0){
                temp*=-1;
            }
            return temp-1>battery;//since the field value is 1 larger than distance
        }
        void roaming(){//visit unclean & farthest(to charger) near by block
            Node **direction=new Node*[4],*largest;
            while(1){
                if(h==charger->_h()&&w==charger->_w()){//means in the charger
                    return;
                }
                int largestIndex=0;
                direction[0]=new Node(h,w-1);
                direction[1]=new Node(h-1,w);
                direction[2]=new Node(h,w+1);
                direction[3]=new Node(h+1,w);
                largest=direction[0];
                for(int i=1;i<4;i++){
                    if(field->_data(largest)<field->_data(direction[i])){
                        largest=direction[i];
                        largestIndex=i;
                    }
                }
                if(field->_data(largest)>1){//means it is not wall or clean block
                   for(int i=0;i<4;i++){//delete usless node
                        if(i!=largestIndex){
                            delete(direction[i]);
                        }
                    }
                    if(!moveOneStepTo(largest)){//means there are no enough battery to go back,so robot refuse to move that step
                        return;
                    }
                    
                }else{
                    if(!moveToNearestDirtyBlock()){
                        return;
                    }
                }
            }
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
    Node *_rootNode(){
        heapify();
        return head[0];
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
    bot->setHouseSize(WalkableBlockNumbers);
    //algorithm 1
    //counting all dsitance to charger(by BFS) and enter maxheap
    q.setSize(WalkableBlockNumbers);
    queue <Node>BFS_q(WalkableBlockNumbers);
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

    field->print();//test
    //cleaning
    while(bot->powerIsOn()){
        bot->moveTo(m._rootNode());//move to farthest block in shortest path
        field->print();//test
        bot->roaming();
        field->print();//test
        bot->moveTo(charger);
        field->print();//test
    }

    cout << endl ;
    cout << q.size()-1 << endl ;
    q.printAll();
    return 0;
}