### 【3129】number selected edges 选择边，并为每条边编号，然后删除重复的边

#### 代码

```cpp
    /*HEAD NUMBER_SELECTED_EDGES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：UF_print_syslog 是在 V18 中新增的函数。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    /* qq3123197280 */  
    static int mask_for_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_edges(char *prompt, tag_t **edges)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select edges", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_edges, NULL, &resp, &cnt, edges));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*edges)[ii], FALSE));  
        return cnt;  
    }  
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void number_objects(tag_t *objects, int n_objects)  
    {  
        logical  
            is_displayable;  
        int  
            ii;  
        double  
            loc[3];  
        char  
            msg[133];  
        for (ii = 0; ii < n_objects; ii++)  
        {  
            UF_CALL(UF_OBJ_is_displayable(objects[ii], &is_displayable));  
            if (!is_displayable) continue;  
            sprintf(msg, "%d", ii+1);  
            if (UF_OBJ_ask_name_origin(objects[ii], loc))  
            {  
                UF_CALL(UF_OBJ_set_name(objects[ii], "X"));  
                UF_CALL(UF_OBJ_ask_name_origin(objects[ii], loc));  
                UF_CALL(UF_OBJ_delete_name(objects[ii]));  
            }  
            display_temporary_text(msg, loc);  
        }  
    }  
    static uf_list_p_t make_a_list(int count, tag_t *item_array)  
    {  
        int  
            ii;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < count; ii++)  
            UF_CALL(UF_MODL_put_list_item(list, item_array[ii]));  
        return (list);  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int remove_duplicate_tags_from_array(int n_objects, tag_t **objects)  
    {  
        uf_list_p_t  
            list = make_a_list(n_objects, *objects);  
        UF_free(*objects);  
        return make_an_array(&list, objects);  
    }  
    static void do_it(void)  
    {  
        int  
            n_edges;  
        tag_t  
            *edges;  
        char  
            msg[MAX_LINE_SIZE+1];  
        while ((n_edges = select_edges("Number edges", &edges)) > 0)  
        {  
            UF_DISP_refresh();  
            number_objects(edges, n_edges);  
            sprintf(msg, "Before deleting duplicates n_edges = %d", n_edges);  
            uc1601(msg, TRUE);  
        /*  Chaining may put duplicate entities in the returned array.  
            See PR 4968729 */  
            n_edges = remove_duplicate_tags_from_array(n_edges, &edges);  
            UF_DISP_refresh();  
            number_objects(edges, n_edges);  
            sprintf(msg, "After deleting duplicates n_edges = %d", n_edges);  
            uc1601(msg, TRUE);  
            UF_free(edges);  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码实现了在NX中为边线编号，并去除重复边线标签的功能。
>
> 主要功能包括：
>
> 1. 使用UF_UI_select_with_class_dialog函数选择边线，并通过自定义的mask_for_edges函数设置选择掩码为只选择特征边线。
> 2. 使用UF_DISP_display_temporary_text函数在选中的边线上方显示编号。
> 3. 使用UF_MODL_create_list和UF_MODL_put_list_item函数将选中的边线标签加入列表，然后转换为数组。
> 4. 使用UF_MODL_delete_list函数删除列表，UF_free释放数组内存。
> 5. 使用UF_MODL_ask_list_count函数统计列表数量，使用UF_MODL_create_list和UF_MODL_put_list_item函数创建新列表，将数组元素加入新列表，再转换为数组，以去除重复标签。
> 6. 在UFusr函数中调用do_it函数循环执行以上功能，直到用户没有选择任何边线。
> 7. 使用UF_initialize和UF_terminate函数初始化和终止NX Open。
> 8. 使用自定义的report_error函数打印错误信息。
> 9. 使用uc1601函数打印提示信息。
>
> 总体来说，这段代码通过UI选择、模型查询、列表操作等功能实现了为边线编号和去除重复标签的功能。
>
