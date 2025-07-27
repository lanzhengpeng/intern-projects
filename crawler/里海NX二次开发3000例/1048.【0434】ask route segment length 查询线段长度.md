### 【0434】ask route segment length 查询线段长度

#### 代码

```cpp
    /*HEAD ASK_ROUTE_SEGMENT_LENGTH CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据注释，翻译如下：

注意：UF_print_syslog是V18版本新增的功能。 */  
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
    /*  This fails prior to NX 1.0.3.1 - see PR 4733117 译:这在中兴1.0.3.1版本之前失败了-请参阅PR 4733117。 */  
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
        tag_t segment;  
        double length;  
        char msg[133];  
        while ((segment = select_a_routing_segment("Select Routing Segment")) != NULL_TAG)  
        {  
            UF_CALL(UF_ROUTE_ask_segment_length( segment, &length ));  
            sprintf( msg, "Length: %f", length);  
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

> 这段代码是一个NX Open的二次开发示例，主要功能是查询选中线段的长度。以下是该代码的主要组成部分：
>
> 1. 错误处理函数：该函数用于在调用UF函数出错时打印错误信息。
> 2. 线段选择掩码：用于在对话框中选择线段时设置选择掩码，以确保只选择线段。
> 3. 选择线段：通过对话框让用户选择一个线段，并返回该线段的tag。
> 4. 获取长度：使用UF_ROUTE_ask_segment_length函数查询选中线段的长度。
> 5. 主函数：初始化NX环境，调用do_it函数获取并打印线段长度，最后终止NX环境。
> 6. 卸载函数：返回立即卸载标志，以便在卸载用户函数时不会等待。
>
> 该代码实现了通过对话框选择线段并获取其长度的功能，为NX的二次开发提供了示例。
>
