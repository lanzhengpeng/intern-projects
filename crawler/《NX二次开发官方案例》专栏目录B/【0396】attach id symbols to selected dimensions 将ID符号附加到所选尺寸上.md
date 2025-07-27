### 【0396】attach id symbols to selected dimensions 将ID符号附加到所选尺寸上

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
    #include <uf_object_types.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    /* 里海 */  
    static int mask_for_dimensions(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_dimension_type, UF_all_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_dimension(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_dimensions, NULL, &resp,  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            dim,  
            idsym;  
        double  
            loc[3];  
        char  
            text[MAX_ID_SYM_TEXT_LENGTH + 1] = { "A" };  
        while (prompt_for_text("Enter balloon text", text))  
            while (((dim = select_a_dimension("Select dimension")) != NULL_TAG)  
                && (select_pos("Indicate balloon location", loc)))  
            {  
                UF_CALL(UF_DRF_create_id_symbol(UF_DRF_sym_circle, text, "", loc,  
                    UF_DRF_without_leader, UF_DRF_leader_attach_object, NULL_TAG,  
                    NULL, &idsym));  
                UF_CALL(UF_DRF_add_assortpart_to_ann(dim, 1, &idsym));  
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

> 这段代码是一个NX二次开发程序，主要实现以下功能：
>
> 1. 使用UF_initialize()和UF_terminate()初始化和终止NX。
> 2. 定义了一个错误报告函数report_error()，用于打印错误信息。
> 3. 定义了一个提示用户输入文本的函数prompt_for_text()。
> 4. 定义了一个选择一个尺寸的函数select_a_dimension()。
> 5. 定义了一个选择位置的函数select_pos()。
> 6. 定义了一个主函数do_it()，用于创建和添加尺寸标注。
> 7. 在do_it()中，首先提示用户输入文本，然后循环选择尺寸和位置，为每个尺寸创建圆形标注并添加到尺寸上。
> 8. ufusr()是程序的入口函数，调用do_it()执行主要功能。
> 9. ufusr_ask_unload()函数返回立即卸载。
>
> 综合来看，该程序通过对话框提示用户输入文本和选择尺寸，为每个尺寸创建圆形标注并添加到尺寸上，实现了尺寸标注的功能。
>
