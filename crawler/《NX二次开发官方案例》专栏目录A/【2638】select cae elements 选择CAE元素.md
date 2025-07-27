### 【2638】select cae elements 选择CAE元素

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_cae_elements(UF_UI_selection_p_t select, void *type)  
    {  
    //  These are new in NX7.5.1 - see uf_ui_type.h:  
        UF_UI_mask_t  
            mask = { UF_pseudo_object_type, UF_pseudo_CAE_subtype,  
                UF_pseudo_CAE_element};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_cae_elements(char *prompt, tag_t **cae_elements)  
    {  
        int  
            cnt,  
            resp;  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark_id));  
        UF_CALL(UF_UI_select_with_class_dialog("Select CAE elements", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_cae_elements, NULL,  
            &resp, &cnt, cae_elements));  
    /*  See PR 6469859  
        The number of elements can be very large so don't do this:  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*cae_elements)[ii], FALSE));  
        Even this can take forever:  
        UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));  
        This seems to work but the elements cannot be selected anymore:  
        UF_DISP_regenerate_display();  
        This works great however both UNDO and regen are needed:  
    */  
        UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
        UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));  
        UF_DISP_regenerate_display();  
        return cnt;  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要功能是选择CAE元素，并进行相应的处理。
>
> 主要步骤包括：
>
> 1. 定义选择掩码：使用UF_UI_mask_t结构定义了一个选择掩码，指定了要选择的对象类型为伪对象，子类型为CAE，元素类型为伪CAE元素。这可以确保只选择CAE相关元素。
> 2. 清除并启用特定选择掩码：使用UF_UI_set_sel_mask函数清除现有的选择，并启用定义的选择掩码，以准备选择操作。
> 3. 与类对话框选择元素：调用UF_UI_select_with_class_dialog函数，弹出一个对话框，让用户根据提示选择CAE元素。其中，mask_for_cae_elements函数用于设置选择掩码。
> 4. 处理选择结果：处理选择结果，获取选择元素的数量和元素标签。同时，代码使用了UNDO标记来保存选择前的状态，并在选择后恢复，以确保选择不会影响后续操作。
> 5. 清理高亮显示：选择后，代码不直接清除高亮显示，而是通过UNDO回滚到选择前的状态，然后删除UNDO标记。最后，重新生成显示来清理高亮显示，从而避免直接清除可能造成的性能问题。
> 6. 返回选择数量：最后返回选择的元素数量。
>
> 总的来说，这段代码实现了在NX中根据类型选择CAE元素，并进行了一些优化以确保性能。
>
