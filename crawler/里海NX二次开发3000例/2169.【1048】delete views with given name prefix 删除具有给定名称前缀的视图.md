### 【1048】delete views with given name prefix 删除具有给定名称前缀的视图

#### 代码

```cpp
    /*HEAD DELETE_VIEWS_WITH_GIVEN_NAME_PREFIX CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <ctype.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本新增的功能。 */  
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
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
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
    static int ask_all_of_type_and_name_prefix(tag_t part, int type, char *prefix,  
        tag_t **objects)  
    {  
        int  
            ii;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        char  
            fullname[MAX_ENTITY_NAME_SIZE+1];  
    /*  make sure the name is all in upper case 里海译:确保名称全部使用大写字母。 */  
        for (ii = 0; ii < strlen(prefix); ii++) prefix[ii] = toupper(prefix[ii]);  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object)) &&  
            (object != NULL_TAG))  
        {  
            if ((!UF_OBJ_ask_name(object, fullname)) &&  
                (strstr(fullname, prefix) == fullname))  
                UF_CALL(UF_MODL_put_list_item(object_list, object));  
        }  
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
            error_flag, ii, n;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            *views;  
        char  
            prefix[MAX_ENTITY_NAME_SIZE+1] = { "" },  
            view[MAX_ENTITY_NAME_SIZE+1];  
        while (prompt_for_text("Delete views - Enter name prefix", prefix))  
        {  
            n = ask_all_of_type_and_name_prefix(part, UF_view_type, prefix, &views);  
            for (ii = 0; ii < n; ii++)  
            {  
                UF_CALL(UF_OBJ_ask_name(views[ii], view));  
                WRITE("Deleting ");  
                WRITE_S(view);  
                UF_VIEW_delete(views[ii], &error_flag);  
                WRITENZ(error_flag);  
            }  
            if (n > 0) UF_free(views);  
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

> 这段NX二次开发代码实现了删除具有给定前缀名称的所有视图的功能。代码的主要步骤如下：
>
> 1. 定义了错误报告函数report_error，用于在出现UF函数调用错误时打印错误信息。
> 2. 定义了写整数到日志窗口的函数write_integer_to_listing_window，用于在日志窗口中输出整数值。
> 3. 定义了写字符串到日志窗口的函数write_string_to_listing_window，用于在日志窗口中输出字符串。
> 4. 定义了分配内存的函数allocate_memory，用于为对象数组分配内存。
> 5. 定义了创建对象数组的函数make_an_array，用于从对象列表创建对象数组并释放列表。
> 6. 定义了查询给定类型和名称前缀的所有对象的函数ask_all_of_type_and_name_prefix。
> 7. 定义了提示用户输入文本的函数prompt_for_text。
> 8. 定义了执行删除操作的函数do_it，包括获取当前显示的部件，提示用户输入视图名称前缀，查询所有匹配的视图，并逐个删除。
> 9. 定义了ufusr函数，在初始化NX后调用do_it函数，执行删除操作，并在完成后终止NX。
> 10. 定义了卸载函数ufusr_ask_unload，用于卸载当前NX会话。
>
> 综合来看，这段代码通过交互方式实现了按视图名称前缀删除视图的功能，利用了UF函数进行对象遍历、查询和删除等操作。
>
