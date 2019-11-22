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
class Node{
    private:
        int h;
        int w;
    public:
        Node(int h,int w):h(h),w(w){}
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
    int _H(){
        return H;
    }
    int _W(){
        return W;
    }
    void setData(int h, int w, int data)
    {
        head[h * W + w] = data;
    }
    void setData(Node *node,int data){
        head[node->_h() * W + node->_w()] = data;
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
                    cout << "  R ";
                }
                else
                {   
                    if(_data(i,j)>99){
                        cout << " " ;
                    }else if(_data(i,j)>9){
                        cout << "  " ;
                    }else if(_data(i,j)>0){
                        cout << "   " ;
                    }else if(_data(i,j)>-10){
                        cout << "  " ;
                    }else if(_data(i,j)>-100){
                        cout << " " ;
                    }
                    cout << this->_data(i, j) ;
                }
            }
            cout << endl ;
        }
        cout << endl;
    }
    void check(){
        for (int i = 0; i < H; i++){
            for (int j = 0; j < W; j++){
                if (this->_data(i, j)>1){
                    cout << "not clean!";
                    return;
                }
            }
        }
        cout << "everything went on throughly." ;
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
    void printAll(ofstream &of){
        if(_front==-1){
            cout << "error! nothing to print" << endl ;
            return ;
        }
       int temp=_front;
       while(1){
           of << head[temp]->_h() << " " << head[temp]->_w() << endl ;
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
        delete(&_size);
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
        bool notInWall(int h,int w){
            
            if(h<=0||h>=field->_H()-1||w<=0||w>=field->_W()-1){
                return false;
            }else{
                return true;
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
                if(found==false&&tempfield->_data(temp->_h(),temp->_w()-1)==0&&notInWall(temp->_h(),temp->_w()-1)){//means unvisited and not wall
                    tempQ.push(new Node(temp->_h(),temp->_w()-1));
                    tempfield->setData(temp->_h(),temp->_w()-1,tempfield->_data(temp)+1);
                }
                //visit up
                if(found==false&&tempfield->_data(temp->_h()-1,temp->_w())==0&&notInWall(temp->_h()-1,temp->_w())){//means unvisited and not wall
                    tempQ.push(new Node(temp->_h()-1,temp->_w()));
                    tempfield->setData(temp->_h()-1,temp->_w(),tempfield->_data(temp)+1);
                }
                //visit right
                if(found==false&&tempfield->_data(temp->_h(),temp->_w()+1)==0&&notInWall(temp->_h(),temp->_w()+1)){//means unvisited and not wall
                    tempQ.push(new Node(temp->_h(),temp->_w()+1));
                    tempfield->setData(temp->_h(),temp->_w()+1,tempfield->_data(temp)+1);
                }
                //visit down
                if(found==false&&tempfield->_data(temp->_h()+1,temp->_w())==0&&notInWall(temp->_h()+1,temp->_w())){//means unvisited and not wall
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
                        //cleanedBlock++;
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
                            //cleanedBlock++;
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
        queue <Node> *findShortestPath(Node *_from,Node *_to){
            //construct a matrix "similar" to field
            Field *tempField=new Field(field);//used to store the smallest number of cleaned block needed to pass to get to here
            Field *distance=new Field(field);//used to store distance to "from"
            for(int i=0;i<tempField->_H();i++){
                for(int j=0;j<tempField->_W();j++){
                    distance->setData(i,j,-1);
                    if(field->_data(i,j)==1){
                        tempField->setData(i,j,1);
                    }else{
                        tempField->setData(i,j,0);
                    }
                }
            }
            Node ***previous=new Node**[field->_H()];
            for(int i=0;i<field->_H();i++){
                previous[i]=new Node*[field->_W()];
            }
            //counting path beside destination
            int count=0,paths;
            if(notInWall(_to->_h(),_to->_w())){
                paths=((tempField->_data(_to->_h(),_to->_w()-1)==0)?1:0)+
                      ((tempField->_data(_to->_h()-1,_to->_w())==0)?1:0)+
                      ((tempField->_data(_to->_h(),_to->_w()+1)==0)?1:0)+
                      ((tempField->_data(_to->_h()+1,_to->_w())==0)?1:0);
            }else{
                paths=1;
            }
            //start finding
            queue <Node> *Q=new queue <Node> (houseSize);
            previous[_from->_h()][_from->_w()]=_from;
            distance->setData(_from,0);
            tempField->setData(_from,-1);
            //search from _from
            //visit left
            if(tempField->_data(_from->_h(),_from->_w()-1)!=1&&notInWall(_from->_h(),_from->_w()-1)){
                if((_from->_h()==_to->_h())&&(_from->_w()-1==_to->_w())){
                        count++;
                }
                tempField->setData(_from->_h(),_from->_w()-1,tempField->_data(_from)+(field->_data(_from->_h(),_from->_w()-1)<0)?-1:0);
                distance->setData(_from->_h(),_from->_w()-1,distance->_data(_from)+1);
                previous[_from->_h()][_from->_w()-1]=_from;
                Q->push(new Node(_from->_h(),_from->_w()-1));
            }
            //visit up
            if(tempField->_data(_from->_h()-1,_from->_w())!=1&&notInWall(_from->_h()-1,_from->_w())){
                if((_from->_h()-1==_to->_h())&&(_from->_w()==_to->_w())){
                        count++;
                }
                tempField->setData(_from->_h()-1,_from->_w(),tempField->_data(_from)+(field->_data(_from->_h()-1,_from->_w())<0)?-1:0);
                distance->setData(_from->_h()-1,_from->_w(),distance->_data(_from)+1);
                previous[_from->_h()-1][_from->_w()]=_from;
                Q->push(new Node(_from->_h()-1,_from->_w()));
            }
            //visit right
            if(tempField->_data(_from->_h(),_from->_w()+1)!=1&&notInWall(_from->_h(),_from->_w()+1)){
                if((_from->_h()==_to->_h())&&(_from->_w()+1==_to->_w())){
                        count++;
                }
                tempField->setData(_from->_h(),_from->_w()+1,tempField->_data(_from)+(field->_data(_from->_h(),_from->_w()+1)<0)?-1:0);
                distance->setData(_from->_h(),_from->_w()+1,distance->_data(_from)+1);
                previous[_from->_h()][_from->_w()+1]=_from;
                Q->push(new Node(_from->_h(),_from->_w()+1));
            }
            //visit down
            if(tempField->_data(_from->_h()+1,_from->_w())!=1&&notInWall(_from->_h()+1,_from->_w())){
                if((_from->_h()+1==_to->_h())&&(_from->_w()==_to->_w())){
                        count++;
                }
                tempField->setData(_from->_h()+1,_from->_w(),tempField->_data(_from)+(field->_data(_from->_h()+1,_from->_w())<0)?-1:0);
                distance->setData(_from->_h()+1,_from->_w(),distance->_data(_from)+1);
                previous[_from->_h()+1][_from->_w()]=_from;
                Q->push(new Node(_from->_h()+1,_from->_w()));
            }
            while(Q->size()!=0){
                Node *temp=Q->front();
                Q->pop();
                //visit left
                if(tempField->_data(temp->_h(),temp->_w()-1)!=1){//means not wall
                    if((temp->_h()==_to->_h())&&(temp->_w()-1==_to->_w())){
                        count++;
                    }
                    if(distance->_data(temp->_h(),temp->_w()-1)==-1){//never visited
                        tempField->setData(temp->_h(),temp->_w()-1,tempField->_data(temp)+(field->_data(temp->_h(),temp->_w()-1)<0)?-1:0);
                        distance->setData(temp->_h(),temp->_w()-1,distance->_data(temp)+1);
                        previous[temp->_h()][temp->_w()-1]=temp;
                        Q->push(new Node(temp->_h(),temp->_w()-1));
                    }else if(notInWall(temp->_h(),temp->_w()-1)){//ever visited
                        if(distance->_data(temp->_h(),temp->_w()-1)>distance->_data(temp)+1){//compare distance
                            distance->setData(temp->_h(),temp->_w()-1,distance->_data(temp)+1);
                            previous[temp->_h()][temp->_w()-1]=temp;
                        }else if(distance->_data(temp->_h(),temp->_w()-1)==distance->_data(temp)+1){
                            if(tempField->_data(temp->_h(),temp->_w()-1)<tempField->_data(temp)+(field->_data(temp->_h(),temp->_w()-1)<0)?-1:0){//compare tempField
                                tempField->setData(temp->_h(),temp->_w()-1,tempField->_data(temp)+(field->_data(temp->_h(),temp->_w()-1)<0)?-1:0);
                                previous[temp->_h()][temp->_w()-1]=temp;
                            }
                        }
                    }
                }
                //visit up
                if(tempField->_data(temp->_h()-1,temp->_w())!=1){//means not wall
                    if((temp->_h()-1==_to->_h())&&(temp->_w()==_to->_w())){
                        count++;
                    }
                    if(distance->_data(temp->_h()-1,temp->_w())==-1){//never visited
                        tempField->setData(temp->_h()-1,temp->_w(),tempField->_data(temp)+(field->_data(temp->_h()-1,temp->_w())<0)?-1:0);
                        distance->setData(temp->_h()-1,temp->_w(),distance->_data(temp)+1);
                        previous[temp->_h()-1][temp->_w()]=temp;
                        Q->push(new Node(temp->_h()-1,temp->_w()));
                    }else if(notInWall(temp->_h()-1,temp->_w())){//ever visited
                        if(distance->_data(temp->_h()-1,temp->_w())>distance->_data(temp)+1){//compare distance
                            distance->setData(temp->_h()-1,temp->_w(),distance->_data(temp)+1);
                            previous[temp->_h()-1][temp->_w()]=temp;
                        }else if(distance->_data(temp->_h()-1,temp->_w())==distance->_data(temp)+1){
                            if(tempField->_data(temp->_h()-1,temp->_w())<tempField->_data(temp)+(field->_data(temp->_h()-1,temp->_w())<0)?-1:0){//compare tempField
                                tempField->setData(temp->_h()-1,temp->_w(),tempField->_data(temp)+(field->_data(temp->_h()-1,temp->_w())<0)?-1:0);
                                previous[temp->_h()-1][temp->_w()]=temp;
                            }
                        }
                    }
                }
                //visit right
                if(tempField->_data(temp->_h(),temp->_w()+1)!=1){//means not wall
                    if((temp->_h()==_to->_h())&&(temp->_w()+1==_to->_w())){
                        count++;
                    }
                    if(distance->_data(temp->_h(),temp->_w()+1)==-1){//never visited
                        tempField->setData(temp->_h(),temp->_w()+1,tempField->_data(temp)+(field->_data(temp->_h(),temp->_w()+1)<0)?-1:0);
                        distance->setData(temp->_h(),temp->_w()+1,distance->_data(temp)+1);
                        previous[temp->_h()][temp->_w()+1]=temp;
                        Q->push(new Node(temp->_h(),temp->_w()+1));
                    }else if(notInWall(temp->_h(),temp->_w()+1)){//ever visited
                        if(distance->_data(temp->_h(),temp->_w()+1)>distance->_data(temp)+1){//compare distance
                            distance->setData(temp->_h(),temp->_w()+1,distance->_data(temp)+1);
                            previous[temp->_h()][temp->_w()+1]=temp;
                        }else if(distance->_data(temp->_h(),temp->_w()+1)==distance->_data(temp)+1){
                            if(tempField->_data(temp->_h(),temp->_w()+1)<tempField->_data(temp)+(field->_data(temp->_h(),temp->_w()+1)<0)?-1:0){//compare tempField
                                tempField->setData(temp->_h(),temp->_w()+1,tempField->_data(temp)+(field->_data(temp->_h(),temp->_w()+1)<0)?-1:0);
                                previous[temp->_h()][temp->_w()+1]=temp;
                            }
                        }
                    }
                }
                //visit down
                if(tempField->_data(temp->_h()+1,temp->_w())!=1){//means not wall
                    if((temp->_h()+1==_to->_h())&&(temp->_w()==_to->_w())){
                        count++;
                    }
                    if(distance->_data(temp->_h()+1,temp->_w())==-1){//never visited
                        tempField->setData(temp->_h()+1,temp->_w(),tempField->_data(temp)+(field->_data(temp->_h()+1,temp->_w())<0)?-1:0);
                        distance->setData(temp->_h()+1,temp->_w(),distance->_data(temp)+1);
                        previous[temp->_h()+1][temp->_w()]=temp;
                        Q->push(new Node(temp->_h()+1,temp->_w()));
                    }else if(notInWall(temp->_h()+1,temp->_w())){//ever visited
                        if(distance->_data(temp->_h()+1,temp->_w())>distance->_data(temp)+1){//compare distance
                            distance->setData(temp->_h()+1,temp->_w(),distance->_data(temp)+1);
                            previous[temp->_h()+1][temp->_w()]=temp;
                        }else if(distance->_data(temp->_h()+1,temp->_w())==distance->_data(temp)+1){
                            if(tempField->_data(temp->_h()+1,temp->_w())<tempField->_data(temp)+(field->_data(temp->_h()+1,temp->_w())<0)?-1:0){//compare tempField
                                tempField->setData(temp->_h()+1,temp->_w(),tempField->_data(temp)+(field->_data(temp->_h()+1,temp->_w())<0)?-1:0);
                                previous[temp->_h()+1][temp->_w()]=temp;
                            }
                        }
                    }
                }
                
                //find destination enough times
                if(count==paths){
                    break;
                }
            }
            Q->reset();
            Q->setSize(houseSize);
            Node *temp=_to;
            while(temp!=_from){
                Q->push(temp);
                temp=previous[temp->_h()][temp->_w()];
            }
            return Q;
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
        Node *temp=BFS_q.front();
        m.insert(temp);
        BFS_q.pop();
        //visit left
        if(field->_data(temp->_h(),temp->_w()-1)==0&&bot->notInWall(temp->_h(),temp->_w()-1)){//0 means unvisited
            field->setData(temp->_h(),temp->_w()-1,field->_data(temp)+1);
            BFS_q.push(new Node(temp->_h(),temp->_w()-1));
        }
        //visit up
        if(field->_data(temp->_h()-1,temp->_w())==0&&bot->notInWall(temp->_h()-1,temp->_w())){//0 means unvisited
            field->setData(temp->_h()-1,temp->_w(),field->_data(temp)+1);
            BFS_q.push(new Node(temp->_h()-1,temp->_w()));
        }
        //visit right
        if(field->_data(temp->_h(),temp->_w()+1)==0&&bot->notInWall(temp->_h(),temp->_w()+1)){//0 means unvisited
            field->setData(temp->_h(),temp->_w()+1,field->_data(temp)+1);
            BFS_q.push(new Node(temp->_h(),temp->_w()+1));
        }
        //visit down
        if(field->_data(temp->_h()+1,temp->_w())==0&&bot->notInWall(temp->_h()+1,temp->_w())){//0 means unvisited
            field->setData(temp->_h()+1,temp->_w(),field->_data(temp)+1);
            BFS_q.push(new Node(temp->_h()+1,temp->_w()));
        }
    }
    field->setData(charger,-1);//recover R=-1

    //cleaning
    while(bot->powerIsOn()){
        if(field->_data(m._rootNode())>1){
            bot->moveTo(m._rootNode());//move to farthest block in shortest path
            bot->roaming();
            bot->moveTo(charger);
        }else{
            bot->knockedOff();
        }
    }

    cout << endl ;    
    ofstream of("final.path");
    of << q.size()-1 << endl;
    q.printAll(of);
    of.close();
    field->check();
    cout << q.size()-1 << endl ;
    cout << WalkableBlockNumbers;
    return 0;
}