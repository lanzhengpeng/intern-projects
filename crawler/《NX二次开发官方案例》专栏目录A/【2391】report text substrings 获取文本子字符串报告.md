### 【2391】report text substrings 获取文本子字符串报告

#### 代码

```cpp
    /*HEAD REPORT_TEXT_SUBSTRINGS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据提供的信息，UF_print_syslog在V18版本中是新增的。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE2F(X)  (write_double_array_to_listing_window(#X, X, 1, 2))  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE6F(X)  (write_double_array_to_listing_window(#X, X, 2, 3))  
    #define WRITE9F(X)  (write_double_array_to_listing_window(#X, X, 3, 3))  
    #define WRITE16F(X) (write_double_array_to_listing_window(#X, X, 4, 4))  
    #define WRITEnF(n,X)(write_double_array_to_listing_window(#X, X, 1, n))  
    #define WRITEnF4(X,n) (write_double_array_to_listing_window(#X, X, n, 4))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
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
    static int mask_for_draft_aids(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_drafting_entity_type, UF_all_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_draft_aid(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_draft_aids, NULL, &resp,  
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
        int  
            ann_data[10],  
            cycle = 0,  
            data_form,  
            data_type,  
            ii,  
            jj,  
            n_bytes,  
            n_segs,  
            n_substr,  
            text_data[4] = { 0, 0, 1, 0 },  
            text_type;  
        tag_t  
            object;  
        double  
            origin[2],  
            rad,  
            sub_chars[6],  
            sub_org[2];  
        char  
            substr[UF_DRF_MAX_FONT_LEN+1];  
        while ((object = select_a_draft_aid("Select text object")) != NULL_TAG)  
        {  
            UF_CALL(UF_DRF_set_draft_common(&object));  
            while (!UF_CALL(UF_DRF_ask_ann_data(&object, text_data, &cycle,  
                ann_data, &data_type, &data_form, &n_segs, origin, &rad)) &&  
                (cycle != 0))  
            {  
                WRITE_D(n_segs);  
                for (ii = 0; ii < n_segs; ii++)  
                {  
                    WRITE_D(ii);  
                    UF_CALL(UF_DRF_count_text_substring(&ii, ann_data, &n_substr));  
                    WRITE_D(n_substr);  
                    for (jj = 0; jj < n_substr; jj++)  
                    {  
                        UF_CALL(UF_DRF_get_text_substring(&ii, ann_data, &jj,  
                            &text_type, &n_bytes, substr, sub_org, sub_chars));  
                        WRITE_D(jj);  
                        WRITE_D(text_type);  
                        WRITE_D(n_bytes);  
                        WRITE_S(substr);  
                        WRITE2F(sub_org);  
                        WRITE6F(sub_chars);  
                    }  
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

> 这段代码是用于NX的二次开发，其主要功能是从NX的文本对象中提取文本子字符串。
>
> 主要函数和流程如下：
>
> 1. ufusr函数：这是NX二次开发的入口函数。在这里，首先调用UF_initialize()进行初始化，然后调用do_it()函数执行主要逻辑，最后调用UF_terminate()进行清理。
> 2. do_it函数：这个函数不断调用select_a_draft_aid()，让用户选择文本对象，然后提取文本信息。
> 3. select_a_draft_aid函数：这个函数使用UF_UI_select_with_single_dialog()让用户选择一个文本对象，并返回其tag。
> 4. UF_DRF_set_draft_common函数：设置当前文本对象为活动对象。
> 5. UF_DRF_ask_ann_data函数：获取文本对象的注释信息，包括文本行数、文本类型、数据格式等。
> 6. UF_DRF_count_text_substring函数：获取指定行的文本子字符串数量。
> 7. UF_DRF_get_text_substring函数：获取指定行指定索引的文本子字符串。
> 8. WRITE_XXX系列宏：这些宏用于将提取到的文本信息输出到日志窗口。
> 9. UF_print_syslog函数：将错误信息输出到系统日志。
> 10. report_error函数：用于处理错误。
>
> 总的来说，这段代码实现了从NX的文本对象中提取文本子字符串的功能，并将提取结果输出到日志窗口，供用户查看。
>
