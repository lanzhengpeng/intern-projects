### 【0265】ask next dplane 询问下一个基准平面

#### 代码

```cpp
    static tag_t ask_next_dplane(tag_t part, tag_t dplane)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_datum_plane_type, &dplane));  
        return (dplane);  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发的，其主要功能是遍历指定部件中的所有平面并获取下一个平面。
>
> 代码中使用了UF_OBJ_cycle_objs_in_part()函数来遍历指定部件中的所有平面，其中part是部件的tag，UF_datum_plane_type表示遍历类型为平面。此外，dplane作为输入和输出参数，表示当前遍历到的平面。
>
> 函数返回下一个平面的tag。如果当前平面已经是最后一个，则返回0。在调用此函数之前，需要确保dplane参数已经被初始化为部件中第一个平面的tag。
>
> 通过使用这个函数，可以方便地遍历部件中的所有平面，并进行相应的操作。
>
