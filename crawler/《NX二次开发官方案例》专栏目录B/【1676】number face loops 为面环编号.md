### 【1676】number face loops 为面环编号

#### 代码

```cpp
    static void display_temporary_colored_text(char *text, double loc[3], int color)  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        props.color = color;  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_ACTIVE_MINUS,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void number_list_objects(uf_list_p_t objects)  
    {  
        int  
            ii = 0;  
        double  
            loc[3];  
        char  
            msg[133];  
        uf_list_p_t  
            tmp;  
        for (tmp = objects; tmp != NULL; tmp = tmp->next)  
        {  
            if (UF_OBJ_ask_name_origin(tmp->eid, loc))  
            {  
                UF_CALL(UF_OBJ_set_name(tmp->eid, "X"));  
                UF_CALL(UF_OBJ_ask_name_origin(tmp->eid, loc));  
                UF_CALL(UF_OBJ_delete_name(tmp->eid));  
            }  
            sprintf(msg, "%d", ++ii);  
            display_temporary_colored_text(msg, loc, UF_OBJ_BLUE);  
        }  
    }  
    static void number_face_loops(tag_t face)  
    {  
        int  
            ii,  
            n_loops,  
            type;  
        uf_list_p_t  
            edge_list;  
        uf_loop_p_t  
            loop_list;  
        if (!UF_CALL(UF_MODL_ask_face_loops(face, &loop_list)) && loop_list)  
        {  
            UF_CALL(UF_MODL_ask_loop_list_count(loop_list, &n_loops));  
            for (ii = 0; ii < n_loops; ii++)  
            {  
                UF_CALL(UF_MODL_ask_loop_list_item(loop_list,ii,&type,&edge_list));  
                number_list_objects(edge_list);  
            }  
            UF_CALL(UF_MODL_delete_loop_list(&loop_list));  
        }  
    }

```

#### 代码解析

> 这段代码是用于NX Open的二次开发代码，主要包含以下几个部分：
>
> 1. display_temporary_colored_text函数：用于在指定位置显示临时彩色文本，用于给对象编号。
> 2. number_list_objects函数：遍历边列表，并在每条边的中点位置显示编号。
> 3. number_face_loops函数：获取面中的所有环(Loop)，然后遍历每个环中的边，并在每条边的中点位置显示编号。
> 4. UF_DISP_display_temporary_text：NX Open提供的API，用于显示临时文本。
> 5. UF_OBJ_set_name：NX Open提供的API，用于设置对象的名称。
> 6. UF_OBJ_ask_name_origin：NX Open提供的API，用于获取对象名称的位置。
> 7. UF_OBJ_delete_name：NX Open提供的API，用于删除对象的名称。
> 8. UF_MODL_ask_face_loops：NX Open提供的API，用于获取面的所有环。
> 9. UF_MODL_ask_loop_list_count：NX Open提供的API，用于获取环列表的数量。
> 10. UF_MODL_ask_loop_list_item：NX Open提供的API，用于获取环列表中的指定项。
> 11. UF_MODL_delete_loop_list：NX Open提供的API，用于删除环列表。
>
> 总体来说，这段代码的目的是对NX模型中的面进行编号，并使用临时文本显示编号，以辅助识别和选择模型中的面。
>
