### 【2521】save all parts and export jt files 保存所有零件并导出JT文件

#### 代码

```cpp
    /*HEAD SAVE_ALL_PARTS_AND_EXPORT_JT_FILES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据上下文，可以翻译为：

注意：UF_print_syslog 是 V18 新增的功能，请只提供翻译，不要添加其他无关内容。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static void create_jt_of_all_open_parts(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            part;  
        char  
            msg[133],  
            cmd[UF_UI_MAX_STRING_LEN+1],  
            pv_dir[UF_CFI_MAX_PATH_NAME_SIZE],  
            prog_name[UF_CFI_MAX_PATH_NAME_SIZE],     
            *base_dir,  
            part_fspec[MAX_FSPEC_SIZE+1];  
        n = UF_PART_ask_num_parts();  
        sprintf(msg, "Initiating JT conversion for %d parts", n);  
        uc1601(msg, TRUE);  
        for (ii = 0; ii < n; ii++)  
        {  
            part = UF_PART_ask_nth_part(ii);  
            UF_PART_ask_part_name(part, part_fspec);  
            printf("Part tag %d - %s\n", part, part_fspec);  
            UF_CALL(UF_translate_variable("UGII_BASE_DIR", &base_dir));  
            UF_CALL(uc4575(base_dir, 100, "pvtrans", pv_dir));  
            UF_CALL(uc4575(pv_dir, 0, "ugtopv", prog_name));  
            sprintf(cmd, "%s %s", prog_name, part_fspec);  
            system(cmd);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            count = 0,  
            *error_list,  
            ii;  
        tag_t  
            *part_list;  
        char  
            fspec[MAX_FSPEC_SIZE+1],  
            msg[133];  
        UF_CALL(UF_PART_save_all(&count, &part_list, &error_list));  
        WRITE_D(count);  
        if (count > 0)  
        {  
            WRITE_D(count);  
            for (ii = 0; ii < count; ii++)  
            {  
                WRITE_D(ii);  
                WRITE_D(part_list[ii]);  
                UF_PART_ask_part_name(part_list[ii], fspec);  
                WRITE_S(fspec);  
                WRITE_D(error_list[ii]);  
                UF_get_fail_message(error_list[ii], msg);  
                WRITE_S(msg);  
            }  
            UF_free(part_list);  
            UF_free(error_list);  
            create_jt_of_all_open_parts();  
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

> 这段代码是一个NX的二次开发UF程序，主要用于保存所有打开的零件并导出为JT文件。以下是代码的主要功能：
>
> 1. 错误报告宏：定义了错误报告宏UF_CALL，用于在函数调用失败时输出错误信息。
> 2. 输出函数：定义了整数和字符串的输出函数WRITE_D和WRITE_S，用于在列表窗口中输出信息。
> 3. 创建JT文件：定义了函数create_jt_of_all_open_parts，用于遍历所有打开的零件，并调用外部程序将每个零件保存为JT文件。
> 4. 保存零件：定义了函数do_it，用于保存所有打开的零件，并输出保存的零件数、零件名、错误码等信息。
> 5. UFusr函数：定义了UFusr函数，用于在NX启动时执行do_it函数，实现保存所有零件和导出JT文件的功能。
> 6. 卸载函数：定义了卸载函数ufusr_ask_unload，用于在卸载插件时返回立即卸载标志。
>
> 总体来说，这段代码通过调用NX的UF函数，实现了保存所有打开的零件，并调用外部程序将每个零件导出为JT文件的功能。代码结构清晰，功能明确。
>
