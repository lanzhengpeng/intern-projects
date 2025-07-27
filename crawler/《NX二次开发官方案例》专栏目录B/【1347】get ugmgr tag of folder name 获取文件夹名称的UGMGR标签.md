### 【1347】get ugmgr tag of folder name 获取文件夹名称的UGMGR标签

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ugmgr.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf(msg, UF_UI_MAX_STRING_LEN, format, args);  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static UF_UGMGR_tag_t find_folder(UF_UGMGR_tag_t look_in, char *find_name)  
    {  
        int  
            ii,  
            count;  
        UF_UGMGR_tag_t  
            found_folder = UF_UGMGR_null_tag,  
            *folder_contents;  
        UF_UGMGR_object_type_t  
            obj_type;  
        char  
            this_name[UF_UGMGR_NAME_SIZE + 1];  
        UF_CALL(UF_UGMGR_list_folder_contents(look_in, &count, &folder_contents));  
        for (ii = 0; ii < count; ii++)  
        {  
            UF_CALL(UF_UGMGR_ask_object_type(folder_contents[ii], &obj_type));  
            if (obj_type == UF_UGMGR_type_folder)  
            {  
                UF_CALL(UF_UGMGR_ask_folder_name(folder_contents[ii], this_name));  
                if (!strcmp(this_name, find_name))  
                {  
                    found_folder = folder_contents[ii];  
                    break;  
                }  
                found_folder = find_folder(folder_contents[ii], find_name);  
                if (found_folder != UF_UGMGR_null_tag) break;  
            }  
        }  
        UF_free(folder_contents);  
        return found_folder;  
    }  
    static UF_UGMGR_tag_t ask_tag_of_folder_name(char *folder_name)  
    {  
        UF_UGMGR_tag_t  
            root_folder;  
        char  
            root_name[UF_UGMGR_NAME_SIZE + 1] = { "" };  
        UF_CALL(UF_UGMGR_ask_root_folder(&root_folder));  
        UF_CALL(UF_UGMGR_ask_folder_name(root_folder, root_name));  
        if (!strcmp(root_name, folder_name)) return root_folder;  
        return find_folder(root_folder, folder_name);  
    }  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    static void do_it(void)  
    {  
        char  
            look_for[UF_UGMGR_NAME_SIZE + 1] = { "Newstuff" },  
            found_name[UF_UGMGR_NAME_SIZE + 1] = { "" };  
        UF_UGMGR_tag_t  
            found_folder;  
        while (prompt_for_text("Enter name of folder", look_for))  
        {  
            WRITE_S(look_for);  
            found_folder = ask_tag_of_folder_name(look_for);  
            WRITE_D(found_folder);  
            if (found_folder != UF_UGMGR_null_tag)  
            {  
                UF_CALL(UF_UGMGR_ask_folder_name(found_folder, found_name));  
                WRITE_S(found_name);  
            }  
            else  
            {  
                ECHO("Not found.\n");  
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

> 这段代码是NX Open的二次开发示例，主要功能是查找NX文件系统中的文件夹。以下是代码的主要功能和流程：
>
> 1. 定义了ECHO宏，用于打印调试信息到系统日志和列表窗口。
> 2. 定义了UF_CALL宏，用于调用NX Open函数，并在出错时报告错误信息。
> 3. 定义了prompt_for_text函数，用于提示用户输入文件夹名称。
> 4. 定义了find_folder函数，递归地在指定文件夹及其子文件夹中查找指定名称的文件夹。
> 5. 定义了ask_tag_of_folder_name函数，从根文件夹开始查找指定名称的文件夹。
> 6. 定义了WRITE_D和WRITE_S宏，用于打印调试信息。
> 7. 定义了write_string_to_listing_window函数，用于打印字符串到列表窗口。
> 8. 定义了do_it函数，主函数，循环提示用户输入文件夹名称，并调用ask_tag_of_folder_name函数查找，打印结果。
> 9. 定义了ufusr函数，NX Open的入口函数，初始化后调用do_it，然后终止。
> 10. 定义了ufusr_ask_unload函数，NX Open卸载时调用。
>
> 通过以上流程，代码实现了在NX文件系统中查找指定名称的文件夹的功能。
>
