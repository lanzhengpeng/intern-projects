### 【1327】fully load assembly tree 完全加载装配树

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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据原文，我理解的是：

UF_print_syslog 是一个在 V18 版本中新增的功能，用于打印系统日志。这个函数在 V18 版本中首次出现，因此在使用这个函数时需要注意它仅适用于 V18 及以后的版本。 */  
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

> 这段代码是一个NX Open C++程序，用于实现自动加载装配树中的所有零件的功能。以下是代码的主要内容和功能：
>
> 1. 包含必要的头文件，如uf.h、uf_ui.h、uf_assem.h等，以调用NX Open API。
> 2. 定义了UF_CALL宏，用于调用NX Open API函数，并在失败时打印错误信息。
> 3. report_error函数用于在API调用失败时打印错误信息。
> 4. WRITE和WRITE_S宏用于向列表窗口写入信息，方便调试。
> 5. write_string_to_listing_window函数用于向列表窗口写入标题和字符串。
> 6. report_load_status函数用于报告零件加载状态。
> 7. fully_load_assembly_tree函数递归地遍历装配树，检查并加载未加载的零件。
> 8. do_it函数获取当前显示的零件，获取其根装配节点，并调用fully_load_assembly_tree函数。
> 9. ufusr函数是程序的入口点，用于初始化NX Open、调用do_it函数，然后终止NX Open。
> 10. ufusr_ask_unload函数用于卸载二次开发程序。
>
> 总体来说，这段代码实现了自动加载装配树中的所有零件的功能，以便进行后续的装配、分析和加工等操作。
>
