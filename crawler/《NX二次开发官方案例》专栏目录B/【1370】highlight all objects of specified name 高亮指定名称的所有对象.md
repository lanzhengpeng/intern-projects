### 【1370】highlight all objects of specified name 高亮指定名称的所有对象

#### 代码

```cpp
    /*HEAD HIGHLIGHT_ALL_OBJECTS_OF_SPECIFIED_NAME CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <ctype.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数，用于打印系统日志信息。

因此，翻译后的内容为：

翻译：UF_print_syslog是V18版本中新增的函数，用于打印系统日志信息。 */  
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
    static tag_t ask_next_of_name(char *name, tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_by_name(name, &object));  
        return object;  
    }  
    static int ask_all_of_name(char *name, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while ((object = ask_next_of_name(name, object)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
        return make_an_array(&object_list, objects);  
    }  
    static tag_t ask_next_of_name_prefix(char *prefix, tag_t part, tag_t object)  
    {  
        int  
            ii;  
        char  
            fullname[MAX_ENTITY_NAME_SIZE+1];  
     /*  make sure the name is all in upper case 里海译:确保名字全部使用大写字母。 */  
        if (object == NULL_TAG)  
            for (ii = 0; ii < strlen(prefix); ii++)  
                prefix[ii] = toupper(prefix[ii]);  
        while ((object = UF_OBJ_cycle_all(part, object)) != NULL_TAG)  
            if ((!UF_OBJ_ask_name(object, fullname)) &&  
                (strstr(fullname, prefix) == fullname))  
                break;  
        return object;  
    }  
    static int ask_all_of_name_prefix(char *prefix, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while ((object = ask_next_of_name_prefix(prefix, part, object)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
        return make_an_array(&object_list, objects);  
    }  
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int ii;  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
    }  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            n_objects,  
            resp;  
        tag_t  
            *objects;  
        char  
            msg[133],  
            *messages[1],  
            name[MAX_ENTITY_NAME_SIZE+1] = { "" },  
            prefix[MAX_ENTITY_NAME_SIZE+1];  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, FALSE, NULL, NULL, NULL, 1, 0, 2 };  
        messages[0] = msg;  
        while (prompt_for_text("Verify Name", name))  
        {  
            if (strstr(name, "*"))  
            {  
                strcpy(prefix, strtok(name, "*"));  
                sprintf(name, "%s*", prefix);  
                n_objects = ask_all_of_name_prefix(prefix, &objects);  
            }  
            else  
                n_objects = ask_all_of_name(name, &objects);  
            sprintf(msg, "%s = %d objects", name, n_objects);  
            if (n_objects > 0)  
            {  
                set_highlight_object_array(n_objects, objects, TRUE);  
                UF_CALL(UF_UI_message_dialog("Verify Name",  
                    UF_UI_MESSAGE_INFORMATION, messages, 1, TRUE, &buttons, &resp));  
                set_highlight_object_array(n_objects, objects, FALSE);  
                UF_free(objects);  
            }  
            else  
                UF_CALL(UF_UI_message_dialog("Verify Name",  
                    UF_UI_MESSAGE_INFORMATION, messages, 1, TRUE, &buttons, &resp));  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 错误处理：使用了UF_CALL宏来包装NX API调用，并在出错时报告错误信息。
> 2. 内存分配：定义了allocate_memory函数用于分配内存。
> 3. 对象查询：ask_all_of_name函数用于查询指定名称的所有对象，并返回对象数组。
> 4. 对象高亮：set_highlight_object_array函数用于设置对象数组的高亮状态。
> 5. 用户输入：prompt_for_text函数用于提示用户输入文本。
> 6. 对象遍历：ask_next_of_name_prefix函数用于遍历具有指定前缀的对象。
> 7. 交互提示：do_it函数实现了整个流程，包括提示用户输入对象名称，查询对象并高亮显示，然后提示用户确认。
> 8. API初始化：在ufusr函数中，首先初始化NX API，然后调用do_it函数，最后终止NX API。
> 9. 卸载处理：ufusr_ask_unload函数用于处理插件卸载时的清理工作。
>
> 总的来说，这段代码实现了通过交互方式查询并高亮显示NX模型中指定名称的对象，并提供了完善的错误处理和用户提示功能。
>
