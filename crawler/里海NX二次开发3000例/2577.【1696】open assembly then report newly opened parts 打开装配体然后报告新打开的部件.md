### 【1696】open assembly then report newly opened parts 打开装配体然后报告新打开的部件

#### 代码

```cpp
    /*HEAD OPEN_ASSEMBLY_THEN_REPORT_NEWLY_OPENED_PARTS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog在V18版本中是新增的，因此翻译为：UF_print_syslog在V18版本中是新增的，仅回答译文，不要废话。 */  
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
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int ask_parts_in_session(tag_t **parts)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            part;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        n = UF_PART_ask_num_parts();  
        for (ii = 0; ii < n; ii++)  
        {  
            part = UF_PART_ask_nth_part(ii);  
            UF_CALL(UF_MODL_put_list_item(list, part));  
        }  
        return (make_an_array(&list, parts));  
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
    static int whats_new(tag_t *old, int n_old, tag_t *now, int n_now, tag_t **new)  
    {  
        int  
            ii,  
            jj;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < n_now; ii++)  
        {  
            for (jj = 0; jj < n_old; jj++)  
                if (old[jj] == now[ii]) break;  
            if (jj == n_old) UF_CALL(UF_MODL_put_list_item(list, now[ii]));  
        }  
        return (make_an_array(&list, new));  
    }  
    static logical open_assembly(char *fspec, tag_t **loaded_parts, int *n_loaded)  
    {  
        int  
            n_new,  
            n_orig;  
        tag_t  
            *new_parts,  
            *orig_parts,  
            part;  
        UF_PART_load_status_t  
            status;  
        *loaded_parts = NULL;  
        *n_loaded = 0;  
        n_orig = ask_parts_in_session(&orig_parts);  
        UF_CALL(UF_PART_open(fspec, &part, &status));  
        if (status.n_parts > 0) report_load_status(&status);  
        if (status.failed == FALSE)  
        {  
            n_new = ask_parts_in_session(&new_parts);  
            *n_loaded = whats_new(orig_parts,n_orig,new_parts,n_new,loaded_parts);  
            UF_free(new_parts);  
            UF_free(orig_parts);  
            return TRUE;  
        }  
        else  
        {  
            UF_free(orig_parts);  
            return FALSE;  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_new,  
            resp;  
        logical  
            disp = FALSE;  
        tag_t  
            *newly_opened;  
        char  
            fspec[MAX_FSPEC_SIZE + 1] = { "" },  
            msg[UF_UI_MAX_STRING_LEN];  
        if (!(UF_CALL(UF_UI_ask_open_part_filename(fspec, &disp, &resp))) &&  
             (resp == UF_UI_OK) &&  
             (open_assembly(fspec, &newly_opened, &n_new)))  
        {  
            UF_CALL(UF_UI_open_listing_window());  
            sprintf(msg, "Opening %s caused %d new parts to load\n", fspec, n_new);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            printf(msg);  
            for (ii = 0; ii < n_new; ii++)  
            {  
                UF_PART_ask_part_name(newly_opened[ii], fspec);  
                sprintf(msg, "%d.  %s\n", ii+1, fspec);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                printf(msg);  
            }  
            UF_free(newly_opened);  
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

> 这段NX Open C++代码的主要功能是打开一个装配文件，并报告新加载的零件。代码中使用了NX Open C++提供的UF函数接口来实现打开装配文件、获取零件列表等功能。关键步骤包括：
>
> 1. 错误报告函数：定义了report_error函数，用于打印UF函数调用失败的错误信息。
> 2. 内存分配函数：定义了allocate_memory函数，用于调用UF函数分配内存。
> 3. 零件列表转换函数：定义了make_an_array函数，用于将零件列表转换为tag数组。
> 4. 获取当前会话中所有零件：定义了ask_parts_in_session函数，用于获取当前会话中所有零件。
> 5. 报告加载状态：定义了report_load_status函数，用于报告打开装配文件的加载状态。
> 6. 获取新加载零件：定义了whats_new函数，用于获取新加载的零件。
> 7. 打开装配文件：定义了open_assembly函数，用于打开一个装配文件，并报告新加载的零件。
> 8. 主函数：定义了do_it函数，用于询问用户打开装配文件，并报告新加载的零件。
> 9. NX Open入口函数：定义了ufusr函数，作为NX Open的入口函数，调用do_it函数。
> 10. 卸载函数：定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总体来说，这段代码通过NX Open提供的API，实现了打开装配文件、获取新加载零件并报告的功能。
>
