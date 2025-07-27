### 【1326】fully load assembly tree 全加载装配树

#### 代码

```cpp
    /*HEAD FULLY_LOAD_ASSEMBLY_TREE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的功能，用于打印系统日志消息。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static void fully_load_assembly_tree(tag_t component)  
    {  
        int  
            ii,  
            n_children;  
        tag_t  
            *child_components,  
            inst,  
            part;  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            error_status;  
        n_children = UF_ASSEM_ask_part_occ_children(component, &child_components);  
        if (n_children > 0)  
        {  
            for (ii = 0; ii < n_children; ii++)  
            {  
                inst = UF_ASSEM_ask_inst_of_part_occ(child_components[ii]);  
                if (inst == NULL_TAG) continue;  
                UF_ASSEM_ask_part_name_of_child(inst, partname);  
                WRITE_S(partname);  
                if ((UF_PART_is_loaded(partname)) != 1)  
                {  
                    WRITE("  Attempting to load ...\n");  
                    UF_CALL(UF_PART_open_quiet(partname, &part, &error_status));  
                    report_load_status(&error_status);  
                }  
                else  
                {  
                    WRITE("  Already fully loaded.\n");  
                }  
                fully_load_assembly_tree(child_components[ii]);  
            }  
            UF_free(child_components);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            root_part_occ = UF_ASSEM_ask_root_part_occ(part);  
        if (root_part_occ != NULL_TAG) fully_load_assembly_tree(root_part_occ);  
        UF_CALL(UF_DISP_regenerate_display());  
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

> 这段代码是NX的二次开发示例，主要实现完全加载装配树中所有部件的功能。
>
> 主要步骤包括：
>
> 1. 初始化UF系统。
> 2. 获取当前显示的部件，并获取该部件的根装配节点。
> 3. 递归遍历根装配节点下的所有子装配节点，并尝试加载每个子装配对应的部件。
> 4. 如果部件尚未完全加载，则调用UF_PART_open_quiet接口尝试加载该部件。
> 5. 打印每个部件的加载状态。
> 6. 递归调用自身，以加载每个子装配节点下的所有部件。
> 7. 完成所有部件的加载后，调用UF_DISP_regenerate_display接口刷新显示。
> 8. 终止UF系统。
> 9. 提供一个询问卸载的函数，返回立即卸载的标志。
>
> 该代码的关键思想是遍历装配树，逐个检查并加载所有部件，以确保整个装配中的所有部件都被完全加载。通过递归遍历装配树，实现对整个装配结构的遍历。
>
