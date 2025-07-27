### 【2560】save as entire assembly 保存整个装配为

#### 代码

```cpp
    /*HEAD SAVE_AS_ENTIRE_ASSEMBLY CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的函数。

UF_print_syslog在V18版本中是一个新增的函数。 */  
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
            the_list,  /* The bottom up ordered list 里海译:自底向上顺序列表 */  
            tmp_list,  /* Used to walk all the assembly parts 里海译:过去，所有的装配部件都是用手工搬运的。 */  
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
                /*  Put the children on the FRONT of the ordered list 里海译:把孩子们放在名单的前面。 */  
                    UF_CALL(allocate_memory(sizeof(uf_list_p_t), (void **)&front));  
                    front->eid = child;  
                    front->next = the_list;  
                    the_list = front;  
                }  
                UF_free(child_occs);  
            }  
        }  
        UF_CALL(UF_MODL_delete_list(&tmp_list));  
    /*  Remove the duplicates from the ordered list maintaining order 里海译:从有序列表中去除重复项，保持顺序 */  
        tmp_list = the_list;  
        UF_CALL(UF_MODL_create_list(&the_list));  
        for (tmp = tmp_list; tmp != NULL; tmp = tmp->next)  
            UF_CALL(UF_MODL_put_list_item(the_list, tmp->eid));  
        return make_an_array(&the_list, children);  
    }  
    static void build_new_partname(char *old_fspec, char *addon, char *copy_fspec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE];  
        UF_CALL(uc4576(old_fspec, 2, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        strcat(part_name, addon);  
        UF_CALL(uc4575(dspec, 2, part_name, copy_fspec));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_children,  
            old_option;  
        tag_t  
            *children,  
            part = UF_PART_ask_display_part();  
        char  
            new_spec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_spec[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_ASSEM_options_t  
            options;  
        UF_CALL(UF_ASSEM_ask_assem_options(&options));  
        old_option = options.maintain_work_part;  
        if (old_option == UF_ASSEM_do_maintain_work_part)  
        {  
            options.maintain_work_part = UF_ASSEM_dont_maintain_work_part;  
            UF_CALL(UF_ASSEM_set_assem_options(&options));  
        }  
        n_children = ask_assembly_parts_bottom_up(part, &children);  
        for (ii = 0; ii < n_children; ii++)  
        {  
            UF_PART_ask_part_name(children[ii], part_spec);  
            build_new_partname(part_spec, "_x", new_spec);  
            WRITE("Saving ");  
            WRITE(part_spec);  
            WRITE(" As ");  
            WRITE(new_spec);  
            WRITE("...\n");  
            UF_CALL(UF_PART_set_display_part(children[ii]));  
            UF_CALL(UF_PART_save_as(new_spec));  
        }  
        WRITE("\n");  
        if (old_option == UF_ASSEM_do_maintain_work_part)  
        {  
            options.maintain_work_part = UF_ASSEM_do_maintain_work_part;  
            UF_CALL(UF_ASSEM_set_assem_options(&options));  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在出错时输出错误信息。
> 2. 定义了一个向列表窗口写入字符串的函数write_string_to_listing_window。
> 3. 定义了一个分配内存的函数allocate_memory。
> 4. 定义了一个从列表中提取标签数组并删除列表的函数make_an_array。
> 5. 定义了一个报告加载状态的函数report_load_status。
> 6. 定义了一个自底向上询问装配体子部件的函数ask_assembly_parts_bottom_up。
> 7. 定义了一个构建新部件名称的函数build_new_partname。
> 8. 定义了一个主函数do_it，用于遍历装配体的子部件，并将每个部件另存为新文件。
> 9. 定义了ufusr函数，作为NX的二次开发接口函数，调用主函数do_it。
> 10. 定义了ufusr_ask_unload函数，用于卸载NX时调用。
> 11. 定义了主函数main，用于处理命令行传入的部件文件路径，打开部件并调用主函数do_it。
>
> 总的来说，这段代码实现了自底向上遍历装配体的子部件，并对每个子部件进行另存的操作，具有一定的参考价值。
>
