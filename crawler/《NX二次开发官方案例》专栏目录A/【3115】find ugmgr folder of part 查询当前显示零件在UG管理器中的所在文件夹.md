### 【3115】find ugmgr folder of part 查询当前显示零件在UG管理器中的所在文件夹

#### 代码

```cpp
    /*HEAD FIND_UGMGR_FOLDER_OF_PART CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_ugmgr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据您提供的说明，UF_print_syslog是V18版本新增的功能。 */  
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
    static logical find_in_folder(UF_UGMGR_tag_t folder, char *find_part_number,  
        UF_UGMGR_tag_t *found_in)  
    {  
        int  
            ii,  
            count;  
        UF_UGMGR_tag_t  
            *folder_contents;  
        UF_UGMGR_object_type_t  
            obj_type;  
        char  
            part_number[UF_UGMGR_NAME_SIZE + 1];  
        if (folder == UF_UGMGR_null_tag) return FALSE;  
        UF_CALL(UF_UGMGR_list_folder_contents(folder, &count, &folder_contents));  
        for (ii = 0; ii < count; ii++)  
        {  
            UF_CALL(UF_UGMGR_ask_object_type(folder_contents[ii], &obj_type));  
            switch(obj_type)  
            {  
                case UF_UGMGR_type_folder :  
                    if (find_in_folder(folder_contents[ii], find_part_number,  
                        found_in))  
                    {  
                        UF_free(folder_contents);  
                        return TRUE;  
                    }  
                    break;  
                case UF_UGMGR_type_part :  
                    UF_CALL(UF_UGMGR_ask_part_number(folder_contents[ii],  
                        part_number));  
                    if (!strcmp(part_number, find_part_number))  
                    {  
                        *found_in = folder;  
                        UF_free(folder_contents);  
                        return TRUE;  
                    }  
                    break;  
                default :  
                    break;  
            }  
        }  
        UF_free(folder_contents);  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        logical  
            active;  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_UGMGR_tag_t  
            folder,  
            root;  
        char  
            encoded_name[MAX_FSPEC_SIZE+1],  
            folder_name[UF_UGMGR_NAME_SIZE + 1],  
            msg[133],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1];  
        UF_CALL(UF_is_ugmanager_active(&active));  
        if (!active)  
        {  
            uc1601("this is a not a UGMGR session", TRUE);  
            return;  
        }  
        UF_PART_ask_part_name(part, encoded_name);  
        UF_CALL(UF_UGMGR_decode_part_filename(encoded_name, part_number,  
            part_revision, part_file_type, part_file_name));  
        if ((!UF_CALL(UF_UGMGR_ask_root_folder(&root))) &&  
            find_in_folder(root, part_number, &folder))  
        {  
            UF_CALL(UF_UGMGR_ask_folder_name(folder, folder_name));  
            sprintf(msg, "%s found in %s", part_number, folder_name);  
            uc1601(msg, TRUE);  
        }  
        else  
        {  
            sprintf(msg, "%s NOT found", part_number);  
            uc1601(msg, TRUE);  
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

> 这段代码是用于NX Open的二次开发代码，主要实现了以下功能：
>
> 1. 定义了UF_CALL宏，用于封装UF函数调用，并在出错时报告错误信息。
> 2. 实现了report_error函数，用于在UF函数调用出错时，获取错误信息并打印到系统日志和提示窗口中。
> 3. 实现了递归函数find_in_folder，用于在UGManager的文件夹中查找指定零件号对应的零件。
> 4. 实现了do_it函数，用于获取当前显示的零件，解码出零件号，然后在UGManager的根目录下递归查找该零件号对应的零件，如果找到，则打印出零件号和所在文件夹名称；如果没有找到，则打印提示信息。
> 5. 实现了ufusr函数，作为程序的入口函数，用于初始化NX环境，调用do_it函数执行查找操作，然后终止NX环境。
> 6. 实现了ufusr_ask_unload函数，用于在卸载UF时立即卸载，而不是等待垃圾回收。
>
> 总的来说，这段代码的主要目的是为了在UGManager的文件夹结构中查找指定零件号对应的零件，并打印出零件号和所在文件夹名称。
>
