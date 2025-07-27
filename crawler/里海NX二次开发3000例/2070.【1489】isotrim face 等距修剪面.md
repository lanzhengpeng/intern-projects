### 【1489】isotrim face 等距修剪面

#### 代码

```cpp
    static tag_t isotrim_face(tag_t object, double trim_param[4])  
    {  
        tag_t  
            new_object = object;  
        UF_CALL(UF_MODL_isotrim_face(&new_object,trim_param));  
        return(new_object);  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发的C++函数，其主要功能是对NX中的面进行等距修剪。
>
> 函数的名称为isotrim_face，接受两个参数：
>
> 1. object：要修剪的面所在的NX对象。
> 2. trim_param：一个包含4个双精度浮点数的数组，用于指定修剪的参数，包括修剪的偏置量、修剪的方向等。
>
> 函数内部首先创建了一个新的NX对象new_object，然后将旧的object对象传递给UF_MODL_isotrim_face函数进行等距修剪。这个NX API函数会根据trim_param中的参数对面进行修剪，并返回一个新的修剪后的对象。
>
> 最后，函数返回新的修剪后的对象new_object。
>
> 这个函数的主要作用是提供了一个简单的接口，用于在NX中根据指定的参数对面进行等距修剪。通过封装NX的API，使得二次开发人员可以更方便地进行模型操作。
>
