### 【1708】open renamed components 打开重命名的装配合

#### 代码

```cpp
    /*HEAD OPEN_RENAMED_COMPONENTS CCC UFUN */  
    /*  
        This example program assumes that you have already set your load  
        options to load "No components", and have loaded the assembly whose  
        components cannot be found because they have already be renamed on  
        the disk.  The new parts must be in the same directory and named the  
        same thing as the old parts with an _new added at the end.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中的新功能，用于打印系统日志。 */  
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
    static void report_load_status(UF_PART_load_status_t *status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        if (status->n_parts > 0)  
        {  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void rename_children(tag_t component)  
    {  
        int  
            i,  
            n_children;  
        tag_t  
            *children,  
            instance = UF_ASSEM_ask_inst_of_part_occ(component);  
        char  
            new_name[MAX_FSPEC_SIZE+1],  
            old_name[MAX_FSPEC_SIZE+1],  
            *p;  
        UF_PART_load_status_t  
            error_status;  
        tag_t  
            part;  
        if (instance != NULL_TAG)  
        {  
            UF_ASSEM_ask_part_name_of_child(instance, old_name);  
            if (UF_PART_is_loaded(old_name) == 0)  
            {  
                printf("    mapping %s to ", old_name);  
                if ((p = strstr(old_name, ".prt")) != 0 ||  
                    (p = strstr(old_name, ".PRT")) != 0)  
                {  
                    strncpy(new_name, old_name, p - old_name);  
                    new_name[p - old_name] = '\0';  
                    strcat(new_name, "_new.prt");  
                    printf("%s\n", new_name);  
                    if (UF_CALL(UF_PART_open_component_as(component, old_name,  
                        new_name, &part, &error_status))  
                            || error_status.n_parts > 0)  
                        report_load_status(&error_status);  
                }  
                else printf("*** UNKNOWN ***\n");  
            }  
        }  
        n_children = UF_ASSEM_ask_part_occ_children(component, &children);  
        for (i=0; i<n_children; i++) rename_children(children[i]);  
        if (n_children > 0) UF_free(children);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            root = UF_ASSEM_ask_root_part_occ(UF_PART_ask_display_part());  
        if (root == NULL_TAG)  
            uc1601("Displayed part is not an assembly", TRUE);  
        else  
        {  
            rename_children(root);  
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

> 这是一段用于处理NX装配体组件重命名问题的二次开发代码。
>
> 主要功能包括：
>
> 1. 错误报告函数：用于记录函数调用失败时的错误代码和错误信息。
> 2. 加载状态报告函数：用于报告组件加载过程中的错误状态。
> 3. 重命名子组件函数：递归遍历装配体的所有子组件，并尝试将已重命名的组件映射到新的prt文件。
> 4. 主函数：获取当前显示的装配体的根组件，并调用重命名子组件函数。
> 5. ufusr函数：NX二次开发标准入口函数，用于初始化和调用主函数。
> 6. 卸载函数：返回立即卸载标志，用于在二次开发模块被卸载时执行。
>
> 代码的关键逻辑是递归遍历装配体，将每个子组件的旧prt文件名映射到新的prt文件名，以解决组件重命名后NX找不到组件的问题。
>
