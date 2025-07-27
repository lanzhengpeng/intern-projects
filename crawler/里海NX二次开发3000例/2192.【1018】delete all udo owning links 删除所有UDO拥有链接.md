### 【1018】delete all udo owning links 删除所有UDO拥有链接

#### 代码

```cpp
    /*HEAD DELETE_ALL_UDO_OWNING_LINKS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_udobj.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的功能。 */  
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
    static logical ask_yes_or_no(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        return (2 - resp);  
    }  
    static tag_t ask_next_udo(tag_t part, tag_t last)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_user_defined_object_type,  
            &last));  
        return (last);  
    }  
    static void report_udo_info(tag_t udo)  
    {  
        int  
            ii,  
            status,  
            subtype,  
            type;  
        UF_UDOBJ_all_data_t  
            data;  
        char  
            out[UF_UI_MAX_STRING_LEN];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(out,"\nUDO tag %d:\n", udo);  
        UF_CALL(UF_UI_write_listing_window(out));  
        UF_CALL(UF_UDOBJ_ask_udo_data(udo, &data));  
        sprintf(out, "\tUDO update status = %d\n", data.udo_status);  
        UF_CALL(UF_UI_write_listing_window(out));  
        sprintf(out, "\tUDO has %d integers\n", data.num_ints);  
        UF_CALL(UF_UI_write_listing_window(out));  
        for (ii = 0; ii < data.num_ints; ii++)  
        {  
            sprintf(out, "\t\t%d. %d\n", ii+1, data.ints[ii]);  
            UF_CALL(UF_UI_write_listing_window(out));  
        }  
        sprintf(out, "\tUDO has %d doubles\n", data.num_doubles);  
        UF_CALL(UF_UI_write_listing_window(out));  
        for (ii = 0; ii < data.num_doubles; ii++)  
        {  
            sprintf(out, "\t\t%d. %f\n", ii+1, data.doubles[ii]);  
            UF_CALL(UF_UI_write_listing_window(out));  
        }  
        sprintf(out, "\tUDO has %d strings\n", data.num_strings);  
        UF_CALL(UF_UI_write_listing_window(out));  
        for (ii = 0; ii < data.num_strings; ii++)  
        {  
            sprintf(out, "\t\t%d. %s\n", ii+1, data.strings[ii]);  
            UF_CALL(UF_UI_write_listing_window(out));  
        }  
        sprintf(out, "\tUDO has %d links\n", data.num_links);  
        UF_CALL(UF_UI_write_listing_window(out));  
        for (ii = 0; ii < data.num_links; ii++)  
        {  
            if (data.link_defs[ii].assoc_ug_tag != NULL_TAG)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(  
                    data.link_defs[ii].assoc_ug_tag, &type, &subtype));  
                status = UF_OBJ_ask_status(data.link_defs[ii].assoc_ug_tag);  
                sprintf(out, "\t\t%d. type %d to %d (%d/%d %d) status %d\n", ii+1,  
                    data.link_defs[ii].link_type, data.link_defs[ii].assoc_ug_tag,  
                    type, subtype, status, data.link_defs[ii].object_status);  
                UF_CALL(UF_UI_write_listing_window(out));  
            }  
            else  
            {  
                sprintf(out, "\t\t%d. type %d to NULL_TAG\n", ii+1,  
                    data.link_defs[ii].link_type);  
                UF_CALL(UF_UI_write_listing_window(out));  
            }  
        }  
        sprintf(out, "\tUDO has %d lengths\n", data.num_lengths);  
        UF_CALL(UF_UI_write_listing_window(out));  
        for (ii = 0; ii < data.num_lengths; ii++)  
        {  
            sprintf(out, "\t\t%d. %f\n", ii+1, data.lengths[ii]);  
            UF_CALL(UF_UI_write_listing_window(out));  
        }  
        sprintf(out, "\tUDO has %d areas\n", data.num_areas);  
        UF_CALL(UF_UI_write_listing_window(out));  
        for (ii = 0; ii < data.num_areas; ii++)  
        {  
            sprintf(out, "\t\t%d. %f\n", ii+1, data.areas[ii]);  
            UF_CALL(UF_UI_write_listing_window(out));  
        }  
        sprintf(out, "\tUDO has %d volumes\n", data.num_volumes);  
        UF_CALL(UF_UI_write_listing_window(out));  
        for (ii = 0; ii < data.num_volumes; ii++)  
        {  
            sprintf(out, "\t\t%d. %f\n", ii+1, data.volumes[ii]);  
            UF_CALL(UF_UI_write_listing_window(out));  
        }  
        UF_CALL(UF_UDOBJ_free_udo_data(&data));  
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
    static void do_it(void)  
    {  
        int  
            ii,  
            kk,  
            n_objs = 0,  
            n = 0;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            *objs = NULL_TAG,  
            *udos = NULL_TAG,  
            udo = NULL_TAG;  
        uf_list_p_t  
            udo_list;  
        UF_CALL(UF_MODL_create_list(&udo_list));  
        while ((udo = ask_next_udo(part, udo)) != NULL_TAG)  
        {  
            report_udo_info(udo);  
            UF_CALL(UF_MODL_put_list_item(udo_list, udo));  
        }  
        n = make_an_array(&udo_list, &udos);  
        if (n == 0)  
            uc1601("No UDOs Found", TRUE);  
        else  
        {  
            if (ask_yes_or_no("ARE YOU SURE?","DELETE All UDO Links?" ))  
            {  
                for( ii=0; ii < n; ii++)  
                {  
                    UF_CALL(UF_UDOBJ_ask_owned_objects(udos[ii], &n_objs, &objs));  
                    if (n_objs > 0 )  
                    {  
                        for( kk=0; kk < n_objs; kk++)  
                        {  
                            UF_CALL(UF_UDOBJ_delete_owning_link (udos[ii], objs[kk]));  
                        }  
                    }  
                }  
            }  
            UF_free(udos);  
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

> 这段代码是一个NX Open C++ UG/Open User Function，用于删除NX零件中的所有用户定义对象(UDO)拥有的链接。其主要功能包括：
>
> 1. 初始化和错误处理：代码首先初始化NX环境，并定义了一个错误处理函数report_error，用于输出错误信息。
> 2. 询问对话框：定义了一个函数ask_yes_or_no，用于创建一个询问对话框，让用户确认是否删除所有UDO链接。
> 3. 遍历零件中的UDO：定义了一个函数ask_next_udo，用于遍历零件中的所有UDO，并获取UDO的tag。
> 4. 输出UDO信息：定义了一个函数report_udo_info，用于输出UDO的各种属性信息。
> 5. 删除UDO拥有的链接：在主函数do_it中，遍历所有UDO，并使用UF_UDOBJ_delete_owning_link API删除UDO拥有的所有链接。在删除前会弹出一个确认对话框。
> 6. 内存分配和释放：使用了自定义的内存分配和释放函数allocate_memory和UF_free。
> 7. UF_UNLOAD_IMMEDIATELY：定义了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY，表示该UF在调用后立即卸载。
>
> 整体来说，这段代码实现了在NX零件中遍历所有UDO，并让用户确认后删除UDO拥有的链接的功能，代码结构清晰，注释详细，易于理解和使用。
>
