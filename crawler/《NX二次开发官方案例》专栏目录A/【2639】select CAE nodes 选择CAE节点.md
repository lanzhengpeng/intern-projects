### 【2639】select CAE nodes 选择CAE节点

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_cae_nodes(UF_UI_selection_p_t select, void *type)  
    {  
    //  These are new in NX7.5.1 - see uf_ui_type.h:  
        UF_UI_mask_t  
            mask = { UF_pseudo_object_type, UF_pseudo_CAE_subtype,  
                UF_pseudo_CAE_node};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_cae_nodes(char *prompt, tag_t **cae_nodes)  
    {  
        int  
            cnt,  
            resp;  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark_id));  
        UF_CALL(UF_UI_select_with_class_dialog("Select CAE Nodes", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_cae_nodes, NULL,  
            &resp, &cnt, cae_nodes));  
    /*  See PR 6469859  
        The number of nodes can be very large so don't do this:  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*cae_nodes)[ii], FALSE));  
        Even this can take forever:  
        UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));  
        This seems to work but the nodes cannot be selected anymore:  
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

> 这是NX二次开发中的代码段，主要功能是选择CAE节点。
>
> 代码中定义了一个静态函数mask_for_cae_nodes，用于设置选择掩码，以便在对话框中选择CAE节点。然后定义了一个函数select_cae_nodes，用于打开选择对话框，让用户选择CAE节点。在选择完成后，代码会撤销选择操作，并刷新显示。需要注意的是，由于CAE节点的数量可能非常大，所以代码中避免了直接清除高亮显示，而是选择了撤销选择并刷新显示的方式。
>
