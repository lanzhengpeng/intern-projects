### 【1552】load all children 加载所有子组件

#### 代码

```cpp
    /*HEAD LOAD_ALL_CHILDREN CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog 是 V18 中的新功能，请只提供翻译，不要添加其他内容。 */  
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
    static tag_t ask_next_component(tag_t part, tag_t comp)  
    {  
        int  
            subtype,  
            type;  
        while  (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_component_type, &comp))  
            && (comp != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(comp, &type, &subtype));  
            if (subtype == UF_part_occurrence_subtype) return comp;  
        }  
        return comp;  
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
    static void ensure_children_loaded(tag_t part)  
    {  
        tag_t  
            child_part,  
            comp = NULL_TAG,  
            inst;  
        UF_PART_load_status_t  
            status;  
        while ((comp = ask_next_component(part, comp)) != NULL_TAG)  
        {  
            inst = UF_ASSEM_ask_inst_of_part_occ(comp);  
            UF_ASSEM_ensure_child_loaded(inst, &status);  
            report_load_status(&status);  
            child_part = UF_ASSEM_ask_prototype_of_occ(comp);  
            if (child_part != NULL_TAG) ensure_children_loaded(child_part);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        ensure_children_loaded(part);  
    }  
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
    void main( int argc, char *argv[] )  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            if (argc > 1)  
            {  
                int  
                    cnt = 0,  
                    ii;  
                logical  
                    new_flag  = FALSE,  
                    save_flag = FALSE;  
                tag_t  
                    part;  
                UF_PART_load_status_t  
                    status;  
                for (ii=1; ii<argc; ii++)  
                {  
                    if (!strcmp(argv[ii], "-save")) save_flag = TRUE;  
                    else if (!strcmp(argv[ii], "-new")) new_flag = TRUE;  
                    else  
                    {  
                        printf("%d.  %s\n", ++cnt, argv[ii]);  
                        if (new_flag)  
                        {  
                            if (!UF_CALL(UF_PART_new(argv[ii], UF_PART_ENGLISH,  
                                &part)))  
                            {  
                                do_it();  
                                if (save_flag)  UF_CALL(UF_PART_save());  
                                UF_PART_close_all();  
                            }  
                        }  
                        else  
                        {  
                            if (UF_CALL(UF_PART_open(argv[ii], &part, &status))  
                                || status.n_parts > 0)  
                                report_load_status(&status);  
                            else  
                            {  
                                do_it();  
                                if (save_flag)  UF_CALL(UF_PART_save());  
                                UF_PART_close_all();  
                            }  
                        }  
                    }  
                }  
                printf("\nProcessed %d parts.\n", cnt);  
            }  
            else do_it();  
            UF_CALL(UF_terminate());  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的二次开发代码，其主要功能是确保NX装配体中的所有组件都被加载。
>
> 代码的主要特点包括：
>
> 1. 使用UF_OBJ_cycle_objs_in_part函数遍历装配体中的所有组件，并使用UF_ASSEM_ensure_child_loaded函数确保组件被加载。
> 2. 使用自定义的report_error函数来报告函数调用错误，并通过UF_print_syslog输出错误信息。
> 3. 主函数中，通过命令行参数指定要处理的NX部件文件。若指定了"-new"参数，会调用UF_PART_new函数创建新的部件文件；若指定了"-save"参数，会调用UF_PART_save函数保存当前部件文件。
> 4. ufusr函数是NX二次开发程序的入口函数，其中调用UF_initialize初始化API，并调用主函数执行代码逻辑，最后调用UF_terminate终止API。
> 5. ufusr_ask_unload函数用于指定在NX退出时是否卸载二次开发程序，这里返回了UF_UNLOAD_IMMEDIATELY，表示立即卸载。
> 6. 使用了一些自定义函数，如ask_next_component获取装配体中的下一个组件，report_load_status报告组件加载状态，ensure_children_loaded递归加载组件等。
>
> 总体来说，这段代码实现了遍历NX装配体，确保所有组件都被加载的功能，同时提供了命令行参数处理、错误报告等辅助功能。
>
