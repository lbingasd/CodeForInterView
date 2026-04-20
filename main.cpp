#include <iostream>
#include "SingletonTest/LazySingleton.h"

int main()
{
       LazySingleton::GetInstance()->Show();
       std::cout<<"123";
       return 0;
}