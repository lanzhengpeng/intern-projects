### 【0099】ask all other parts 获取所有其他部件

#### 代码

```cpp
    static int ask_all_other_parts(tag_t except_part, tag_t **parts)  
    {  
        int  
            err,  
            ii, jj,  
            n = UF_PART_ask_num_parts();  
        tag_t  
            a_part;  
        if (n > 1)  
        {  
            *parts = UF_allocate_memory((n-1) * sizeof(tag_t), &err);  
            if (UF_CALL(err)) return 0;  
            for (ii = 0, jj = 0; ii < n; ii++)  
            {  
                a_part = UF_PART_ask_nth_part(ii);  
                if (a_part != except_part) (*parts)[jj++] = a_part;  
            }  
        }  
        return n  - 1;  
    }

```

#### 代码解析

> 这段NX Open C++代码用于从一个NX部件中查询除指定部件外的所有其他部件，并将这些部件的tag存储在一个tag_t类型的数组中。
>
> 主要步骤如下：
>
> 1. 首先调用UF_PART_ask_num_parts()获取当前部件中的部件数量。
> 2. 判断部件数量是否大于1，如果是，则分配一个大小为n-1的tag_t类型数组，用于存储除指定部件外的其他部件的tag。
> 3. 遍历当前部件中的所有部件，使用UF_PART_ask_nth_part()获取每个部件的tag。
> 4. 判断当前遍历的部件tag是否等于要排除的指定部件tag，如果不是，则将该部件的tag存储到步骤2中分配的数组中。
> 5. 遍历结束后，返回存储其他部件tag的数组及其长度n-1。
>
> 该函数的目的是获取一个NX部件中除指定部件外的所有其他部件，以便后续对这些部件进行其他操作。
>
