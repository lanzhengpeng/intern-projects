### 【2261】report owning groups 报告拥有组

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_group.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的函数。 */  
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
    static int mask_add_faces_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_object(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an object", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, NULL,NULL,&resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            mbr,  
            *owners;  
        char  
            msg[133];  
        int  
            ii = 0,  
            n = 0;  
        while ( (mbr = select_an_object("Select an object")) != NULL_TAG)  
        {  
            UF_CALL(UF_GROUP_ask_all_owning_groups( mbr, &n, &owners ));  
            if(n > 0)  
            {  
                if (!UF_UI_open_listing_window())  
                    UF_UI_write_listing_window("Tags of owning groups:\n\n");  
                for (ii = 0; ii < n; ii++)  
                {  
                    sprintf(msg, "%d. %d\n", ii + 1, owners[ii]);  
                    UF_UI_write_listing_window(msg);  
                }  
            }  
            else  
            {  
                if (!UF_UI_open_listing_window())  
                    UF_UI_write_listing_window("No group found for that object");  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了一个函数mask_add_faces_and_edges，用于设置选择掩码，使NX选择对话框仅显示实体特征的面和边。
> 3. 定义了一个函数select_an_object，用于打开NX选择对话框，让用户选择一个对象，并返回其tag。
> 4. 定义了一个函数do_it，循环调用select_an_object获取用户选择的每个对象，然后查询这个对象属于哪些组，并将这些组的tag输出到NX列表窗口。
> 5. 定义了主函数ufusr，初始化NX环境后调用do_it函数，然后终止NX环境。
> 6. 定义了卸载函数ufusr_ask_unload，表示立即卸载此二次开发模块。
>
> 总体来说，这段代码通过循环让用户选择对象，然后查询并输出这些对象所属的组，实现了在NX中选择对象并获取其所属组的功能。代码结构清晰，包含错误处理，是一个典型的NX二次开发示例。
>
