#include <iostream>
#include <ostream>

#include "Core/DataStructures/Array.h"
//#include "Core/Reflection/Function.h"
#include "Core/DataStructures/Stack.h"
#include "Core/Delegate/Delegate.h"
#include "Core/SmartPointer/UniquePtr.h"
#include "Function/EngineCore/Engine.h"

int main(int argc, char* argv[])
{
    
    Array<int> myArray = {1, 2, 3, 4, 5};
    //using class_type=Function<decltype(&Array<int>::begin)>::class_type;
    auto ptr = myArray.size();
    //Function<decltype(&Array<int>::size)> func(&myArray,&Array<int>::size);
    //std::cout<<(func())<<std::endl;
    
    Delegate<int> delegate;
    delegate.bind(&myArray,&Array<int>::size);
    delegate.unbind(&myArray,&Array<int>::size);
    delegate.invoke();
    
    try
    {
        UniquePtr<Engine> engine_ptr(new Engine());
        engine_ptr->Run();
    }
    catch( std::exception e )
    {
        std::cout<<e.what()<<std::endl;
        return -1;
    }
    
    return 0;
}
