### 【1221】export selected bodies 选择要导出的实体，导出选择实体

#### 代码

```cpp
    /*HEAD EXPORT_SELECTED_BODIES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的，用于打印系统日志信息。

在V18版本中新增了一个名为UF_print_syslog的函数，用于打印系统日志信息。 */  
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
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_bodies(char *prompt, tag_t **bodies)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select solids or sheets", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp, &cnt,  
            bodies));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*bodies)[ii], FALSE));  
        return cnt;  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            *bodies;  
        if ((n = select_bodies("Select bodies to export", &bodies)) > 0)  
        {  
            UF_CALL(UF_PART_export("/tmp/junk.prt", n, bodies));  
            UF_free(bodies);  
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

> 这是NX的二次开发代码，主要实现了以下功能：
>
> 1. 错误处理函数：定义了report_error函数，用于打印错误信息，包括错误代码、位置、调用信息等。
> 2. 体选择掩码：定义了mask_for_bodies函数，用于设置选择掩码，只允许选择实体。
> 3. 选择实体：定义了select_bodies函数，用于弹出选择对话框，让用户选择实体。
> 4. 导出实体：定义了do_it函数，用于导出选择的实体到指定的prt文件。
> 5. ufusr入口函数：用于初始化和调用do_it函数，是二次开发的入口点。
> 6. 卸载函数：定义了ufusr_ask_unload函数，用于设置立即卸载二次开发模块。
>
> 综合来看，这段代码实现了选择实体并导出为prt文件的功能，采用了错误处理、选择掩码、选择对话框等NX二次开发常见的技巧。
>
