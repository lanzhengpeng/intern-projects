### 【1373】highlight all objects with name prefix 高亮所有以特定前缀命名的对象

#### 代码

```cpp
    /*HEAD HIGHLIGHT_ALL_OBJECTS_WITH_NAME_PREFIX CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_assem.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
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
    static tag_t ask_next_of_name_prefix(char *prefix, tag_t part, tag_t object)  
    {  
        int  
            ii;  
        char  
            fullname[MAX_ENTITY_NAME_SIZE+1];  
     /*  make sure the name is all in upper case 里海译:确保名字全部为大写。 */  
        if (object == NULL_TAG)  
            for (ii = 0; ii < strlen(prefix); ii++)  
                prefix[ii] = toupper(prefix[ii]);  
        while ((object = UF_OBJ_cycle_all(part, object)) != NULL_TAG)  
            if ((!UF_OBJ_ask_name(object, fullname)) &&  
                (strstr(fullname, prefix) == fullname))  
                break;  
        return object;  
    }  
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int  
            ii,  
            subtype,  
            type;  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(objects[ii], &type, &subtype));  
            if (type == UF_occ_instance_type)  
                UF_CALL(UF_DISP_set_highlight(UF_ASSEM_ask_part_occ_of_inst(  
                    NULL_TAG, objects[ii]), flag));  
            else  
                UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
        }  
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
            n;  
        tag_t  
            *objs;  
        char  
            msg[UF_UI_MAX_STRING_LEN+1],  
            prefix[MAX_ENTITY_NAME_SIZE+1] = { "" };  
        while (prompt_for_text("Highlight named objects by prefix", prefix))  
        {  
            n = ask_all_of_name_prefix(prefix, &objs);  
            if (n > 0)  
            {  
                set_highlight_object_array(n, objs, TRUE);  
                sprintf(msg, "%d objects named %s* highlighted", n, prefix);  
                uc1601(msg, TRUE);  
                set_highlight_object_array(n, objs, FALSE);  
                UF_free(objs);  
            }  
            else  
            {  
                sprintf(msg, "No objects named %s* found", prefix);  
                uc1601(msg, TRUE);  
            }  
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

> 这段代码是NX的二次开发代码，实现了以下功能：
>
> 1. 定义了一个echo函数，用于在列表窗口、系统日志窗口输出信息。
> 2. 定义了一个report_error函数，用于输出错误信息。
> 3. 定义了一个allocate_memory函数，用于分配内存。
> 4. 定义了一个make_an_array函数，用于将对象列表转换为对象数组。
> 5. 定义了一个ask_next_of_name_prefix函数，用于查询具有特定前缀名称的对象。
> 6. 定义了一个set_highlight_object_array函数，用于设置对象的高亮显示。
> 7. 定义了一个ask_all_of_name_prefix函数，用于查询所有具有特定前缀名称的对象。
> 8. 定义了一个prompt_for_text函数，用于提示用户输入文本。
> 9. 定义了一个do_it函数，用于实现主体功能：提示用户输入对象名称前缀，查询并高亮显示所有匹配的对象。
> 10. 定义了ufusr函数，用于初始化NX系统，调用do_it函数，然后终止NX系统。
> 11. 定义了一个ufusr_ask_unload函数，用于卸载用户自定义函数。
>
> 这段代码的主要功能是根据用户输入的对象名称前缀，查询并高亮显示所有匹配的对象。
>
