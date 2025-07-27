### 【2247】report object view modifications 报告对象视图修改

#### 代码

```cpp
    static void report_object_view_modifications(tag_t object)  
    {  
        int  
            ii,  
            n_edits;  
        UF_VIEW_vde_data_p_t  
            vde_data;  
        UF_CALL(UF_VIEW_ask_vde_data(object, &n_edits, &vde_data));  
        WRITE_D(n_edits);  
        for (ii = 0; ii < n_edits; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_D(vde_data[ii].view_tag);  
            WRITE_F(vde_data[ii].start_parameter);  
            WRITE_F(vde_data[ii].end_parameter);  
            WRITE_D(vde_data[ii].color);  
            WRITE_D(vde_data[ii].font);  
            WRITE_D(vde_data[ii].width);  
        }  
        if (n_edits > 0) UF_free(vde_data);  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是报告对象在视图中的修改情况。以下是代码的主要步骤：
>
> 1. 首先，定义一个静态函数report_object_view_modifications，它接受一个NX对象标签作为参数。
> 2. 在函数内部，定义变量n_edits和vde_data，用于存储视图修改数据的数量和结构体数组。
> 3. 使用UF_VIEW_ask_vde_data函数获取传入对象的视图修改数据，并存储在n_edits和vde_data变量中。
> 4. 通过一个for循环遍历vde_data数组，输出每个视图修改记录的信息，包括视图标签、起始参数、结束参数、颜色、字体和线宽。
> 5. 如果n_edits大于0，释放vde_data数组占用的内存。
> 6. WRITE_D和WRITE_F是自定义的输出函数，用于打印变量值。
>
> 通过该函数，可以获取和输出一个NX对象在所有视图中的修改情况，包括视图标签、参数、颜色等，从而监控对象视图属性的变更。
>
