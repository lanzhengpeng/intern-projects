### 【1662】move tabular note 移动表格注释

#### 代码

```cpp
    /*HEAD MOVE_TABULAR_NOTE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

注意：UF_print_syslog是V18版本新增的。 */  
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
    static int mask_for_tabular_notes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_tabular_note_type, UF_tabular_note_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_tabular_note(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_tabular_notes, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical select_pos(char *prompt, double pos[3])  
    {  
        int  
            resp = uc1615(prompt, pos);  
        if (resp == 5) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            tabular_note;  
        double  
            pos[3];  
        while (((tabular_note =  
            select_a_tabular_note("Select tabnote to move")) != NULL_TAG) &&  
            select_pos("Indicate new location", pos))  
        {  
            UF_CALL(UF_DRF_set_origin(tabular_note, pos));  
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

> 这段代码是一个用于NX的二次开发示例，主要功能是移动表格注释。下面是对代码的简要介绍：
>
> 1. 包含必要的头文件：代码首先包含了必要的NX API头文件，如uf.h、uf_ui.h等。
> 2. 错误报告函数：定义了report_error函数，用于在API调用失败时打印错误信息。
> 3. 表格注释选择掩码：定义了mask_for_tabular_notes函数，用于设置选择掩码，仅选择表格注释。
> 4. 选择表格注释：定义了select_a_tabular_note函数，用于弹出对话框让用户选择一个表格注释。
> 5. 选择位置：定义了select_pos函数，用于获取用户指定的新位置。
> 6. 执行移动：定义了do_it函数，循环调用select_a_tabular_note和select_pos，获取用户选择的表格注释和位置，并调用UF_DRF_set_origin函数移动注释到新位置。
> 7. ufusr入口函数：定义了ufusr函数作为程序的入口，初始化NX环境，调用do_it函数执行移动操作，然后终止NX环境。
> 8. 卸载请求函数：定义了ufusr_ask_unload函数，用于处理卸载请求。
>
> 这段代码通过用户交互式选择和指定位置，实现了表格注释的移动功能，并包含了错误处理和卸载处理。
>
