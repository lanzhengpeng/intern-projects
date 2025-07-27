### 【0227】ask first other part 询问第一个其他部分

#### 代码

```cpp
    static tag_t ask_first_other_part(tag_t except_part)  
    {  
        int  
            ii,  
            n = UF_PART_ask_num_parts();  
        tag_t  
            a_part;  
        if (n > 1)  
        {  
            for (ii = 0; ii < n; ii++)  
            {  
                a_part = UF_PART_ask_nth_part(ii);  
                if (a_part != except_part) return a_part;  
            }  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码定义了一个名为ask_first_other_part的函数，其功能是查找当前模型中第一个与指定标记except_part不同的部件标记。
>
> 函数的基本逻辑如下：
>
> 1. 首先，调用UF_PART_ask_num_parts获取当前模型中的部件数量。
> 2. 如果部件数量大于1，则遍历所有部件。
> 3. 对于每个部件，调用UF_PART_ask_nth_part获取其标记。
> 4. 如果该部件的标记与指定的except_part不同，则直接返回该部件的标记。
> 5. 如果遍历完所有部件后都没有找到与except_part不同的部件，则返回NULL_TAG。
>
> 该函数的主要用途是在当前模型中查找与指定部件不同的第一个部件。在NX二次开发中，这种功能可以用于需要基于特定部件执行操作，但又不希望影响该特定部件本身的情况。
>
