### 【2263】report parents of drafting curve 报告草绘曲线的父对象

#### 代码

```cpp
    static void report_parents_of_drafting_curve(tag_t curve)  
    {  
        int  
            gtype,  
            ii,  
            n_parents = 0,  
            subtype;  
        tag_t  
            group,  
            *parents;  
        char  
            name[31];  
        UF_CALL(UF_DRAW_ask_group_of_curve(curve, &group, &gtype, &subtype));  
        if (group != NULL_TAG)  
        {  
            UF_CALL(UF_SO_ask_parents(curve, UF_SO_ASK_ALL_PARENTS, &n_parents,  
                &parents));  
            for (ii = 0; ii < n_parents; ii++)  
            {  
                sprintf(name, "parents[%d]", ii);  
                report_object_info(name, parents[ii]);  
            }  
            if (n_parents > 0) UF_free(parents);  
        }  
        sprintf(name, "%d parents", n_parents);  
        UF_CALL(UF_UI_set_status(name));  
    }

```

#### 代码解析

> 这段代码用于在NX中报告草绘曲线的所有父对象。主要功能包括：
>
> 1. 获取曲线所在的组的类型和子类型。
> 2. 使用UF_SO_ask_parents函数获取曲线的所有父对象，包括直接父对象和间接父对象。
> 3. 遍历所有父对象，并使用report_object_info函数报告每个父对象的信息。
> 4. 如果存在父对象，则释放父对象数组的空间。
> 5. 设置状态栏显示父对象的数量。
>
> 通过这段代码，可以获取草绘曲线的所有父对象信息，并进行相应的操作或处理。
>
