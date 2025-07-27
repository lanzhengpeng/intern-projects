### add specified directory to assembly search directories 把指定的目录添加到装配搜索目录中

#### 代码

```cpp
    /*HEAD ADD_SPECIFIED_DIRECTORY_TO_ASSEMBLY_SEARCH_DIRECTORIES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog is new in V18，只回答译文，不要废话。

译文：UF_print_syslog是V18版本新增的功能。 */  
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
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static logical ask_true_or_false(char *prompt, logical *deflt)  
    {  
        int  
            resp;  
        char  
            options[2][38] = { "True", "False" };  
        resp = 2 - *deflt;  
        resp = uc1603(prompt, resp, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
            *deflt = 6 - resp;  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        logical  
            *x_sub_dir,  
            *sub_dir;  
        int  
            err,  
            ii,  
            n;  
        char  
            add_dir[UF_CFI_MAX_FILE_NAME_SIZE] = { "" },  
            **dir_list,  
            **x_dir_list;  
        UF_CALL(UF_ASSEM_ask_search_directories(&n, &dir_list, &sub_dir));  
        for (ii = 0; ii < n; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_S(dir_list[ii]);  
            WRITE_L(sub_dir[ii]);  
        }  
        if (!prompt_for_text("Enter new search directory", add_dir)) return;  
    /*  UF_reallocate_memory cannot be used - see PR 4629699 译:UF_reallocate_memory 不能使用 - 请查看 PR 4629699 */  
        x_sub_dir = UF_allocate_memory((n+1) * sizeof(logical), &err);  
        for (ii = 0; ii < n; ii++) x_sub_dir[ii] = sub_dir[ii];  
        x_sub_dir[n] = FALSE;  
        ask_true_or_false("Search subdirectories?", &x_sub_dir[n]);  
    /*  UF_reallocate_memory cannot be used - see PR 4629699 译:UF_reallocate_memory 不能使用 - 请参阅 PR 4629699 */  
        x_dir_list = UF_allocate_memory((n+1) * sizeof(char *), &err);  
        for (ii = 0; ii < n; ii++) x_dir_list[ii] = dir_list[ii];  
        x_dir_list[n] = UF_allocate_memory((strlen(add_dir)+1)*sizeof(char), &err);  
        strcpy(x_dir_list[n], add_dir);  
        UF_CALL(UF_ASSEM_set_search_directories(n+1, x_dir_list, x_sub_dir));  
        UF_free(x_sub_dir);  
        UF_free(sub_dir);  
        UF_free(dir_list);  /* the strings will be free'd by the next call 译:在下一个调用中将会释放这些字符串。 */  
        UF_free_string_array(n+1, x_dir_list);  
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

> 这是NX二次开发代码，其主要功能是向NX的装配搜索目录中添加指定的目录。具体代码功能如下：
>
> 1. 包含了必要的头文件，如uf.h、uf_ui.h、uf_assem.h等，这些头文件包含了NX API的相关声明。
> 2. 定义了一个错误报告函数report_error，用于在发生错误时打印错误信息。
> 3. 定义了一些宏，用于将整数、字符串、逻辑值写入列表窗口。
> 4. 定义了一个获取用户输入文本的函数prompt_for_text。
> 5. 定义了一个询问用户选择真或假的函数ask_true_or_false。
> 6. 定义了主要功能函数do_it，该函数首先获取当前的装配搜索目录列表，然后提示用户输入新的搜索目录，并将其添加到列表中。
> 7. 定义了ufusr函数，这是NX二次开发程序的入口函数。在这个函数中，首先调用UF_initialize进行初始化，然后调用do_it执行主要功能，最后调用UF_terminate终止。
> 8. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示二次开发程序可以立即被卸载。
>
> 总体来说，这段代码通过NX的装配搜索目录API，实现了向其添加新目录的功能。代码结构清晰，使用了一些宏和辅助函数，提高了可读性和可维护性。
>
