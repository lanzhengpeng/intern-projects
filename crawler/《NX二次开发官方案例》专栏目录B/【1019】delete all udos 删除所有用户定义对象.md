### 【1019】delete all udos 删除所有用户定义对象

#### 代码

```cpp
    /*HEAD DELETE_ALL_UDOS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 新增的函数。文档提到："Note: UF_print_syslog is new in V18"，意思是在 V18 版本中新增加了这个函数。因此，我的回答是：UF_print_syslog 是 V18 新增的函数。 */  
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
            n = 0;  
        tag_t  
            part = UF_PART_ask_display_part(),  
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
            if (ask_yes_or_no("ARE YOU SURE?","DELETE All UDOs?" ))  
                UF_OBJ_delete_array_of_objects(n, udos, NULL);  
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

> 根据代码的内容，这是一个用于删除NX模型中所有用户定义对象(UDOs)的NX Open C++应用程序。以下是代码的主要功能和步骤：
>
> 1. 初始化NX环境并定义错误报告函数report_error()。
> 2. 定义一个询问用户是否确定删除所有UDOs的对话框函数ask_yes_or_no()。
> 3. 定义一个函数ask_next_udo()来循环遍历模型中的UDOs。
> 4. 定义一个函数report_udo_info()来报告每个UDO的信息，包括数据状态、整型、浮点数、字符串、链接、长度、面积和体积等。
> 5. 定义一个函数allocate_memory()来分配内存。
> 6. 定义一个函数make_an_array()来将UDO列表转换为数组。
> 7. 在do_it()函数中，获取当前显示的部件，并循环遍历所有UDOs，将它们的信息报告到列表窗口，并将UDO标签添加到列表中。
> 8. 调用make_an_array()函数将UDO列表转换为数组。
> 9. 如果找到UDOs，询问用户是否确定删除，如果确定，则调用UF_OBJ_delete_array_of_objects()删除所有UDOs。
> 10. 最后，释放数组内存。
> 11. 在ufusr()函数中，初始化NX环境，调用do_it()函数执行UDOs的删除，然后终止NX环境。
> 12. 定义一个卸载函数，用于立即卸载应用程序。
>
> 总的来说，这段代码实现了在NX模型中查找、报告并删除所有用户定义对象的功能。
>
