### 【1998】report assembly parts in bottom up order 自底向上报告装配零件

#### 代码

```cpp
    /*HEAD REPORT_ASSEMBLY_PARTS_IN_BOTTOM_UP_ORDER CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：UF_print_syslog是V18版本新增的功能。 */  
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
    static int ask_assembly_parts_bottom_up(tag_t top, tag_t **children)  
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
            front,  
            the_list,  /* The bottom up ordered list 里海译:自底向上的有序列表 */  
            tmp_list,  /* Used to walk all the assembly parts 里海译:过去常常需要走遍所有的装配零件。 */  
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
            if ((n = UF_ASSEM_ask_part_occ_children(root, &child_occs)) > 0)  
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
                /*  Put the children on the FRONT of the ordered list 里海译:将孩子们放在有序列表的前面。 */  
                    UF_CALL(allocate_memory(sizeof(uf_list_p_t), (void **)&front));  
                    front->eid = child;  
                    front->next = the_list;  
                    the_list = front;  
                }  
                UF_free(child_occs);  
            }  
        }  
        UF_CALL(UF_MODL_delete_list(&tmp_list));  
    /*  Remove the duplicates from the ordered list maintaining order 里海译:去除有序列表中的重复项，保持顺序 */  
        tmp_list = the_list;  
        UF_CALL(UF_MODL_create_list(&the_list));  
        for (tmp = tmp_list; tmp != NULL; tmp = tmp->next)  
            UF_CALL(UF_MODL_put_list_item(the_list, tmp->eid));  
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
            n_children = ask_assembly_parts_bottom_up(top_parts[ii], &children);  
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

> 这段代码是用于NX二次开发的C++代码，主要功能包括：
>
> 1. 定义了一个报告错误函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了allocate_memory函数，用于分配内存。
> 3. make_an_array函数，用于从列表中提取元素，存入数组中，并删除列表。
> 4. ask_top_parents函数，用于递归查询一个零件的所有顶层父件，并返回顶层父件数组。
> 5. report_load_status函数，用于报告加载状态。
> 6. ask_assembly_parts_bottom_up函数，用于递归查询一个装配的零件，并以自底向上的顺序返回零件数组。
> 7. do_it函数，用于打开当前显示的零件，查询其所有顶层父件，然后以自底向上的顺序查询每个父件的子件，并输出。
> 8. ufusr函数，用于初始化NX，调用do_it，然后终止NX。
> 9. ufusr_ask_unload函数，用于设置卸载模式。
> 10. main函数，用于处理命令行参数，打开零件，调用do_it，然后关闭零件。
>
> 总的来说，这段代码实现了从NX装配中查询零件，并以自底向上的顺序输出零件的功能。
>
