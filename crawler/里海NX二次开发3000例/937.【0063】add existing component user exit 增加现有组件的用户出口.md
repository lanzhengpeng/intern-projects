### 【0063】add existing component user exit 增加现有组件的用户出口

#### 代码

```cpp
    /*HEAD ADD_EXISTING_COMPONENT_USER_EXIT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_exit.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文中内容，UF_print_syslog是V18版本新增的功能。 */  
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
    /* 里海 */  
    void ufrcp(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        prompt_for_part_name("Component to Add?", param);  
        *retcode = 3;  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这是段NX Open C++二次开发代码，主要实现了添加组件的功能。
>
> 代码主要分为以下几个部分：
>
> 1. 头文件包含：引入了必要的NX Open C++库头文件。
> 2. 错误报告函数：定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 3. 目录和通配符组合函数：定义了一个组合目录和通配符的函数combine_directory_and_wildcard。
> 4. 提示输入部件名称函数：定义了一个提示用户输入部件名称的函数prompt_for_part_name。
> 5. 主函数：定义了一个主函数ufrcp，用于初始化NX环境，提示用户选择要添加的部件，并设置返回码为3。
> 6. 卸载函数：定义了一个卸载函数ufusr_ask_unload，用于在组件卸载时立即释放资源。
>
> 通过这些函数，代码实现了添加组件的功能。用户会看到一个文件选择框，可以选择要添加的部件。选择后，部件会被添加到当前NX装配中。
>
