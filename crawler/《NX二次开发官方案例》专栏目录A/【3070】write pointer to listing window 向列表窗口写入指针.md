### 【3070】write pointer to listing window 向列表窗口写入指针

#### 代码

```cpp
    #define WRITE_P(X) (write_pointer_to_listing_window(#X, X))  
    static void write_pointer_to_listing_window(char *title, void *pointer)  
    {  
        if (pointer == NULL)  
            ECHO("%s = NULL\n", title);  
        else  
            ECHO("%s = %p != NULL\n", title, pointer);  
    }

```

#### 代码解析

> 这段代码定义了一个宏WRITE_P，用于输出变量名和对应的指针地址到列表窗口。
>
> 主要功能包括：
>
> 1. 定义了一个宏WRITE_P，它接受一个参数X，并展开为调用write_pointer_to_listing_window函数，并传递变量名#X和变量地址X。
> 2. write_pointer_to_listing_window函数负责输出变量名和对应的指针地址到列表窗口。
> 3. 如果指针为NULL，则输出"变量名 = NULL"；否则输出"变量名 = 指针地址 != NULL"。
> 4. ECHO宏用于向列表窗口输出信息。
>
> 这段代码的主要作用是调试，通过输出变量名和指针地址，可以方便地观察变量的内存分配情况。通过宏定义，可以简化调用方式，提高代码的可读性。
>
