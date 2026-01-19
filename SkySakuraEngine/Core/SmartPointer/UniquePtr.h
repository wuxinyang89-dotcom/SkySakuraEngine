#pragma once

template<typename T>
class UniquePtr
{
public:
    UniquePtr()
    {
        unique_data_=nullptr;
    }
    
    UniquePtr(T& data=nullptr) 
    {
        unique_data_=&data;
    }
    
    explicit UniquePtr(T* data=nullptr) noexcept
    {
        unique_data_=data;
    }

    UniquePtr(const UniquePtr& other){
        if (other.unique_data_!=nullptr)
        {
            unique_data_=other.unique_data_;
            other.unique_data_=nullptr;
        }
    }
    
    UniquePtr& operator=(const UniquePtr& other)
    {
        if (this!=&other)
        {
            if (this->unique_data_!=nullptr)
            {
                delete this->unique_data_;
            }
            if (other.unique_data_!=nullptr)
            {
                unique_data_=other.unique_data_;
                other.unique_data_=nullptr;
            }
        }
        return *this;
    }
    
    void make_unique()
    {
        
        //*unique_data_=data;
    }
    
    T* operator->()
    {
        return this->unique_data_;
    }
    
    ~UniquePtr()
    {
        delete unique_data_;
    }
private:
    T* unique_data_;
};
