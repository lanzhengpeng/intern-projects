### 【1814】put selected solids in stl file 将选择的实体放入STL文件

#### 代码

```cpp
    /*HEAD PUT_SELECTED_SOLIDS_IN_STL_FILE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_csys.h>  
    #include <uf_modl.h>  
    #include <uf_cfi.h>  
    #include <uf_std.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的描述，UF_print_syslog是一个新函数，在V18版本中引入。该函数的用途是打印系统日志消息。 */  
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
    static logical prompt_for_two_numbers(char *prompt, char *item1, char *item2,  
        double *number1, double *number2)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[2][16];  
        double  
            da[2];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        menu[0][15] = '\0';  
        menu[1][15] = '\0';  
        da[0] = *number1;  
        da[1] = *number2;  
        resp = uc1609(prompt, menu, 2, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void combine_directory_and_wildcard(char *dir, char *fltr, char *spec)  
    {  
        if (!strcmp(dir, ""))  
            strcpy(spec, fltr);  
        else  
        {  
            UF_CALL(uc4575(dir, 0, "junk.xxx", spec));  
            strcpy(strstr(spec, "junk.xxx"), fltr);  
        }  
    }  
    static logical prompt_for_stl_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            *stl_dir,  
            *stl_ext,  
            filter[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_EXPORT_DIR, &stl_dir));  
        UF_CALL(UF_UI_ask_dialog_filter(UF_UI_EXPORT_RPT_FLTR, &stl_ext));  
        combine_directory_and_wildcard(stl_dir, stl_ext, filter);  
        UF_free(stl_dir);  
        UF_free(stl_ext);  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Export Rapid-Prototyping",  
            filter, "", fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
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
    /* qq3123197280 */  
    static int filter_body_type(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            body_type;  
        UF_CALL(UF_MODL_ask_body_type(object, &body_type));  
        if (body_type == *(int *)data)  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    static int mask_body_type( UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_body_type, NULL, type)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_solids(char *prompt, tag_t **solids)  
    {  
        int  
            ii,  
            n,  
            solid = UF_MODL_SOLID_BODY,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog( "Select solids", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &solid, &resp, &n, solids));  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight((*solids)[ii], FALSE));  
        return n;  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
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
    static void report_stl_errors(int n_errors, UF_STD_stl_error_t *error_info)  
    {  
        int  
            ii;  
        char  
            error_text[133];  
        WRITE_D(n_errors);  
        for (ii = 0; ii < n_errors; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_D(error_info[ii].error_code);  
            UF_get_fail_message(error_info[ii].error_code, error_text);  
            WRITE_S(error_text);  
            WRITE_D(error_info[ii].related);  
            WRITE3F(error_info[ii].point1);  
            WRITE3F(error_info[ii].point2);  
        }  
        if (n_errors > 0) UF_free(error_info);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_errors,  
            n_solids;  
        tag_t  
            *solids;  
        double  
            max_edge = 100,  
            tol = 0.05;  
        char  
            bfn[UF_CFI_MAX_PATH_NAME_SIZE],  
            header[MAX_LINE_SIZE+1] = { "" };  
        void  
            *file;  
        UF_STD_stl_error_p_t  
            error_info;  
        if (prompt_for_two_numbers("Rapid-Prototyping", "Max edge length",  
                "Triangle Tol", &max_edge, &tol) &&  
            prompt_for_stl_file_name("Enter Rapid-Prototyping Output File Name",  
                bfn) &&  
            prompt_for_text("Enter file header information", header) &&  
            ((n_solids =  
                select_solids("Select solids for Rapid-Prototyping", &solids)) > 0))  
        {  
            UF_CALL(UF_STD_open_binary_stl_file(bfn, FALSE, header, &file));  
            for (ii = 0; ii < n_solids; ii++)  
            {  
                UF_CALL(UF_STD_put_solid_in_stl_file(file, NULL_TAG, solids[ii], 0,  
                    max_edge, tol, &n_errors, &error_info));  
                report_stl_errors(n_errors, error_info);  
            }  
            UF_CALL(UF_STD_close_stl_file(file));  
            UF_free(solids);  
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

> 这段代码是一个NX的二次开发示例，主要功能是将用户选择的实体保存到STL文件中。以下是代码的主要逻辑：
>
> 1. 包含了必要的NX Open API头文件。
> 2. 定义了一个错误处理函数report_error，用于打印错误信息。
> 3. 定义了一个提示输入两个数字的函数prompt_for_two_numbers。
> 4. 定义了一个组合目录和通配符的函数combine_directory_and_wildcard。
> 5. 定义了一个提示输入STL文件名的函数prompt_for_stl_file_name。
> 6. 定义了一个提示输入文本的函数prompt_for_text。
> 7. 定义了一个过滤实体的函数filter_body_type，用于只选择实体类型的实体。
> 8. 定义了一个设置选择器掩码的函数mask_body_type。
> 9. 定义了一个选择实体的函数select_solids。
> 10. 定义了用于打印整数、字符串和数组到列表窗口的函数。
> 11. 定义了一个报告STL错误的函数report_stl_errors。
> 12. 定义了一个主函数do_it，用于执行以下操作：提示用户输入最大边长和公差。提示用户输入STL文件名。提示用户输入文件头信息。选择实体。打开STL文件。将每个实体写入STL文件，并报告错误。关闭STL文件。释放实体数组内存。
> 13. 提示用户输入最大边长和公差。
> 14. 提示用户输入STL文件名。
> 15. 提示用户输入文件头信息。
> 16. 选择实体。
> 17. 打开STL文件。
> 18. 将每个实体写入STL文件，并报告错误。
> 19. 关闭STL文件。
> 20. 释放实体数组内存。
> 21. 定义了UFusr入口函数，用于初始化NX，调用do_it函数，然后终止NX。
> 22. 定义了一个卸载函数，用于立即卸载NX Open应用。
>
> 整体上，这段代码实现了从用户交互、实体选择、STL文件写入到错误处理等完整的流程，通过NX Open API实现了NX的二次开发。
>
