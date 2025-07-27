### 【0497】close all parts except displayed part and its children 关闭除显示部件及其子部件外的所有部件

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_styler.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    #define GTAC_ACTION_0                  ("ACTION_0")  
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
        int  
            ii;  
        char  
            msg[133];  
        if (status->n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_get_fail_message(status->statuses[ii], msg);  
                ECHO("  %s - %s\n", status->file_names[ii], msg);  
            }  
            ECHO("\n");  
            UF_CALL(UF_PART_free_load_status(status));  
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
    static void close_all_other_parts(tag_t dontClose)  
    {  
        int  
            ii,  
            n_parts;  
        tag_t  
            *parts;  
        uf_list_p_t  
            parts_list;  
        if ((n_parts = UF_PART_ask_num_parts()) == 0) return;  
        UF_CALL(UF_MODL_create_list(&parts_list));  
        for (ii = 0; ii < n_parts; ii++)  
            UF_CALL(UF_MODL_put_list_item(parts_list, UF_PART_ask_nth_part(ii)));  
        n_parts = ask_all_assembly_parts(dontClose, &parts);  
        for (ii = 0; ii < n_parts; ii++)  
            UF_CALL(UF_MODL_delete_list_item(&parts_list, parts[ii]));  
        if (n_parts > 0) UF_free(parts);  
        n_parts = make_an_array(&parts_list, &parts);  
        for (ii = 0; ii < n_parts; ii++)  
            UF_CALL(UF_PART_close(parts[ii], 0, 1));  
        if (n_parts > 0) UF_free(parts);  
    }  
    /* 里海 */  
    static int GTAC_close_all_but_displayed_part_cb(int dialog_id,  
        void *client_data, UF_STYLER_item_value_type_p_t callback_data)  
    {  
        close_all_other_parts(UF_PART_ask_display_part());  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        UF_STYLER_callback_info_t  
            GTAC_cbs[2] = {  
                 {GTAC_ACTION_0, UF_STYLER_ACTIVATE_CB, 0,  
                    GTAC_close_all_but_displayed_part_cb},  
                 {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };  
        UF_CALL(UF_STYLER_create_dialog("close_all_other_parts.dlg", GTAC_cbs, 2,  
            NULL, &resp));  
    }  
    /* 里海 */  
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

> 根据代码内容，这是一段用于NX的二次开发代码，其主要功能是关闭除当前显示部件外的所有部件。具体来说：
>
> 1. 定义了报告错误函数report_error，用于在发生UF函数调用错误时打印错误信息。
> 2. 定义了关闭除指定部件外的所有部件的函数close_all_other_parts。
> 3. 定义了对话框回调函数GTAC_close_all_but_displayed_part_cb，用于在用户点击对话框按钮时调用close_all_other_parts函数。
> 4. 定义了创建并显示对话框的函数do_it。
> 5. ufusr函数是NX二次开发的入口函数，用于初始化、调用do_it函数、以及终止二次开发。
> 6. ufusr_ask_unload函数用于处理二次开发的卸载。
>
> 综上所述，这段代码通过创建一个简单的对话框，在用户点击按钮时，关闭除当前显示部件外的所有部件。这为NX用户提供了方便的二次开发功能。
>
