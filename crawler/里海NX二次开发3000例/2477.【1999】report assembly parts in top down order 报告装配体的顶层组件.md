### 【1999】report assembly parts in top down order 报告装配体的顶层组件

#### 代码

```cpp
    /*HEAD REPORT_ASSEMBLY_PARTS_IN_TOP_DOWN_ORDER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，我理解的是：

在V18版本中，新增了UF_print_syslog函数。

仅回答翻译，不添加任何评论。 */  
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
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static int ask_top_parents(tag_t child, tag_t **top_parents)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *parents;  
        uf_list_p_t  
            rel,  
            rel_list,  
            top_list;  
        UF_CALL(UF_MODL_create_list(&rel_list));  
        UF_CALL(UF_MODL_create_list(&top_list));  
        UF_CALL(UF_MODL_put_list_item(rel_list, child));  
        for (rel = rel_list; rel != NULL; rel = rel->next)  
        {  
            n = UF_ASSEM_where_is_part_used(rel->eid, &parents);  
            if (n == 0)  
                UF_CALL(UF_MODL_put_list_item(top_list, rel->eid));  
            else  
            {  
                for (ii = 0; ii < n; ii++)  
                    UF_CALL(UF_MODL_put_list_item(rel_list, parents[ii]));  
                UF_free(parents);  
            }  
        }  
        UF_CALL(UF_MODL_delete_list(&rel_list));  
        return make_an_array(&top_list, top_parents);  
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
    static int ask_assembly_parts_top_down(tag_t top, tag_t **children)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            child,  
            *child_occs,  
            inst,  
            root;  
        uf_list_p_t  
            the_list,  /* The top down up ordered list 里海译:根据上下文，"The top down up ordered list"可以翻译为“从上到下的顺序列表”。 */  
            tmp_list,  /* Used to walk all the assembly parts 里海译:过去常常步行运送所有零部件。 */  
            tmp;  
        UF_PART_load_status_t  
            load_status;  
        UF_CALL(UF_MODL_create_list(&the_list));  
        UF_CALL(UF_MODL_put_list_item(the_list, top));  
        UF_CALL(UF_MODL_create_list(&tmp_list));  
        UF_CALL(UF_MODL_put_list_item(tmp_list, top));  
        for (tmp = tmp_list; tmp != NULL; tmp = tmp->next)  
        {  
            root = UF_ASSEM_ask_root_part_occ(tmp->eid);  
            if (root == NULL_TAG) continue;  
            if ((n = UF_ASSEM_ask_part_occ_children( root, &child_occs)) > 0)  
            {  
                for (ii = 0; ii < n; ii++)  
                {  
                    inst = UF_ASSEM_ask_inst_of_part_occ(child_occs[ii]);  
                    if (inst == NULL_TAG) continue;  
                    UF_CALL(UF_ASSEM_ensure_child_loaded(inst, &load_status));  
                    report_load_status(&load_status);  
                    child = UF_ASSEM_ask_prototype_of_occ(child_occs[ii]);  
                    if (child == NULL_TAG) continue;  
                    UF_CALL(UF_MODL_put_list_item(tmp_list, child));  
                /*  Move the children to the end of the ordered list 里海译:将孩子们移至有序列表的末尾 */  
                    if (is_item_on_list(the_list, child))  
                        UF_CALL(UF_MODL_delete_list_item(&the_list, child));  
                    UF_CALL(UF_MODL_put_list_item(the_list, child));  
                }  
                UF_free(child_occs);  
            }  
        }  
        UF_CALL(UF_MODL_delete_list(&tmp_list));  
        return make_an_array(&the_list, children);  
    }  
    static void do_it(void)  
    {  
        int  
            ii, jj,  
            n_children,  
            n_top;  
        tag_t  
            *children,  
            part = UF_PART_ask_display_part(),  
            *top_parts;  
        char  
            part_spec[UF_CFI_MAX_PATH_NAME_SIZE];  
        n_top = ask_top_parents(part, &top_parts);  
        for (ii = 0; ii < n_top; ii++)  
        {  
            n_children = ask_assembly_parts_top_down(top_parts[ii], &children);  
            for (jj = 0; jj < n_children; jj++)  
            {  
                UF_PART_ask_part_name(children[jj], part_spec);  
                WRITE_S(part_spec);  
            }  
            if (n_children > 0) UF_free(children);  
            WRITE("\n");  
        }  
        if (n_top > 0) UF_free(top_parts);  
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
    /*  To run in external native mode UG:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external UG/Manager or IMAN mode:  
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

> 根据代码内容，这是一段用于NX的二次开发代码，主要实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于输出函数调用错误。
> 2. 定义了WRITE和WRITE_S宏，用于向列表窗口输出信息。
> 3. 定义了allocate_memory函数，用于申请内存。
> 4. 定义了make_an_array函数，用于将列表转换为数组。
> 5. 定义了ask_top_parents函数，用于查询顶级父项。
> 6. 定义了report_load_status函数，用于报告加载状态。
> 7. 定义了is_item_on_list函数，用于判断项目是否在列表中。
> 8. 定义了ask_assembly_parts_top_down函数，用于按自顶向下顺序查询装配部件。
> 9. 定义了do_it函数，用于执行查询并输出结果。
> 10. 定义了ufusr函数，用于NX环境下的入口函数。
> 11. 定义了main函数，用于外部调用入口，可以接受零件路径参数或数据库路径参数。
>
> 总的来说，这段代码实现了按自顶向下顺序查询装配部件，并输出每个部件的名称。它可以通过NX环境或外部程序调用执行。
>
