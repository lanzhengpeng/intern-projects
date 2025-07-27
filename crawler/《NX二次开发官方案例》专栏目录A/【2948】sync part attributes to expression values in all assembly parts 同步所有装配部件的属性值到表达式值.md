### 【2948】sync part attributes to expression values in all assembly parts 同步所有装配部件的属性值到表达式值

#### 代码

```cpp
    /*HEAD SYNC_PART_ATTRIBUTES_TO_EXPRESSION_VALUES_IN_ALL_ASSEMBLY_PARTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_attr.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 中的新功能。 */  
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
            the_list,  /* The top down up ordered list 里海译:从上到下的有序列表 */  
            tmp_list,  /* Used to walk all the assembly parts 里海译:曾经需要步行运送所有零部件。 */  
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
                /*  Move the children to the end of the ordered list 里海译:将孩子们移动到有序列表的末尾。 */  
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
    static void strip_trailing_zeros(char *s)  
    {  
        int ii;  
        if (strchr(s, '.'))  
        {  
            for (ii = strlen(s) - 1; s[ii] == '0'; ii--) s[ii] = '\0';  
            if (s[ii] == '.') s[ii] = '\0';  
        }  
    }  
    #define N_EXP 3  
    void sync_attributes_to_expressions(tag_t part)  
    {  
        int  
            ii;  
        tag_t  
            old_work,  
            part_attr;  
        double  
            size;  
        char  
            msg[133],  
            string[UF_ATTR_MAX_STRING_LEN+1],  
            *titles[N_EXP] = { "length", "height", "width" };  
        UF_ATTR_value_t  
            value;  
        UF_CALL(UF_ASSEM_set_work_part_quietly(part, &old_work));  
        UF_CALL(UF_ATTR_ask_part_attribute(&part_attr));  
        value.type = UF_ATTR_string;  
        value.value.string = string;  
        for (ii = 0; ii < N_EXP; ii++)  
        {  
            if (UF_MODL_eval_exp(titles[ii], &size)) continue;  
            sprintf(msg, "  %s = %f", titles[ii], size);  
            WRITE(msg);  
            WRITE("\n");  
            sprintf(string, "%f", size);  
            strip_trailing_zeros(string);  
            UF_CALL(UF_ATTR_assign(part_attr, titles[ii], value));  
        }  
        UF_CALL(UF_ASSEM_set_work_part_quietly(old_work, &part));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_children;  
        tag_t  
            *children,  
            part = UF_PART_ask_display_part();  
        char  
            part_spec[UF_CFI_MAX_PATH_NAME_SIZE];  
        n_children = ask_assembly_parts_top_down(part, &children);  
        for (ii = 0; ii < n_children; ii++)  
        {  
            UF_PART_ask_part_name(children[ii], part_spec);  
            WRITE(part_spec);  
            WRITE(":\n");  
            sync_attributes_to_expressions(children[ii]);  
        }  
        if (n_children > 0) UF_free(children);  
        WRITE("\n");  
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

> 根据代码，这段NX二次开发代码的主要功能是遍历装配体中的所有零件，并将零件的长度、高度、宽度三个尺寸属性的值同步到对应的表达式。具体步骤如下：
>
> 1. 定义了错误报告函数report_error，用于输出错误信息。
> 2. 定义了写入列表窗口的宏WRITE和WRITE_S。
> 3. 定义了分配内存的函数allocate_memory。
> 4. 定义了将列表转换为数组的函数make_an_array。
> 5. 定义了报告装配体加载状态的函数report_load_status。
> 6. 定义了检查列表中是否包含指定元素的函数is_item_on_list。
> 7. 定义了递归获取装配体所有零件的函数ask_assembly_parts_top_down。
> 8. 定义了去除字符串尾随零的函数strip_trailing_zeros。
> 9. 定义了同步零件属性到表达式的函数sync_attributes_to_expressions。
> 10. 定义了主函数do_it，用于获取当前显示的零件，递归获取所有子零件，并同步它们的尺寸属性到表达式。
> 11. 定义了UF函数ufusr，作为程序的入口，初始化后调用do_it，然后终止。
> 12. 定义了卸载函数ufusr_ask_unload。
>
> 总体来说，这段代码实现了NX装配体中所有零件的尺寸属性与表达式的同步功能。
>
