#include <iostream>

using namespace std;

void condition(int a, bool x);
void loop(int a);

int main(){
    int a= 10;
    bool x= 1;
    condition(a, x);
    loop(a);
}

void condition(int a, bool x){
    if(x){
        cout<< "True"<< endl;
    }
    else{
        cout<< "False"<< endl;
    }
}

void loop(int a){
    for(int i= 0; i< a; i++){
        cout<< "loop executing.."<< endl;
    }
    cout<< "loop executed completely..."<< endl;
}