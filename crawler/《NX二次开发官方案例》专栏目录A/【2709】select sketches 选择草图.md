### 【2709】select sketches 选择草图

#### 代码

```cpp
    /*HEAD SELECT_SKETCHES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18中的新功能，仅回答译文，不要添加无关的废话。 */  
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
    static int mask_for_sketches(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_sketch_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_sketches(char *prompt, tag_t **sketches)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        tag_t  
            feat;  
        UF_CALL(UF_UI_select_with_class_dialog("Select sketches", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_sketches, NULL, &resp, &cnt,  
            sketches));  
        for (ii = 0; ii < cnt; ii++)  
        {  
        /*  This does not work - see PR 5038935  
            UF_CALL(UF_DISP_set_highlight((*sketches)[ii], FALSE));  
            This is the work around  
        */  
            UF_CALL(UF_MODL_ask_object_feat((*sketches)[ii], &feat));  
            UF_CALL(UF_DISP_set_highlight(feat, FALSE));  
        }  
        return cnt;  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            *sketches;  
        if ((n = select_sketches("Select Sketches", &sketches)) > 0)  
            UF_free(sketches);  
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

> 这段代码是一个NX的二次开发示例，主要用于选择草图并高亮显示。以下是代码的主要功能：
>
> 1. 定义了一个错误报告函数report_error，用于在UF_CALL宏调用UF函数失败时记录错误信息。
> 2. 定义了一个用于草图选择的掩码函数mask_for_sketches，设置选择掩码只选择草图类型。
> 3. 定义了一个选择草图函数select_sketches，使用UF_UI_select_with_class_dialog函数弹出选择对话框，让用户选择草图，并返回选择的草图数量和数组。
> 4. 在选择草图函数中，通过UF_MODL_ask_object_feat获取草图对应的特征，然后调用UF_DISP_set_highlight来高亮显示，因为直接设置草图高亮不生效。
> 5. 定义了一个do_it函数，调用select_sketches选择草图，如果选择不为空则释放草图数组内存。
> 6. ufusr是NX二次开发程序的入口函数，调用UF_initialize初始化环境，调用do_it执行主要功能，最后调用UF_terminate终止环境。
> 7. ufusr_ask_unload函数返回立即卸载，表示二次开发程序不需要在NX关闭时保留状态。
>
> 整体上，这段代码通过选择对话框让用户选择草图，并调用UF函数高亮显示所选草图。
>
