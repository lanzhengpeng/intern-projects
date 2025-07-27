### 【1150】edit group and member objects display 编辑组和成员对象显示

#### 代码

```cpp
    static void edit_group_and_member_objects_display(tag_t group, int operation,  
        int new_value)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *members;  
        UF_CALL(UF_GROUP_ask_group_data(group, &members, &n));  
        switch (operation)  
        {  
            case 1:  
                UF_OBJ_set_layer(group, new_value);  
                for (ii = 0; ii < n; ii++)  
                    UF_OBJ_set_layer(members[ii], new_value);  
                break;  
            case 2:  
                UF_OBJ_set_color(group, new_value);  
                for (ii = 0; ii < n; ii++)  
                    UF_OBJ_set_color(members[ii], new_value);  
                break;  
            case 3:  
                UF_OBJ_set_blank_status(group, new_value);  
                for (ii = 0; ii < n; ii++)  
                    UF_OBJ_set_blank_status(members[ii], new_value);  
                break;  
            case 4:  
                UF_OBJ_set_line_width(group, new_value);  
                for (ii = 0; ii < n; ii++)  
                    UF_OBJ_set_line_width(members[ii], new_value);  
                break;  
            case 5:  
                UF_OBJ_set_font(group, new_value);  
                for (ii = 0; ii < n; ii++)  
                    UF_OBJ_set_font(members[ii], new_value);  
                break;  
            case 6:  
                UF_DISP_set_highlight(group, new_value);  
                for (ii = 0; ii < n; ii++)  
                    UF_DISP_set_highlight(members[ii], new_value);  
                break;  
            default:  
                break;  
        }  
        if (n > 0) UF_free(members);  
    }

```

#### 代码解析

> 这段代码是用于NX Open C++ API的二次开发代码，其主要功能是根据指定的组对象和操作类型，更新组对象及其成员对象的显示属性。
>
> 代码的主要逻辑如下：
>
> 1. 通过UF_GROUP_ask_group_data函数获取组对象的所有成员对象数组及其数量。
> 2. 根据不同的操作类型，更新组对象和所有成员对象的显示属性。操作类型包括设置图层、颜色、隐藏状态、线宽、字体、高亮等。
> 3. 更新完毕后，释放成员对象数组。
> 4. 该函数可以同时更新组对象及其所有成员对象的显示属性，以提高开发效率。
>
> 该代码提供了便捷的二次开发接口，通过一个函数即可统一处理组对象和其成员对象的显示属性，无需逐个调用函数，从而简化了NX二次开发中对组对象及其成员对象进行显示属性设置的过程。
>
