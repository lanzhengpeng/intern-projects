### 【1579】make quadrant points on ends of cylindrical face 在圆柱面端部制作四分点

#### 代码

```cpp
    /*HEAD MAKE_QUADRANT_POINTS_ON_ENDS_OF_CYLINDRICAL_FACE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_so.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的信息，可以总结如下：

UF_print_syslog 是 V18 版本新增的函数。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    /* qq3123197280 */  
    static int mask_for_cylindrical_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_CYLINDRICAL_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_cylindrical_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select cylindrical face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_cylindrical_faces, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
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
    static void create_smart_points_spaced_along_curve(tag_t curve, double inc_percent)  
    {  
        tag_t  
            last,  
            offset,  
            start;  
        double  
            percent;  
        UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_update_after_modeling,  
            0.0, &start));  
        UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_update_after_modeling,  
            inc_percent/100, &offset));  
        UF_CALL(UF_SO_create_point_on_curve(curve, UF_SO_update_after_modeling,  
            curve, start, &last));  
        UF_CALL(UF_SO_set_visibility_option(last, UF_SO_visible));  
        for (percent = inc_percent; percent < 100; percent = percent+inc_percent)  
        {  
            UF_CALL(UF_SO_create_point_along_curve(curve,  
                UF_SO_update_after_modeling, curve, last, offset,  
                UF_SO_point_along_curve_percent, FALSE, &last));  
            UF_CALL(UF_SO_set_visibility_option(last, UF_SO_visible));  
        }  
    }  
    static void do_it(void)  
    {  
        int   
            ii,  
            n_edges,  
            edge_type;  
        tag_t  
            face = NULL_TAG,  
            *edges = NULL;  
        uf_list_p_t  
            edge_list;  
        if(( face = select_a_cylindrical_face("Pick Cylindrical Face:" ) ) &&  
                                                    NULL_TAG != face )  
        {  
            UF_CALL(UF_MODL_ask_face_edges( face, &edge_list ));  
            if ((n_edges = make_an_array( &edge_list, &edges )) > 0)  
            {  
                for (ii = 0; ii < n_edges; ii++)  
                {  
                    UF_CALL(UF_MODL_ask_edge_type(edges[ii], &edge_type));  
                    if (edge_type == UF_MODL_CIRCULAR_EDGE)  
                    {  
                        create_smart_points_spaced_along_curve( edges[ii], 25.0 );  
                    }  
                }  
                UF_free(edges);  
                if( n_edges > 0 ) UF_MODL_delete_list( &edge_list );  
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

> 这段代码是一个NX Open C++的UF二次开发示例，主要功能是在圆柱面上均匀地生成智能点。
>
> 代码的主要步骤包括：
>
> 1. 包含UF相关的头文件。
> 2. 定义了错误报告函数report_error，用于打印错误信息。
> 3. 定义了选择圆柱面的mask函数mask_for_cylindrical_faces。
> 4. 定义了选择圆柱面的函数select_a_cylindrical_face。
> 5. 定义了分配内存的函数allocate_memory。
> 6. 定义了将list转换为数组的函数make_an_array。
> 7. 定义了沿曲线均匀生成智能点的函数create_smart_points_spaced_along_curve。
> 8. 主函数do_it中，首先选择圆柱面，然后获取其所有边，对圆弧边均匀生成智能点。
> 9. 定义了ufusr函数，初始化后执行do_it，然后终止。
> 10. 定义了卸载函数ufusr_ask_unload。
>
> 整体来说，这是一个典型的NX Open C++二次开发示例，通过UF函数实现沿圆柱面圆弧边均匀生成智能点的功能。
>
