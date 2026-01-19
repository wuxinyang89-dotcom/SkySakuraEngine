#pragma once
#include <stdexcept>

#include "ControlBlock.h"

template<typename T>
class SharedPtr
{
public:
   SharedPtr()
   {
       control_block_=nullptr;
   }
    
   explicit SharedPtr(T* data) noexcept
   {
       control_block_=new ControlBlock<T>(*data);
       ++control_block_->share_cnt_;
   }
    
    explicit SharedPtr(T data) noexcept
   {
       control_block_=new ControlBlock<T>(data);
       ++control_block_->share_cnt_;
   }
    
   void make_shared(T data)
   {
       if (control_block_!=nullptr)
       {
           --control_block_->share_cnt_;
           if (control_block_->share_cnt_==0)
           {
               delete control_block_;
           }
       }
       control_block_=new ControlBlock<T>(data);
       ++control_block_->share_cnt_;
   }
    
    SharedPtr(const SharedPtr<T>& other)
   {
       control_block_=other.control_block_;
       if (control_block_!=nullptr)
       {
           if (control_block_->share_cnt_==~0)
           {
               throw std::runtime_error("ControlBlock Count Full!");
           }
           ++control_block_->share_cnt_;
       }
   }
    
    SharedPtr<T>& operator=(const SharedPtr<T>& other)
   {
       if (this != &other)
       {
           if (control_block_!=nullptr)
           {
               --control_block_->share_cnt_;
               if (control_block_->share_cnt_==0)
               {
                   delete control_block_;
               }
           }
           control_block_=other.control_block_;
           if (control_block_!=nullptr)
           {
               if (control_block_->share_cnt_==~0)
               {
                   throw std::runtime_error("ControlBlock Count Full!");
               }
               ++control_block_->share_cnt_;
           }
       }
       return *this;
   }
    
    T& operator*()
   {
       if (control_block_==nullptr)
       {
           throw std::runtime_error("ControlBlock Null");
       }
       return control_block_->data_;
   }
    
   unsigned int get_share_count()
   {
       if (control_block_==nullptr)
       {
           throw std::runtime_error("ControlBlock Null");
       }
       return control_block_->share_cnt_;
   }
    
    ~SharedPtr()
   {
       if (control_block_!=nullptr)
       {
           --control_block_->share_cnt_;
           if (control_block_->share_cnt_==0)
           {
               delete control_block_;
           }
       }
   }
private:
    ControlBlock<T>* control_block_;
};
