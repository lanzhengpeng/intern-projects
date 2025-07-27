### 【1142】edit appended text 编辑附加文本

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdarg.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_disp.h>  
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
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
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
        UF_CALL(UF_UI_select_with_single_dialog("Select a dimension", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_dimensions, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
        int  
            ii, jj,  
            n_strings;  
        tag_t  
            dim;  
        char  
            new_text[133],  
            *location[] = {"BEFORE", "AFTER", "ABOVE", "BELOW"};  
        UF_DRF_appended_text_t  
            *appended_text;  
        strcpy(new_text, "Change me");  
        while( (dim=select_a_dimension("Select a Dimension")) != NULL_TAG)  
        {  
            WRITE_D(dim);  
            UF_CALL(UF_DRF_ask_appended_text(dim, &n_strings, &appended_text));  
            WRITE_D(n_strings);  
            for(ii =0; ii < n_strings; ii++)  
            {  
                ECHO("%s:\n", location[appended_text[ii].location]);  
                for(jj=0; jj<appended_text[ii].num_lines; jj++)  
                {  
                    ECHO("%s\n", appended_text[ii].text[jj]);  
                    strcpy(new_text, appended_text[ii].text[jj]);  
                    if( prompt_for_text("Enter new text or cancel", new_text) )  
                    {  
                        strcpy( appended_text[ii].text[jj], new_text);  
                        UF_CALL(UF_DRF_set_appended_text(dim, n_strings, appended_text));  
                    }  
                }  
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

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 包含了NX二次开发常用的头文件，如uf.h、uf_ui.h等，用于调用NX的API函数。
> 2. 定义了一个宏UF_CALL，用于调用NX API函数，并报告错误信息。
> 3. 定义了一个宏WRITE_D，用于输出变量值到窗口。
> 4. 定义了一个函数mask_for_dimensions，用于设置选择过滤条件，仅选择尺寸标注。
> 5. 定义了一个函数select_a_dimension，用于选择一个尺寸标注。
> 6. 定义了一个函数prompt_for_text，用于获取用户输入的文本。
> 7. 定义了一个函数do_it，用于实现主要功能：循环选择尺寸标注。获取每个尺寸标注的附加文本。输出附加文本。修改文本并更新尺寸标注。
> 8. 循环选择尺寸标注。
> 9. 获取每个尺寸标注的附加文本。
> 10. 输出附加文本。
> 11. 修改文本并更新尺寸标注。
> 12. 定义了ufusr函数，是NX二次开发的入口函数，调用UF_initialize初始化，调用do_it函数执行功能，最后调用UF_terminate结束。
> 13. 定义了ufusr_ask_unload函数，用于卸载二次开发应用程序。
>
> 总的来说，这段代码实现了在NX中循环选择尺寸标注，获取每个标注的附加文本，允许用户修改文本并更新标注的功能。
>
