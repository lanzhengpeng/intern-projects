### 【1182】erase then unerase selected solid in view 选择视图中的实体，先删除然后恢复选中实体

#### 代码

```cpp
    /*HEAD ERASE_THEN_UNERASE_SELECTED_SOLID_IN_VIEW CCC UFUN */  
    /*  
        Although uc6400 will erase a solid, uc6403 will not unerase one.  
        - see PR 4660916  
        This program demonstrates two different work arounds.  The easy way - set  
        an UNDO mark prior to erasing the solid and undo to that mark to unerase  
        it.  The hard way - erase/unerase all the visible edges and silhouettes  
        corresponding to the solid instead of th solid itself.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_view.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_so.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_undo.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的函数。因此，我翻译了您的请求，并提供了简洁的答案：UF_print_syslog是V18版本新增的函数。 */  
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
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_body_in_view(char *prompt, tag_t *view)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select a body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
    static int ask_visible_curves_of_solid_in_view(tag_t body, tag_t view,  
        tag_t **view_curves)  
    {  
        logical  
            is_so;  
        int  
            ii,  
            n_parents,  
            type,  
            subtype;  
        tag_t  
            object = NULL_TAG,  
            *parents,  
            this_body;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        while (!UF_CALL(UF_VIEW_cycle_objects(view, UF_VIEW_VISIBLE_OBJECTS,  
            &object)) && (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            if ((type == UF_solid_type) && (subtype == UF_solid_edge_subtype))  
            {  
                UF_CALL(UF_MODL_ask_edge_body(object, &this_body));  
                if (this_body == body)  
                    UF_CALL(UF_MODL_put_list_item(list, object));  
                continue;  
            }  
            UF_CALL(UF_SO_is_subclass(object, &is_so));  
            if (is_so)  
            {  
                UF_CALL(UF_SO_ask_parents(object, UF_SO_ASK_ALL_PARENTS,  
                    &n_parents, &parents));  
                for (ii = 0; ii < n_parents; ii++)  
                {  
                    UF_CALL(UF_OBJ_ask_type_and_subtype(parents[ii], &type,  
                        &subtype));  
                    if ((type == UF_solid_type) &&  
                        (subtype == UF_solid_face_subtype))  
                    {  
                        UF_CALL(UF_MODL_ask_face_body(parents[ii], &this_body));  
                        if (this_body == body)  
                            UF_CALL(UF_MODL_put_list_item(list, object));  
                    }  
                }  
                if (n_parents > 0) UF_free(parents);  
            }  
        }  
        return make_an_array(&list, view_curves);  
    }  
    static logical ask_yes_or_no(char *prompt, logical *deflt)  
    {  
        int  
            resp;  
        char  
            options[2][38] = { "Yes", "No" };  
        resp = 2 - *deflt;  
        resp = uc1603(prompt, resp, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
            *deflt = 6 - resp;  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        logical  
            unerase = TRUE;  
        int  
            ii,  
            n;  
        tag_t  
            body,  
            *body_curves,  
            view;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_UNDO_mark_id_t  
            mark_id;  
        while ((body = select_a_body_in_view("Erase", &view)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(view, view_name));  
        /*  The easy way 里海译:The easy way */  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark_id));  
            uc6400(view_name, body);  
            if (ask_yes_or_no("Unerase?", &unerase) && unerase)  
                UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
        /*  The hard way 里海译:翻译 The hard way: 困难的方式 */  
            if ((ask_yes_or_no("Try the hard way?", &unerase) && unerase) &&  
                ((n = ask_visible_curves_of_solid_in_view(body, view,  
                &body_curves)) > 0))  
            {  
                for (ii = 0; ii < n; ii++)  
                    uc6400(view_name, body_curves[ii]);  
                if (ask_yes_or_no("Unerase?", &unerase) && unerase)  
                {  
                    for (ii = 0; ii < n; ii++)  
                        uc6403(view_name, body_curves[ii]);  
                }  
                UF_free(body_curves);  
            }  
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

> 这段代码是用于NX的二次开发代码，主要实现了以下功能：
>
> 1. 提供两种方式来擦除然后恢复视图中的实体：简单方式：在擦除实体前设置一个UNDO标记，然后恢复到该标记来恢复实体。复杂方式：循环擦除/恢复实体对应的可见边线，而不是直接擦除实体。
> 2. 简单方式：在擦除实体前设置一个UNDO标记，然后恢复到该标记来恢复实体。
> 3. 复杂方式：循环擦除/恢复实体对应的可见边线，而不是直接擦除实体。
> 4. 用户可以选择擦除视图中的一个实体，然后选择是否恢复它。
> 5. 提供交互式对话框，让用户选择要擦除的实体，并高亮显示。
> 6. 使用UF_UNDO来设置UNDO标记，使用UF_VIEW来循环获取视图中的对象。
> 7. 使用UF_MODL来获取实体的边线、面等，并创建列表。
> 8. 使用UF_UI来提供用户交互。
> 9. 使用UF_OBJ来获取对象名称等。
> 10. 提供错误报告机制，记录函数调用失败的信息。
> 11. 提供内存分配和释放机制。
> 12. 定义了ufusr函数作为程序的入口。
>
> 总的来说，这段代码提供了两种不同的方法来擦除然后恢复视图中的实体，用户可以根据需要进行选择。它充分使用了NX提供的各种API来实现复杂的交互和建模操作。
>
