### 【1012】delete all of type 删除所有类型

#### 代码

```cpp
    /*HEAD DELETE_ALL_OF_TYPE CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog is new in V18，这是一个新的功能，在V18版本中新增。 */  
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
    static int ask_all_of_type(tag_t part, int type, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object)) &&  
                (object != NULL_TAG))  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
        return (make_an_array(&object_list, objects));  
    }  
    static logical prompt_for_an_integer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int ii;  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
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
            type = 0;  
        tag_t  
            *objects,  
            part = UF_PART_ask_display_part();  
        char  
            msg[133];  
        while (prompt_for_an_integer("Delete objects of type", "Type", &type))  
        {  
            n = ask_all_of_type(part, type, &objects);  
            if (n > 0)  
            {  
                set_highlight_object_array(n, objects, TRUE);  
                sprintf(msg, "delete %d objects of type %d?", n, type);  
                if (ask_yes_or_no("Confirm", msg))  
                    UF_CALL(UF_OBJ_delete_array_of_objects(n, objects, NULL));  
                else set_highlight_object_array(n, objects, FALSE);  
                UF_free(objects);  
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

> 这段NX二次开发代码实现了删除当前显示部件中指定类型对象的功能。主要步骤包括：
>
> 1. 定义错误报告函数report_error，用于打印错误信息。
> 2. 定义内存分配函数allocate_memory，用于申请内存。
> 3. 定义从列表中提取对象ID数组的函数make_an_array，用于获取指定类型的对象。
> 4. 定义查询部件中指定类型对象的函数ask_all_of_type，用于获取当前显示部件中指定类型的所有对象。
> 5. 定义提示输入整数的函数prompt_for_an_integer，用于提示用户输入要删除的对象类型。
> 6. 定义设置对象高亮的函数set_highlight_object_array，用于高亮显示要删除的对象。
> 7. 定义确认删除的函数ask_yes_or_no，用于让用户确认删除操作。
> 8. 定义执行删除操作的函数do_it，用于循环获取对象类型，查询对象，提示用户确认，并执行删除。
> 9. 定义ufusr函数，作为二次开发程序的入口函数，初始化后执行do_it，然后结束。
> 10. 定义ufusr_ask_unload函数，返回立即卸载标志，表示二次开发程序卸载后不需要重启NX。
>
> 整体而言，这段代码实现了通过交互方式，让用户指定对象类型，然后删除当前显示部件中所有该类型的对象。
>
