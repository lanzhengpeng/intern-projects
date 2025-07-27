### 【0616】create block centerline 创建块中心线

#### 代码

```cpp
    /*HEAD CREATE_BLOCK_CLINE CCC UFUN */  
    /*    
        NOTE: this assumes that both selected edges were in the same view.  
        If that is not the case, then you need to get the correct view tag  
        into the appropriate object_view_tag structure element.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_drf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* 里海 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
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
    /* 里海 */  
    static int mask_for_linear_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* 里海 */  
    static tag_t select_a_linear_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select linear edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_linear_edges, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            edge1 = NULL_TAG,  
            edge2 = NULL_TAG,  
            view_tag = NULL_TAG,  
            centerline_tag = NULL_TAG;  
        UF_DRF_object_t   
            defining_obj_list[2];  
        char  
            view_name[30] = { "" };  
        UF_CALL(UF_UI_set_cursor_view(0));  
        if((edge1=select_a_linear_edge("First Edge:"))!=NULL_TAG &&  
           (edge2=select_a_linear_edge("Second Edge:"))!=NULL_TAG )  
        {  
            uc1653 ( view_name );  
            UF_CALL(UF_OBJ_cycle_by_name( view_name, &view_tag ));  
            defining_obj_list[0].object_tag = edge1;  
            defining_obj_list[1].object_tag = edge2;  
            defining_obj_list[0].object_view_tag = view_tag;  
            defining_obj_list[1].object_view_tag = view_tag;  
            UF_CALL( UF_DRF_create_block_cline( defining_obj_list,   
                                               NULL, &centerline_tag ));  
        }  
    }  
    /* 里海 */  
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

> 这段代码是一个NX Open C++ API的二次开发代码，用于在NX中创建一个块状中心线。以下是代码的主要功能：
>
> 1. 包含了NX Open C++ API的头文件，定义了与NX交互所需的函数和数据结构。
> 2. 定义了一个错误报告函数report_error，用于在函数调用出错时打印错误信息。
> 3. 定义了一个选择线性边的函数mask_for_linear_edges，通过设置选择掩码，使得只可以选择线性边。
> 4. 定义了一个选择线性边的函数select_a_linear_edge，通过弹出的选择对话框让用户选择一个线性边。
> 5. 在主函数do_it中，通过调用select_a_linear_edge函数让用户选择两个线性边，并获取当前视图的tag。然后调用UF_DRF_create_block_cline函数创建块状中心线。
> 6. 定义了ufusr函数，作为NX的启动函数，初始化NX环境后调用do_it函数，完成后关闭NX环境。
> 7. 定义了ufusr_ask_unload函数，当用户卸载该程序时立即卸载。
>
> 总的来说，这段代码实现了在NX中选择两个线性边，并在它们之间创建一个块状中心线，并通过错误报告函数提高了程序的健壮性。
>
