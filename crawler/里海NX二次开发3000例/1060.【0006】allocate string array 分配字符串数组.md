### 【0006】allocate string array 分配字符串数组

#### 代码

```cpp
    static int allocate_string_array(int n_elements, int n_chars, char ***where)  
    {  
        int  
            ii,  
            resp;  
        *where = UF_allocate_memory(n_elements * sizeof(char *), &resp);  
        for (ii = 0; ii < n_elements; ii++)  
        {  
            (*where)[ii] = UF_allocate_memory(n_chars * sizeof(char), &resp);  
            if (resp) return resp;  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是用于在NX Open中分配一个二维字符数组的内存空间。其功能包括：
>
> 1. 定义了一个静态函数allocate_string_array，用于分配一个字符指针数组和一个字符串数组。
> 2. 参数包括元素数量n_elements、每个字符串的最大字符数n_chars，以及一个指向字符指针数组的指针where。
> 3. 首先使用UF_allocate_memory函数分配字符指针数组的空间，大小为n_elements * sizeof(char*)。
> 4. 接着遍历每个元素，为每个元素分配一个字符串空间，大小为n_chars * sizeof(char)。
> 5. 若分配失败，函数会返回错误码resp。
> 6. 若所有空间都分配成功，则返回0。
> 7. 该函数简化了在NX Open中分配二维字符数组的过程，避免了手动进行多次内存分配。
>
> 总的来说，这段代码封装了在NX Open中分配二维字符数组的内存操作，提高了代码的复用性和可维护性。
>
