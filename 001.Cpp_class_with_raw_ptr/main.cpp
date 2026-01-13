/*
Design a C++ class with raw pointer members
Implement constructor, destructor, copy constructor, copy assignment (Rule of 3 / 5)
*/



#include <iostream>

class Simple{
    private:
        int *data;
    public:
        Simple(int value=0){
            data = new int(value);
            std::cout<<"Constructor\n";
        }
        
        ~Simple(){
            delete data;
            std::cout<<"Destructor\n";
        }
        
        
        //Simple(const Simple& ) = delete;
        //Simple operator=(const Simple&) =delete;
        Simple(const Simple& other){
            data = new int(*other.data);
            std::cout << "Copy Constructor\n";
        }
        
        Simple& operator=(const Simple& other){
            std::cout << "Copy Assignment\n";
            if(this != &other){
                delete data;
                data = new int(*other.data); 
            }
            return *this;
        }
        
        Simple(Simple&& other) noexcept {
            data = other.data;
            other.data = nullptr;
            std::cout << "Move Constructor\n";
        }
        
        Simple& operator=(Simple&& other){
            std::cout << "Move Assignment\n";
            if(this != &other){
                delete data;
                data = other.data;
                other.data = nullptr;
            }
            return *this;
        }
        
  
        
        
        void print() const {
            if(data){
                std::cout<<"Value: "<< *data<<"\n";
            }
            else{
                std::cout<<"Value:  NULL "<<"\n";
            }
        }
        
};


int main(){
    
    {
        Simple a;
        a.print();
    }
    {
        Simple b(10);
        b.print();
    
        Simple c = b;
        c.print();
        
        Simple d(20);
        d.print();
        
        d = c;
        d.print();
        
        
        Simple e = std::move(b);
        e.print();
        
        e = std::move(d);
        e.print();
        
    }
    return 0;
}
