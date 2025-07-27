### 【1395】highlight ordinate origin of selected ordinate dimension 高亮选中坐标尺寸的坐标原点

#### 代码

```cpp
    /*HEAD HIGHLIGHT_ORDINATE_ORIGIN_OF_SELECTED_ORDINATE_DIMENSION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_undo.h>  
    #include <uf_part.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog 是 V18 的新功能，只回答翻译，不要添加其他内容。 */  
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
    static int ask_all_of_type_and_subtype(tag_t part, int type, int subtype,  
        tag_t **objects)  
    {  
        int  
            this_type,  
            this_subtype;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object)) &&  
                (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &this_type, &this_subtype));  
            if (this_subtype == subtype)  
                (UF_MODL_put_list_item(object_list, object));  
        }  
        return (make_an_array(&object_list, objects));  
    }  
    /* qq3123197280 */  
    static int mask_for_ordinate_dimensions(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_dimension_type, UF_dim_ordinate_horiz_subtype, 0 },  
                        { UF_dimension_type, UF_dim_ordinate_vert_subtype, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_ordinate_dimension(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a dimension", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_ordinate_dimensions, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_oorigins,  
            status;  
        tag_t  
            odim,  
            *oorigins,  
            part = UF_PART_ask_display_part();  
        char  
            oname[MAX_ENTITY_NAME_SIZE+1];  
        UF_UNDO_mark_id_t  
            mark;  
        n_oorigins = ask_all_of_type_and_subtype(part, UF_dimension_type,  
            UF_dim_ordinate_origin_subtype, &oorigins);  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, NULL, &mark));  
        while ((odim = select_an_ordinate_dimension("Highlight origin"))  
            != NULL_TAG)  
        {  
            UF_CALL(UF_DISP_set_display(UF_DISP_SUPPRESS_DISPLAY));  
            for (ii = 0; ii < n_oorigins; ii++)  
            {  
                UF_CALL(UF_OBJ_delete_object(oorigins[ii]));  
                status = UF_OBJ_ask_status(odim);  
                UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
                if (status == UF_OBJ_DELETED) break;  
            }  
            UF_CALL(UF_DISP_regenerate_display());  
            UF_CALL(UF_DISP_set_display(UF_DISP_UNSUPPRESS_DISPLAY));  
            if (ii < n_oorigins)  
            {  
                UF_CALL(UF_DISP_set_highlight(oorigins[ii], TRUE));  
                UF_CALL(UF_OBJ_ask_name(oorigins[ii], oname));  
                uc1601(oname, TRUE);  
                UF_CALL(UF_DISP_set_highlight(oorigins[ii], FALSE));  
            }  
            else  
                uc1601("Cannot find origin", TRUE);  
        }  
        if (n_oorigins > 0) UF_free(oorigins);  
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

> 这段代码是一个NX Open C++ API编写的NX二次开发示例代码，主要功能是选择一个坐标尺寸，然后高亮显示其原点。以下是代码的主要功能和流程：
>
> 1. 包含了必要的NX Open C++ API头文件。
> 2. 定义了一个错误报告函数report_error，用于在函数调用出错时输出错误信息。
> 3. 定义了一个内存分配函数allocate_memory。
> 4. 定义了一个函数make_an_array，用于将对象列表转换为对象数组。
> 5. 定义了一个函数ask_all_of_type_and_subtype，用于获取所有指定类型和子类型的对象。
> 6. 定义了一个函数mask_for_ordinate_dimensions，用于设置选择坐标系尺寸的选择器掩码。
> 7. 定义了一个函数select_an_ordinate_dimension，用于选择一个坐标系尺寸。
> 8. 定义了主函数do_it，流程如下：获取当前显示部件。获取所有坐标原点尺寸对象。设置一个撤销标记。循环选择坐标系尺寸。隐藏显示。删除坐标原点尺寸对象。撤销到标记。如果尺寸对象被删除，则高亮显示对应的坐标原点尺寸对象。否则提示找不到原点。释放坐标原点尺寸对象数组。
> 9. 获取当前显示部件。
> 10. 获取所有坐标原点尺寸对象。
> 11. 设置一个撤销标记。
> 12. 循环选择坐标系尺寸。
> 13. 隐藏显示。
> 14. 删除坐标原点尺寸对象。
> 15. 撤销到标记。
> 16. 如果尺寸对象被删除，则高亮显示对应的坐标原点尺寸对象。
> 17. 否则提示找不到原点。
> 18. 释放坐标原点尺寸对象数组。
> 19. 定义了ufusr函数，作为NX二次开发入口函数，初始化后调用主函数，最后终止。
> 20. 定义了ufusr_ask_unload函数，用于卸载二次开发库。
>
> 总的来说，这段代码通过选择坐标系尺寸并高亮显示其原点，实现了NX二次开发的一个典型示例。
>
