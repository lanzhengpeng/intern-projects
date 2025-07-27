### 【2265】report part attributes without opening part 列出部件属性但不打开部件

#### 代码

```cpp
    /*HEAD REPORT_PART_ATTRIBUTES_WITHOUT_OPENING_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_attr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的描述，UF_print_syslog是V18版本新增的功能。因此，文档中提到UF_print_syslog是V18新增的功能，需要翻译为中文。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
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
    static logical prompt_for_part_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE+1],  
            *p_dir,  
            *p_ext;  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_PART_DIR, &p_dir));  
        UF_CALL(UF_UI_ask_dialog_filter(UF_UI_PART_OPEN_FLTR, &p_ext));  
        combine_directory_and_wildcard(p_dir, p_ext, filter);  
        UF_free(p_dir);  
        UF_free(p_ext);  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Part Name", filter, "", fspec,  
            &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_attrs;  
        UF_ATTR_part_attr_p_t  
            attrs;  
        char  
            msg[UF_UI_MAX_STRING_LEN],  
            part_name[MAX_FSPEC_SIZE+1] = { "" };  
        while (prompt_for_part_name("List attributes", part_name))  
        {  
            UF_CALL(UF_ATTR_ask_part_attrs_in_file(part_name,&n_attrs,&attrs));  
            sprintf(msg, "%s has %d attributes\n", part_name, n_attrs);  
            WRITE(msg);  
            for (ii = 0; ii < n_attrs; ii++)  
            {  
                sprintf(msg, "  %d.  %s = %s\n", ii+1, attrs[ii].title,  
                    attrs[ii].string_value);  
                WRITE(msg);  
            }  
            if (n_attrs > 0) UF_free(attrs);  
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

> 这是一段NX二次开发的示例代码，主要功能是列出指定零件文件的所有属性。
>
> 代码主要流程如下：
>
> 1. 包含必要的NX API头文件，如uf.h、uf_ui.h等。
> 2. 定义宏UF_CALL，用于调用NX API，并在出错时报告错误。
> 3. 定义report_error函数，用于在出错时打印错误信息。
> 4. 定义combine_directory_and_wildcard函数，用于组合目录和通配符以生成文件选择器。
> 5. 定义prompt_for_part_name函数，用于提示用户输入零件文件名。
> 6. 定义do_it函数，用于列出指定零件文件的所有属性。
> 7. 定义ufusr函数，这是NX二次开发的主入口函数。在该函数中，首先初始化NX环境，然后调用do_it函数执行主功能，最后终止NX环境。
> 8. 定义ufusr_ask_unload函数，用于卸载二次开发模块。
>
> 总的来说，该代码通过NX API实现了读取零件文件属性并打印输出的功能，是一个典型的NX二次开发示例。
>
