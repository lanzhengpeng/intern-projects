### 【1529】list all csets 列出所有组件集

#### 代码

```cpp
    static void list_all_csets(void)  
    {  
        tag_t  
            cset = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            cset_name[UF_OBJ_NAME_LEN + 1];  
        WRITE("component sets in part\n");  
        while ((cset = ask_next_component_set(part, cset)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(cset, cset_name));  
            WRITE_S(cset_name);  
        }  
    }

```

#### 代码解析

> 这段代码的功能是列出NX部件中的所有组件集。具体步骤如下：
>
> 1. 使用UF_PART_ask_display_part函数获取当前显示的部件。
> 2. 初始化一个组件集的tag为NULL_TAG，用于迭代。
> 3. 使用一个while循环，每次循环调用ask_next_component_set函数获取下一个组件集的tag，直到返回NULL_TAG表示所有组件集都已遍历。
> 4. 在循环体内，调用UF_OBJ_ask_name函数获取组件集的名字，并打印输出。
> 5. 循环结束后，所有组件集的名字都被打印输出。
> 6. 该函数没有参数，也没有返回值。
> 7. 主要用到的UF函数有：UF_PART_ask_display_part、ask_next_component_set、UF_OBJ_ask_name。
> 8. 该函数通过遍历部件中的所有组件集，并打印输出，实现了列出部件中所有组件集的功能。
>
> 这段代码简单直观，通过NX提供的UF函数实现了获取和遍历组件集的功能，打印输出结果，方便用户查看部件中的组件集信息。
>
