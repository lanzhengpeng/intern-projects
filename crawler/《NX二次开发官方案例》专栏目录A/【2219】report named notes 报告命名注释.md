### 【2219】report named notes 报告命名注释

#### 代码

```cpp
    /*HEAD REPORT_NAMED_NOTES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_drf.h>  
    #include <uf_view.h>  
    #include <uf_ui.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n*** ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的函数。因此，我的回答是：UF_print_syslog是V18版本新增的函数。 */  
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
    #define WRITE2F(X)  (write_double_array_to_listing_window(#X, X, 1, 2))  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE9F(X)  (write_double_array_to_listing_window(#X, X, 3, 3))  
    #define WRITE16F(X) (write_double_array_to_listing_window(#X, X, 4, 4))  
    #define WRITEnF(X,n)(write_double_array_to_listing_window(#X, X, 1, n))  
    #define WRITEnF4(X,n) (write_double_array_to_listing_window(#X, X, n, 4))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            jj,  
            n,  
            status,  
            subtype,  
            type;  
        tag_t  
            a_note = NULL_TAG;  
        char  
            msg[133] = { "" },  
            name[UF_OBJ_NAME_LEN + 1],  
            view[UF_OBJ_NAME_LEN + 1];  
        UF_DRF_draft_aid_text_info_t  
            *text_info;  
        UF_UI_open_listing_window();  
        while (prompt_for_text("Enter note name", name))  
        {  
            while (!UF_CALL(UF_OBJ_cycle_by_name(name, &a_note)) &&  
                   (a_note != NULL_TAG))  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(a_note, &type, &subtype));  
                if ((type == UF_drafting_entity_type) &&  
                    (subtype == UF_draft_note_subtype))  
                {  
                    UF_CALL(uc6409(a_note, &status, view));  
                    sprintf(msg, "note named '%s' found on %s\n", name, view);  
                    UF_UI_write_listing_window( msg );  
                    UF_CALL(UF_DRF_ask_draft_aid_text_info(a_note, &n, &text_info));  
                    for (ii=0; ii<n; ii++)  
                    {  
                        WRITE3F(text_info[ii].origin);  
                        for (jj=0; jj<text_info[ii].num_lines; jj++)  
                        {  
                            sprintf(msg, "    %s\n", text_info[ii].text[jj].string);  
                            UF_UI_write_listing_window( msg );  
                        }  
                    }  
                    UF_CALL(UF_DRF_free_text(n, &text_info));  
                }  
            }  
        }  
    }  
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

> 根据代码的内容，这是一段用于在NX中查询和显示注释的二次开发代码。以下是代码的主要功能：
>
> 1. 包含必要的NX Open头文件，用于调用NX的函数。
> 2. 定义了宏UF_CALL，用于在调用NX函数前打印函数名，并在调用后检查返回码，如果有错误则打印错误信息。
> 3. 定义了宏WRITE2F等，用于将二维数组打印到列表窗口。
> 4. 定义了函数write_double_array_to_listing_window，用于将二维数组打印到列表窗口。
> 5. 定义了函数prompt_for_text，用于获取用户输入的文本。
> 6. 定义了函数do_it，是代码的核心逻辑：循环提示用户输入注释名称。根据名称查询注释对象。检查对象类型和子类型是否为注释。获取注释所在的视图名称。获取注释的文本信息。打印注释的文本信息到列表窗口。
> 7. 循环提示用户输入注释名称。
> 8. 根据名称查询注释对象。
> 9. 检查对象类型和子类型是否为注释。
> 10. 获取注释所在的视图名称。
> 11. 获取注释的文本信息。
> 12. 打印注释的文本信息到列表窗口。
> 13. 定义了ufusr函数，是NX二次开发的入口函数，在初始化NX后调用do_it函数，然后终止NX。
> 14. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总体来说，这段代码通过NX的API实现了查询和显示指定名称的注释的功能。
>
