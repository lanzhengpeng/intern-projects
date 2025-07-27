### 【1906】report all dimension values 报告所有尺寸值

#### 代码

```cpp
    /*HEAD REPORT_ALL_DIMENSION_VALUES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_drf.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中的内容，可以翻译为：

注意：UF_print_syslog 是V18中新增的函数，请只回答翻译，不要添加无关内容。 */  
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
    static tag_t ask_next_dim(tag_t part, tag_t dim)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_dimension_type, &dim))  
            && (dim != NULL_TAG))  
        {  
            if (!UF_ASSEM_is_occurrence(dim)) return dim;  
        }  
        return NULL_TAG;  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            jj,  
            subtype;  
        tag_t  
            dim = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
            origin[3];  
        UF_DRF_dim_info_t  
            *info;  
        while ((dim = ask_next_dim(part, dim)) != NULL_TAG)  
        {  
            UF_CALL(UF_DRF_ask_dim_info(dim, &subtype, origin, &info));  
            for (ii = 0; ii<info->num_text; ii++)  
                if (info->text_info[ii].text_type == UF_DRF_DIM_TEXT)  
                    for (jj = 0; jj < info->text_info[ii].num_lines; jj++)  
                        WRITE_S(info->text_info[ii].text[jj].string);  
            UF_CALL(UF_DRF_free_dimension(&info));  
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

> 这是一段用于报告NX部件中所有尺寸值的二次开发代码。其主要功能包括：
>
> 1. 定义了报告错误函数report_error，用于输出函数调用失败时的错误信息。
> 2. 定义了ask_next_dim函数，用于遍历部件中的所有尺寸特征，并返回下一个非实例尺寸特征的标签。
> 3. 定义了WRITE_S宏，用于将字符串写入列表窗口。
> 4. do_it函数是主函数，用于遍历部件中的所有尺寸特征，并输出每个尺寸特征的文本信息。
> 5. ufusr函数是NX的回调函数，用于初始化NX，调用do_it函数，然后终止NX。
> 6. ufusr_ask_unload函数用于卸载用户函数。
>
> 总体来说，该代码的主要功能是遍历NX部件，报告所有尺寸特征的文本信息，并输出到列表窗口中。
>
