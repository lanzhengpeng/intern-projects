### 【1205】export assembly to temp directory 将组件导出到临时目录

#### 代码

```cpp
    /*HEAD EXPORT_ASSEMBLY_TO_TEMP_DIRECTORY CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，可以总结如下：

1. UF_print_syslog 是 V18 版本新增的函数。

2. 该函数用于打印系统日志。

3. 使用该函数时，需要传入日志等级（如 LOG_INFO、LOG_WARNING 等）和日志内容作为参数。

4. 该函数会根据日志等级打印不同颜色的日志信息，如 INFO 级别为蓝色，WARNING 为黄色，ERROR 为红色等。

5. 日志内容将包括时间戳、日志等级和具体内容。

6. 示例代码展示了如何使用 UF_print_syslog 函数打印不同等级的日志信息。

7. 注意该函数是 V18 版本新增的，在之前的版本中不存在。 */  
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
            the_list,  /* The bottom up ordered list 里海译:The bottom up ordered list的翻译为“自底向上的列表”。 */  
            tmp_list,  /* Used to walk all the assembly parts 里海译:Used to walk all the assembly parts。 */  
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
    /*  Remove the duplicates from the ordered list maintaining order 里海译:从有序列表中移除重复项，保持顺序。 */  
        tmp_list = the_list;  
        UF_CALL(UF_MODL_create_list(&the_list));  
        for (tmp = tmp_list; tmp != NULL; tmp = tmp->next)  
            UF_CALL(UF_MODL_put_list_item(the_list, tmp->eid));  
        return make_an_array(&the_list, children);  
    }  
    static void build_similar_temp_filespec(char *fspec, int ftype, char *new_spec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            temp_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            *tmp_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4576(fspec, 0, dspec, temp_name));  
        if (strrchr(temp_name, '.')) strcpy(strrchr(temp_name, '.'), "");  
        UF_CALL(uc4575(tmp_dir, ftype, temp_name, new_spec));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_children,  
            orig_mwp;  
        tag_t  
            *children,  
            part = UF_PART_ask_display_part();  
        char  
            part_spec[UF_CFI_MAX_PATH_NAME_SIZE],  
            temp_spec[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_ASSEM_options_t  
            options;  
        UF_CALL(UF_ASSEM_ask_assem_options(&options));  
        orig_mwp = options.maintain_work_part;  
        if (orig_mwp == UF_ASSEM_do_maintain_work_part)  
        {  
            options.maintain_work_part = UF_ASSEM_dont_maintain_work_part;  
            UF_CALL(UF_ASSEM_set_assem_options(&options));  
        }  
        n_children = ask_assembly_parts_bottom_up(part, &children);  
        for (ii = 0; ii < n_children; ii++)  
        {  
            UF_CALL(UF_PART_set_display_part(children[ii]));  
            UF_PART_ask_part_name(children[ii], part_spec);  
            build_similar_temp_filespec(part_spec, 2, temp_spec);  
            WRITE_S(temp_spec);  
            UF_CALL(UF_PART_save_as(temp_spec));  
        }  
        if (orig_mwp == UF_ASSEM_do_maintain_work_part)  
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

> 根据代码内容，这段NX二次开发代码的功能如下：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 定义了一个内存分配函数allocate_memory，用于分配内存空间。
> 3. 定义了一个从列表中提取对象ID数组的函数make_an_array。
> 4. 定义了一个报告零件加载状态的函数report_load_status。
> 5. 定义了一个自底向上查询装配体零件的函数ask_assembly_parts_bottom_up。
> 6. 定义了一个生成相似临时文件名的函数build_similar_temp_filespec。
> 7. 定义了一个将字符串写入列表窗口的函数write_string_to_listing_window。
> 8. 定义了一个主函数do_it，用于查询当前显示零件的装配体结构，并按自底向上的顺序保存每个零件到临时文件。
> 9. 定义了UF函数ufusr，用于初始化和执行do_it函数。
> 10. 定义了主函数main，用于处理命令行传入的零件文件路径，打开零件，执行do_it函数，然后保存并关闭零件。
>
> 总的来说，这段代码实现了NX装配体自底向上遍历和零件保存的功能。
>
