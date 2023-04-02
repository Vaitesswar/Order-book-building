#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>
#include <cstring>
#include <string>
#include <algorithm>
using namespace std;

struct Square { // By default, all variables and methods are public.
  int length;
  Square(int l){
    this->length = l;
  }
  int area(){
    return length*length;
  }
  int perimeter(){
    return 4*length;
  }
};

class Rectangle { // By default, all variables and methods are private.
  protected:
    int width;
    int breadth;
  public:
    Rectangle(){};
    Rectangle(int width, int breadth){
      this->width = width;
      this->breadth = breadth;
    }
    void setWidth(int width){
      this->width = width;
    }
    void setBreadth(int breadth){
      this->breadth = breadth;
    }
    int area(){
      return width*breadth;
    }
    int perimeter(int width, int breadth){
      return 2*(width + breadth);
    }
};

class Rectangle2:public Rectangle {
  protected:
    int width;
    int breadth;
  public:
    Rectangle2(int width, int breadth){
        this->width = width;
        this->breadth = breadth;
    }
    int area(){
      return width;
    }
};

template<class T>
T Max(T a, T b){
    if (a > b){
        return a;
    } else {
        return b;
    }
}

int main() {
  // Function
  cout << "Max is " << Max(5,7) << endl;
  cout << "Max is " << Max(7.5,2.3) << endl;
  
  // Class
  Rectangle rect(2,5);
  cout << "Area is " << rect.area() << endl;
  cout << "Perimeter is " << rect.perimeter(2,5) << endl;
  rect.setWidth(7);
  rect.setBreadth(9);
  cout << "Area is " << rect.area() << endl;
  cout << "Perimeter is " << rect.perimeter(7,9) << endl;
  
  Rectangle2 rect2(3,2);
  cout << "Area is " << rect2.area() << endl;
   cout << "Perimeter is " << rect2.perimeter(3,2) << endl;
  
  //Struct
  Square sqr(5);
  cout << "Area is " << sqr.area() << endl;
  cout << "Perimeter is " << sqr.perimeter() << endl;
  
  // Vector
  vector<int> y = {5,7,8,3,3};
  sort(y.begin(),y.end()); //Ascending sorting
  vector<int>::iterator it1;
  for (it1 = y.begin(); it1 != y.end(); it1++){
    cout << *it1 << endl;
  }
  y.erase(unique(y.begin(), y.end()),y.end());
  for (int el:y){
    cout << el << endl;
  }
  sort(y.begin(),y.end(),greater<int>()); //Descending sorting
  for (int i = 0; i < y.size(); i ++){
    cout << y[i] << endl; // random access iterator
  }
  cout << y.empty() << endl; // Check if vector is empty
  int maxElement = *max_element (y.begin(),y.end());
  cout << "Max element: " << maxElement << endl;
  int minElement = *min_element (y.begin(),y.end());
  cout << "Min element: " << minElement << endl;
  // Vector algorithms: https://www.geeksforgeeks.org/c-magicians-stl-algorithms/
  
  // List
  list<int> x = {1,2,3,4,7,9};
  list<int>::iterator it = x.begin();
  advance(it,3);
  x.insert(it,2017);
  x.push_back(105);
  x.push_front(102);
  x.sort();
  x.reverse();
  for(it = x.begin(); it != x.end(); it++){
    cout << *it << endl;
  }
  list<int>::reverse_iterator it7 = x.rbegin();
  for(it7 = x.rbegin(); it7 != x.rend(); it7++){
  cout << *it7 << endl;
  }
  list<int>::iterator it2 = x.begin();
  for (auto element:x){
    cout << element << endl;
  }
  
  // Map
	unordered_map<int,float> xMap = {{5,6.8},{8,9.0}};
  xMap[0] = 1.0;
  xMap.insert(make_pair(9,157.7));
  unordered_map<int,float>::iterator it3;
  for (it3 = xMap.begin(); it3 != xMap.end(); it3++){
    cout << it3->first << " " << it3->second << endl;
  }
  cout << xMap[0] << endl;
  
  // Array & pointer
  int* p = new int[5];
  int q[5] = {1,2,3,6,7};
  int* r = q; // Pointing to first element of array q
  p[0] = 5;
  p[3] = 27;
  cout << r << endl;
  cout << r[3] << endl;
  // Method 1
  for (int i = 0; i < sizeof(q)/sizeof(q[0]); i++){
    cout << r[i] << endl;
  }
  for (int i = 0; i < sizeof(q)/sizeof(q[0]); i++){
    cout << *r << endl;
    r++; // Pointer arithmatic
  }
  for (int i = 0; i < 5; i++){
    cout << *p << endl;
    p++; // Pointer arithmatic
  }
  int* t = q;
  int* s = &q[3];
  cout << "distance: " << s - q << endl; // Pointer arithmatic
  
  // Reference
  int a = 5;
  int &b = a; // A different variable name for same memory address and data
  cout << a << endl;;
  cout << b << endl;
  cout << &a << endl;
  cout << &b << endl;
  
  // char
  char xyx[] = "Hello World";
  const char* abc = xyx; //Pointer to first letter
  cout << abc << endl;
  int i = 0;
  while (abc[i] != '\0'){ //Stops at last character before reaching null
    cout << *abc << endl;
    cout << abc[i] << endl;
    cout << abc << endl;
    abc++;
  }
  const char* bbb = "";
  char aaa[50] = "BBBBBBBBBmytrBmBBBBBBBB";
  char bbc[] = "AAA";
  cout << abc[0] << " " << abc << endl;
  cout << aaa << endl;
  cout << sizeof(aaa)/sizeof(char) << endl;
  cout << strlen(abc) << endl;
  cout << strcat(bbc,aaa) << endl;
  cout << strcpy(bbc,aaa) << endl;
  cout << strcmp(abc,aaa) << endl;
  cout << strchr(aaa,'m') << " " << strrchr(aaa,'m') << endl;
  char soup[20] = "mytrBm";
  if (strstr(aaa,soup) != NULL){
    cout << strstr(aaa,soup) << endl;
  } else {
    cout << "NOT FOUND" << endl;
  }
  
  for (int i = 0; i < sizeof(aaa)/sizeof(char); i++){
    cout << aaa[i] << endl; //Prints empty spaces
  }
 
  
  // string
  string X = "HELLO WORLD I LOVE YOU!";
  for (char x:X){
    cout << x;
  }
  string::iterator it4;
  for (it4 = X.begin(); it4 != X.end(); it4++){
    cout << *it4;
  }
  string::reverse_iterator it8 = X.rbegin();
  for (it8 = X.rbegin(); it8 != X.rend(); it8++){
    cout << *it8;
  }
  for (int i = 0; i < X.size(); i++){
    cout << X[i];
  }
  string Y = X.substr(2,5);
  cout << Y << endl;
  cout << X.compare(X) << endl;
  cout << X + Y << endl;
  cout << X.find("L") << endl;
  cout << X.find_first_of("O") << endl;
  return 0;
}
