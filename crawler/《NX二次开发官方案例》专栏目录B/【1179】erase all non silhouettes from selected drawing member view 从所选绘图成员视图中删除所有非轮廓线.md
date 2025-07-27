### 【1179】erase all non silhouettes from selected drawing member view 从所选绘图成员视图中删除所有非轮廓线

#### 代码

```cpp
    /*HEAD ERASE_ALL_NON_SILHOUETTES_FROM_SELECTED_DRAWING_MEMBER_VIEW CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_view.h>  
    #include <uf_draw.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，V18中新增了一个名为UF_print_syslog的函数。 */  
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
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static tag_t select_drawing_member_view(char *prompt)  
    {  
        int  
            orig_cv,  
            resp;  
        tag_t  
            view_tag;  
        double  
            pos[3];  
        UF_CALL(UF_UI_ask_cursor_view(&orig_cv));  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_specify_screen_position(prompt, NULL, NULL, pos,  
            &view_tag, &resp));  
        UF_CALL(UF_UI_set_cursor_view(orig_cv));  
        if (resp == UF_UI_PICK_RESPONSE)  
            return view_tag;  
        else  
            return NULL_TAG;  
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
    static int ask_all_objects_visible_in_view(tag_t view, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_VIEW_cycle_objects(view, UF_VIEW_VISIBLE_OBJECTS,  
            &object)) && (object != NULL_TAG))  
        {  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
        }  
        return make_an_array(&object_list, objects);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_objects,  
            subtype,  
            type;  
        tag_t  
            *objects,  
            group,  
            view;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_DRAW_view_prfs_t  
            view_parms;  
        while ((view = select_drawing_member_view("Erase non-silhouettes"))  
            != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(view, view_name));  
            UF_CALL(UF_DRAW_ask_view_display(view, &view_parms));  
            view_parms.silhouettes = UF_DRAW_silhouettes_on;  
            view_parms.extracted_edges = UF_DRAW_extracted_edges_on;  
            UF_CALL(UF_DRAW_set_view_display(view, &view_parms));  
            n_objects = ask_all_objects_visible_in_view(view, &objects);  
            for (ii = 0; ii < n_objects; ii++)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(objects[ii], &type, &subtype));  
                if ((type >= UF_line_type) && (type <= UF_spline_type))  
                {  
                    UF_CALL(UF_DRAW_ask_group_of_curve(objects[ii], &group,  
                        &type, &subtype));  
                    if ((group == NULL_TAG)  || (type != UF_solid_silhouette_type))  
                    {  
                        WRITENZ(uc6400(view_name, objects[ii]));  
                    }  
                }  
            }  
            if (n_objects > 0) UF_free(objects);  
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

> 这段代码是一个用于NX的二次开发示例，主要功能是从选定的绘图成员视图中删除所有非轮廓线。代码包括以下关键点：
>
> 1. 包含必要的NX API头文件，如uf.h, uf_ui.h等。
> 2. 定义了一个错误报告函数report_error，用于输出错误代码和调用信息。
> 3. 定义了一个写入列表窗口的函数write_integer_to_listing_window。
> 4. 定义了一个选择绘图成员视图的函数select_drawing_member_view。
> 5. 定义了一个内存分配函数allocate_memory。
> 6. 定义了一个创建对象数组函数make_an_array。
> 7. 定义了一个查询视图内所有可见对象的函数ask_all_objects_visible_in_view。
> 8. 定义了主函数do_it，用于遍历视图内所有对象，删除非轮廓线对象。
> 9. 定义了ufusr函数，用于初始化NX API，调用主函数，然后终止NX API。
> 10. 定义了卸载函数ufusr_ask_unload，返回立即卸载。
>
> 整体而言，该代码实现了从选定视图内删除非轮廓线对象的功能，并包含了错误处理、内存管理、对象遍历等NX二次开发常见的代码结构。
>
