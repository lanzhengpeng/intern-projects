### 【1283】find file using search directories 在搜索目录中查找文件

#### 代码

```cpp
    /*HEAD FIND_FILE_USING_SEARCH_DIRECTORIES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_assem.h>  
    #define WRITE(X)      (UF_UI_write_listing_window(X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog 是在 V18 中新增的，只需提供翻译，不要添加无关内容。 */  
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
    static logical is_file_there(char *file_spec)  
    {  
        int  
            missing = TRUE;  
        UF_CALL(UF_CFI_ask_file_exist(file_spec, &missing));  
        if (missing)  
            return FALSE;  
        else  
            return TRUE;  
    }  
    static logical search_search_directories(char *short_name, char *full_name)  
    {  
        logical  
            found = FALSE,  
            *sub_dir;  
        int  
            count,  
            ii,  
            type,  
            resp;  
        char  
            **dir_list,  
            fspec[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_ASSEM_ask_search_directories(&count, &dir_list, &sub_dir));  
        for (ii = 0; ((ii < count) && (!found)); ii++)  
        {  
            if (sub_dir[ii])  
            {  
                UF_CALL(uc4508(dir_list[ii],(1<<10)|(1<<11)|(1<<13),0,short_name));  
                while ((resp = uc4518()) < 2 && (resp > -1))  
                {  
                    switch (resp)  
                    {  
                        case 0:  
                            type = uc4601();  
                            if (type == 100)  
                                uc4509();  
                            else  
                            {  
                                UF_CALL(uc4519(fspec));  
                                strcpy(full_name, fspec);  
                                found = TRUE;  
                            }  
                            break;  
                        case 1:  
                            UF_CALL(uc4549());  
                            break;  
                        default:  
                            UF_CALL(resp);  
                            break;  
                    }  
                }  
                UF_CALL(uc4548());  
            }  
            else  
            {  
                UF_CALL(uc4575(dir_list[ii], 0, short_name, fspec));  
                if (is_file_there(fspec))  
                {  
                    found = TRUE;  
                    strcpy(full_name, fspec);  
                }  
            }  
        }  
        UF_free_string_array(count, dir_list);  
        UF_free(sub_dir);  
        return found;  
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
        char  
            file_name[MAX_FSPEC_SIZE+1] = { "" },  
            fspec[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        while (prompt_for_text("Enter file to find", file_name))  
        {  
            if (search_search_directories(file_name, fspec))  
            {  
                WRITE("FOUND ");  
                WRITE(fspec);  
                WRITE("\n");  
            }  
            else  
            {  
                WRITE("Did NOT find ");  
                WRITE(file_name);  
                WRITE("\n");  
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

> 这段NX二次开发代码的功能是搜索NX打开的部件文件所在的目录树，以查找用户指定的文件是否存在。以下是代码的主要组成部分：
>
> 1. 头文件包含：引入了标准C库、NX Open C++库、NX Open UI库、NX Open CFI库和NX Open Assem库的头文件，这些库提供了在NX环境中进行C++开发所需的接口。
> 2. 错误处理函数：report_error函数用于在调用NX Open API时捕获错误，并输出错误信息。
> 3. 文件存在性检查函数：is_file_there函数用于检查指定路径的文件是否存在。
> 4. 目录树搜索函数：search_search_directories函数遍历NX打开的部件文件所在的目录树，搜索用户指定的文件是否存在，并返回完整的文件路径。
> 5. 文本提示函数：prompt_for_text函数用于在NX界面中提示用户输入要查找的文件名。
> 6. 主函数：do_it函数循环提示用户输入要查找的文件名，然后调用search_search_directories函数进行搜索，并输出结果。
> 7. NX Open入口函数：ufusr函数是NX Open的入口点，在这里初始化NX Open环境，调用do_it函数，然后清理NX Open环境。
> 8. 卸载函数：ufusr_ask_unload函数用于处理插件卸载时的清理工作。
>
> 综上所述，这段代码实现了在NX环境中查找指定文件的功能，通过NX Open API调用与用户交互，实现了在NX打开的部件文件所在的目录树中搜索文件的功能。
>
