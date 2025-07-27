### 【2000】report assembly parts list callouts 列出装配体的部件清单和标注

#### 代码

```cpp
    /*HEAD REPORT_ASSEMBLY_PARTS_LIST_CALLOUTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_attr.h>  
    #include <uf_cfi.h>  
    #include <uf_assem.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18中的新增功能，用于打印系统日志。 */  
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
    static int ask_all_of_type(tag_t part, int type, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object)) &&  
                (object != NULL_TAG))  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
        return (make_an_array(&object_list, objects));  
    }  
    static void ask_part_shortname(tag_t part, char *shortname)  
    {  
        logical  
            nxman;  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
            UF_CALL(UF_PART_name_for_display(partname, shortname));  
        else  
            UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static logical is_item_on_list(uf_list_p_t object_list, tag_t item)  
    {  
        int  
            ii;  
        uf_list_p_t  
            temp;  
        for (ii = 0, temp = object_list; temp != NULL; temp = temp->next, ii++)  
            if (temp->eid == item) return TRUE;  
        return FALSE;  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_comps;  
        tag_t  
            *comps,  
            part = UF_PART_ask_display_part(),  
            proto;  
        char  
            leafname[MAX_FSPEC_SIZE+1];  
        uf_list_p_t  
            done;  
        UF_ATTR_value_t  
            att;  
        if ((n_comps = ask_all_of_type(part, UF_component_type, &comps)) > 0)  
        {  
            UF_CALL(UF_MODL_create_list(&done));  
            for (ii = 0; ii < n_comps; ii++)  
            {  
                proto = UF_ASSEM_ask_prototype_of_occ(comps[ii]);  
                if (!is_item_on_list(done, proto))  
                {  
                    UF_CALL(UF_MODL_put_list_item(done, proto));  
                    ask_part_shortname(proto, leafname);  
                    WRITE(leafname);  
                    UF_CALL(UF_ATTR_read_value(comps[ii], "CALLOUT",  
                        UF_ATTR_any, &att));  
                    WRITE(" CALLOUT = ");  
                    WRITE(att.value.string);  
                    UF_free(att.value.string);  
                    WRITE("\n");  
                }  
            }  
            UF_free(comps);  
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
    static void check_load_status(UF_PART_load_status_p_t status)  
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
        UF_free(status->statuses);  
        UF_free_string_array(status->n_parts, status->file_names);  
    }  
    /*  To run in external native mode NX:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external NX Manager mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                printf("%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) check_load_status(&status);  
                if (!status.failed)  
                {  
                    do_it();  
                    UF_CALL(UF_PART_close_all());  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是NX Open C++的一个示例程序，用于列出指定组件的名称和标注信息。以下是主要功能的介绍：
>
> 1. 初始化和错误报告：使用了UF_initialize进行初始化，并在UF_CALL宏中处理错误。
> 2. 内存分配：定义了allocate_memory函数用于分配内存。
> 3. 创建组件列表：make_an_array函数用于创建组件的数组。
> 4. 获取组件类型：ask_all_of_type函数用于获取指定类型的组件。
> 5. 获取组件简称：ask_part_shortname函数用于获取组件的简称。
> 6. 检查组件是否已处理：is_item_on_list函数用于检查组件是否已处理。
> 7. 执行主体功能：do_it函数用于获取当前显示组件，获取其所有组件，并打印组件简称和标注信息。
> 8. 外部程序入口点：ufusr函数是外部程序入口点。
> 9. 卸载处理：ufusr_ask_unload用于处理卸载。
> 10. 检查加载状态：check_load_status用于检查组件的加载状态。
> 11. 命令行参数处理：在main函数中处理命令行参数，打开指定组件，并调用do_it函数。
>
> 总体来说，这段代码实现了NX组件的查询和处理功能，并提供了命令行和NX内部两种调用方式。代码结构清晰，功能明确，是一个很好的NX Open C++二次开发示例。
>
