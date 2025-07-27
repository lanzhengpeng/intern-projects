### 【0347】ask route segment rcps 询问路径段的参考点

#### 代码

```cpp
    /*HEAD ASK_ROUTE_SEGMENT_RCPS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_route.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：注意，UF_print_syslog 是在 V18 中新增的。 */  
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
    /*  This fails prior to NX 1.0.3.1 - see PR 4733117 译:翻译如下：

在NX 1.0.3.1之前的版本中会失败 - 参见PR 4733117 */  
    /* 里海 */  
    static int mask_for_routing_segments(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { 0, 0, UF_UI_SEL_FEATURE_ROUTING_CURVE_ONLY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_routing_segment(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a routing segment", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_routing_segments, NULL, &resp,  
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
            segment,  
            rcp[2];  
        char  
            msg[133];  
        while ((segment = select_a_routing_segment("Select Routing Segment")) != NULL_TAG)  
        {  
            UF_CALL(UF_ROUTE_ask_seg_rcps(segment, rcp));   
            sprintf( msg, "RCP tags: %d, %d", rcp[0], rcp[1]);  
            uc1601(msg, TRUE);  
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

> 这段代码是用于NX软件中的二次开发，其主要功能是获取选中线段的两个相关RCP点。以下是代码的主要逻辑：
>
> 1. 定义了错误报告函数report_error，用于输出错误信息。
> 2. 定义了mask_for_routing_segments函数，用于设置选择过滤条件，只选择特征线。
> 3. 定义了select_a_routing_segment函数，用于弹出选择对话框，让用户选择一个特征线段。
> 4. 在do_it函数中，循环调用select_a_routing_segment获取用户选择的线段，然后调用UF_ROUTE_ask_seg_rcps获取该线段的两个相关RCP点，并输出这两个点的tag号。
> 5. 定义了ufusr函数，作为程序的入口函数，在初始化后调用do_it，最后终止程序。
> 6. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总体来说，这段代码通过选择对话框获取用户选中的特征线段，然后查询并输出该线段对应的两个RCP点的tag号。
>
