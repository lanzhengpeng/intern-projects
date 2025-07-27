### 【1394】highlight ordinate dimensions of ordinate origin 高亮坐标原点的坐标尺寸

#### 代码

```cpp
    /*HEAD HIGHLIGHT_ORDINATE_DIMENSIONS_OF_ORDINATE_ORIGIN CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_undo.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

UF_print_syslog是V18版本中的新功能。 */  
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
    static int ask_all_ordinate_dimensions(tag_t part, tag_t **objects)  
    {  
        int  
            this_type,  
            this_subtype;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_dimension_type, &object))  
            && (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &this_type, &this_subtype));  
            if ((this_subtype == UF_dim_ordinate_horiz_subtype) ||  
                (this_subtype == UF_dim_ordinate_vert_subtype))  
                (UF_MODL_put_list_item(object_list, object));  
        }  
        return (make_an_array(&object_list, objects));  
    }  
    static int ask_whats_missing(tag_t *old, int n_old, tag_t *now, int n_now,  
        tag_t **missing)  
    {  
        int  
            ii,  
            jj;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < n_old; ii++)  
        {  
            for (jj = 0; jj < n_now; jj++)  
                if (now[jj] == old[ii]) break;  
            if (jj == n_now) UF_CALL(UF_MODL_put_list_item(list, old[ii]));  
        }  
        return (make_an_array(&list, missing));  
    }  
    static tag_t ask_next_ordinate_origin(tag_t part, tag_t item)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_dimension_type, &item))  
                && (item != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(item, &type, &subtype));  
            if (subtype == UF_dim_ordinate_origin_subtype) return item;  
        }  
        return NULL_TAG;  
    }  
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int ii;  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
    }  
    static void do_it(void)  
    {  
        int  
            n_odims,  
            n_odims_now,  
            n_odims_orig;  
        tag_t  
            *odims,  
            *odims_now,  
            *odims_orig,  
            oorigin = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_UNDO_mark_id_t  
            mark;  
        char  
            oname[MAX_ENTITY_NAME_SIZE],  
            msg[133];  
        n_odims_orig = ask_all_ordinate_dimensions(part, &odims_orig);  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, NULL, &mark));  
        while ((oorigin = ask_next_ordinate_origin(part, oorigin)) != NULL_TAG)  
        {  
            UF_CALL(UF_DISP_set_display(UF_DISP_SUPPRESS_DISPLAY));  
            UF_CALL(UF_OBJ_delete_object(oorigin));  
            n_odims_now = ask_all_ordinate_dimensions(part, &odims_now);  
            UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
            UF_CALL(UF_DISP_regenerate_display());  
            UF_CALL(UF_DISP_set_display(UF_DISP_UNSUPPRESS_DISPLAY));  
            n_odims = ask_whats_missing(odims_orig, n_odims_orig, odims_now,  
                n_odims_now, &odims);  
            set_highlight_object_array(n_odims, odims, TRUE);  
            UF_CALL(UF_OBJ_ask_name(oorigin, oname));  
            sprintf(msg, "%s - %d dimensions", oname, n_odims);  
            uc1601(msg, TRUE);  
            set_highlight_object_array(n_odims, odims, FALSE);  
            if (n_odims > 0) UF_free(odims);  
            if (n_odims_now > 0) UF_free(odims_now);  
        }  
        if (n_odims_orig > 0) UF_free(odims_orig);  
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

> 这段NX二次开发代码实现了以下功能：
>
> 1. 遍历当前显示的零件，找出所有的水平标尺和垂直标尺，并将它们的ID存入数组中。
> 2. 在零件中循环遍历每一个标尺原点，将其删除，然后获取删除后的标尺数量。
> 3. 比较删除前后的标尺数量，找出被删除的标尺，并将它们的ID存入数组中。
> 4. 高亮显示被删除的标尺，并显示提示信息，包括标尺原点的名称和被删除的标尺数量。
> 5. 重复步骤2-4，直到零件中不再有标尺原点为止。
> 6. 释放所有分配的内存。
> 7. 在函数 ufusr 中调用 do_it 函数实现上述功能。
> 8. 在函数 ufusr_ask_unload 中设置立即卸载。
>
> 综上所述，这段代码主要用于高亮显示并提示被删除的标尺，以便用户了解每个标尺原点所关联的标尺数量，从而实现标尺的智能管理。
>
