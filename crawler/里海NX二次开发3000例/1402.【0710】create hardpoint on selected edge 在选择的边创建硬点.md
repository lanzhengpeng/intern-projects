### 【0710】create hardpoint on selected edge 在选择的边创建硬点

#### 代码

```cpp
    /*HEAD CREATE_HARDPOINT_ON_SELECTED_EDGE CCC UFUN*/  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_sf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog 是 V18 中新增的功能。 */  
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
    static tag_t select_an_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_edges, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        double  
            ref_point[3] = {0.334, 0.2, 0.0};  
        tag_t      
            edge[1] = {NULL_TAG},  
            hardpoint_tag = NULL_TAG;  
        if ((edge[0] = select_an_edge("Select edge for hardpoint"))!= NULL_TAG)  
        {         
            UF_CALL(UF_SF_create_hardpoint_on_geom (edge[0], ref_point, &hardpoint_tag));  
            UF_CALL(UF_DISP_regenerate_display());  
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

> 这段代码是一个NX二次开发示例，主要实现了在选定的边线上创建硬点的功能。
>
> 1. 错误报告宏：定义了UF_CALL宏，用于检查UF函数调用是否成功，如果不成功，则报告错误。
> 2. 错误报告函数：定义了report_error函数，用于打印UF函数调用失败的错误信息。
> 3. 边线选择掩码：定义了mask_for_edges函数，用于设置选择掩码，使得用户只能选择边线。
> 4. 选择边线：定义了select_an_edge函数，用于弹出对话框让用户选择一个边线，并返回边线的tag。
> 5. 创建硬点：在do_it函数中，先调用select_an_edge选择边线，然后调用UF_SF_create_hardpoint_on_geom在该边线上创建硬点，并更新显示。
> 6. 主函数：定义了ufusr函数，作为NX的启动函数，初始化后调用do_it函数，最后终止。
> 7. 卸载函数：定义了ufusr_ask_unload函数，表示该代码可以被立即卸载。
>
> 这段代码通过NX的二次开发接口，实现了边线选择、硬点创建等交互操作，具有一定的参考价值。
>
