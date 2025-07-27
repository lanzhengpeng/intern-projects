### 【1542】list root folder contents 列出根文件夹内容

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ugmgr.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
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
    static void list_in_folder(UF_UGMGR_tag_t folder, char *indent)  
    {  
        int  
            ii, jj,  
            count,  
            revision_count;  
        UF_UGMGR_tag_t  
            *folder_contents,  
            *revisions;  
        UF_UGMGR_object_type_t  
            obj_type;  
        char  
            folder_name[UF_UGMGR_NAME_SIZE + 1],  
            part_number[UF_UGMGR_NAME_SIZE + 1],  
            revision_id[UF_UGMGR_NAME_SIZE + 1];  
        if (folder == UF_UGMGR_null_tag) return;  
        UF_CALL(UF_UGMGR_ask_folder_name(folder, folder_name));  
        ECHO("%sFolder: %s\n", indent, folder_name);  
        strcat(indent, " ");  
    /*  The only difference between the two functions below is that the first  
        returns folders and the second does not.  
        Use UF_UGMGR_list_folder_contents if you want a recursive list.  
        Use UF_UGMGR_list_part_in_folder if you don't.  
    */  
        UF_CALL(UF_UGMGR_list_folder_contents(folder, &count, &folder_contents));  
        // UF_CALL(UF_UGMGR_list_parts_in_folder(folder, &count, &folder_contents));  
        for (ii = 0; ii < count; ii++)  
        {  
            UF_CALL(UF_UGMGR_ask_object_type(folder_contents[ii], &obj_type));  
            switch(obj_type)  
            {  
                case UF_UGMGR_type_folder :  
                    list_in_folder(folder_contents[ii], indent);  
                    break;  
                case UF_UGMGR_type_part :  
                    UF_CALL(UF_UGMGR_ask_part_number(folder_contents[ii], part_number));  
                    ECHO("%s Part: %s\n", indent, part_number);  
                    UF_CALL(UF_UGMGR_list_part_revisions(folder_contents[ii],  
                        &revision_count, &revisions));  
                    for (jj = 0; jj < revision_count; jj++)  
                    {  
                        UF_CALL(UF_UGMGR_ask_part_revision_id(revisions[jj], revision_id));  
                        ECHO("%s  PartRevision: %s/%s\n", indent, part_number, revision_id);  
                    }  
                    break;  
            // This case never happens! So get the part revisions as above if possible.  
            // See PR 7347269.  
                case UF_UGMGR_type_part_revision :  
                    UF_CALL(UF_UGMGR_ask_part_revision_id(folder_contents[ii],  
                        revision_id));  
                    ECHO("%s Part revision: %s\n", indent, revision_id);  
                    break;  
                default :  
                    ECHO("%s Found unknown type: %d\n", indent, obj_type);  
                    break;  
            }  
        }  
        if (count > 0) UF_free(folder_contents);  
        indent[strlen(indent) - 1] = '\0';  
        return;  
    }  
    static void do_it(void)  
    {  
        char  
            indent[MAX_LINE_SIZE+1] = { "" };  
        UF_UGMGR_tag_t  
            root;  
        if (!UF_CALL(UF_UGMGR_ask_root_folder(&root))) list_in_folder(root, indent);  
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
    int main( int argc, char *argv[] )  
    {  
        if (UF_CALL(UF_UGMGR_initialize(argc, argv))) exit (1);  
        do_it();  
        UF_CALL(UF_terminate());  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发示例，主要实现了以下功能：
>
> 1. 定义了ECHO和report_error两个辅助函数，用于打印信息和错误处理。
> 2. 定义了list_in_folder函数，递归遍历指定文件夹下的所有子文件夹和零件，并打印出文件夹名称、零件编号和版本信息。
> 3. do_it函数获取根文件夹，然后调用list_in_folder函数打印出整个产品结构树。
> 4. ufusr和main函数是程序的入口点，分别用于NX内置调用和外部调用。它们都初始化NX系统，调用do_it函数，然后终止NX系统。
> 5. ufusr_ask_unload函数用于卸载NX内置调用程序。
> 6. 使用UF_CALL宏封装NX函数调用，并进行错误处理。
> 7. 使用递归遍历整个产品结构树，打印出文件夹和零件信息。
> 8. 通过区分文件夹和零件类型，分别进行递归遍历和版本信息打印。
> 9. 使用UF_UGMGR_list_folder_contents获取文件夹内容，支持递归遍历。
> 10. 通过错误处理确保程序稳定运行。
>
> 总体来说，这段代码实现了对NX产品结构树的递归遍历，并以树形结构打印出文件夹和零件信息，是一个典型的NX二次开发示例。
>
