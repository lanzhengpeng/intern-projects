### 【1521】list dates of all component parts in assembly 列出装配中所有组件的日期列表

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的函数。 */  
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
    static logical add_proto_to_list(tag_t comp, void *user_data)  
    {  
        tag_t  
            proto = UF_ASSEM_ask_prototype_of_occ(comp);  
        uf_list_p_t  
            *list = (uf_list_p_t *)user_data;  
        UF_CALL(UF_MODL_put_list_item(*list, proto));  
        return TRUE;  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static int ask_all_assembly_parts(tag_t part, tag_t **parts)  
    {  
        tag_t  
            cset;  
        uf_list_p_t  
            part_list;  
        UF_PART_load_status_t  
            status;  
        UF_CALL(UF_MODL_create_list(&part_list));  
        UF_CALL(UF_MODL_put_list_item(part_list, part));  
        UF_CALL(UF_ASSEM_ask_all_comp_cset(part, &cset));  
        if (cset != NULL_TAG)  
        {  
            UF_CALL(UF_PART_open_cset(cset, &status));  
            report_load_status(&status);  
            UF_CALL(UF_ASSEM_apply_to_cset(cset, add_proto_to_list, &part_list));  
        }  
        return make_an_array(&part_list, parts);  
    }  
    static void do_it(void)  
    {  
        int  
            n_parts,  
            ii,  
            cdate[2],  
            ldate[2],  
            mdate[2];  
        char  
            datestg[21],  
            part_fspec[MAX_FSPEC_SIZE+1],  
            msg[133],  
            timestg[21];  
        tag_t  
            part = UF_PART_ask_display_part(),  
            *parts;  
        if ((n_parts = ask_all_assembly_parts(part, &parts)) == 0) return;  
        UF_UI_open_listing_window();  
        for (ii = 0; ii < n_parts; ii++)  
        {  
            UF_CALL(UF_PART_ask_part_name(parts[ii], part_fspec));  
            UF_UI_write_listing_window(part_fspec);  
            UF_UI_write_listing_window(":\n");  
            if (UF_CALL(uc4564(part_fspec, 2, (1<<4)))) continue;  
            UF_CALL(uc4606(cdate, mdate, ldate));  
            UF_CALL(uc4582(cdate, 3, datestg, timestg));  
            sprintf(msg, "\tCreation date & time     - %s %s\n",datestg,timestg);  
            UF_UI_write_listing_window(msg);  
            UF_CALL(uc4582(mdate, 3, datestg, timestg));  
            sprintf(msg, "\tModification date & time - %s %s\n",datestg,timestg);  
            UF_UI_write_listing_window(msg);  
            UF_CALL(uc4582(ldate, 3, datestg, timestg));  
            sprintf(msg, "\tLast Access date & time  - %s %s\n",datestg,timestg);  
            UF_UI_write_listing_window(msg);  
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

> 这段代码是一个NX Open C++ API的二次开发示例代码，主要功能是获取当前显示组件的所有装配组件，并输出每个组件的名称、创建时间、修改时间以及最后访问时间。以下是代码的主要功能和实现：
>
> 1. 定义了一个错误报告函数report_error，用于在API调用出错时输出错误信息。
> 2. 定义了一个内存分配函数allocate_memory，用于调用UF_allocate_memory分配内存。
> 3. 定义了一个函数make_an_array，用于从链表中提取组件Tag并转换为数组。
> 4. 定义了一个回调函数add_proto_to_list，用于获取组件的原始组件并添加到链表中。
> 5. 定义了一个函数report_load_status，用于输出组件集的加载状态信息。
> 6. 定义了一个函数ask_all_assembly_parts，用于获取组件的所有装配组件，并返回组件数组。
> 7. 定义了一个函数do_it，用于遍历组件数组，输出每个组件的名称和三个时间属性。
> 8. 定义了主函数ufusr，用于初始化和调用do_it函数，然后终止API。
> 9. 定义了一个询问卸载函数ufusr_ask_unload，返回立即卸载标志。
>
> 总的来说，这段代码通过NX Open C++ API获取组件信息，并以列表形式输出，展示了NX二次开发的基本流程和常用技巧。
>
