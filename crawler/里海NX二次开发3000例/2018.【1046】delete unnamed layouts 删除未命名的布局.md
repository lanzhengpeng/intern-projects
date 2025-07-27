### 【1046】delete unnamed layouts 删除未命名的布局

#### 代码

```cpp
    /*HEAD DELETE_UNNAMED_LAYOUTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的功能。

翻译如下：

注意：UF_print_syslog是在V18版本中新增的功能，请只回答译文，不要添加其他内容。 */  
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
    static int ask_all_unnamed_of_type(tag_t part, int type, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object)) &&  
                (object != NULL_TAG))  
        {  
            strcpy(name, "");  
            UF_OBJ_ask_name(object, name);  
            if(strlen(name) < 1)  
                UF_CALL(UF_MODL_put_list_item(object_list, object));  
        }  
        return (make_an_array(&object_list, objects));  
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
    static void do_it(void)  
    {  
        int  
            n,  
            type = UF_layout_type;  
        tag_t  
            *objects,  
            part = UF_PART_ask_display_part();  
        char  
            msg[133];  
            n = ask_all_unnamed_of_type(part, type, &objects);  
            sprintf(msg, "%d objects of type %d", n, type);  
            if (n > 0)  
            {  
                uc1601(msg, TRUE);  
                sprintf(msg, "delete %d objects of type %d?", n, type);  
                if (ask_yes_or_no("Confirm", msg))  
                UF_CALL(UF_OBJ_delete_array_of_objects(n, objects, NULL));  
                UF_free(objects);  
            }  
            else  
            {  
                uc1601(msg, TRUE);  
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

> 这段代码是用于NX的二次开发，主要功能是删除当前打开的部件中所有未命名的布局。代码流程如下：
>
> 1. 包含了NX二次开发所需的头文件。
> 2. 定义了错误报告函数report_error，用于打印错误信息。
> 3. 定义了内存分配函数allocate_memory，用于申请内存。
> 4. 定义了创建数组函数make_an_array，用于将链表转换为数组。
> 5. 定义了查询所有未命名对象函数ask_all_unnamed_of_type，用于获取部件中所有未命名的布局。
> 6. 定义了询问是否删除函数ask_yes_or_no，用于询问用户是否删除对象。
> 7. 定义了执行删除操作的函数do_it。
> 8. 定义了ufusr函数，初始化NX环境，调用do_it函数执行删除操作，然后终止NX环境。
> 9. 定义了卸载函数ufusr_ask_unload，返回立即卸载。
> 10. 整个代码通过调用NX的API，实现了查询未命名布局并提示用户删除的功能。
>
