### 【1371】highlight all objects of specified type 高亮指定类型的所有对象

#### 代码

```cpp
    /*HEAD HIGHLIGHT_ALL_OBJECTS_OF_SPECIFIED_TYPE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
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
    static logical prompt_for_an_integer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strncpy(&menu[0][0], item, 15);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %d\n", title, n);  
        ECHO(msg);  
    }  
    static void do_it(void)  
    {  
        int  
            n_objs,  
            type = 2;  
        tag_t  
            *objs,  
            part = UF_PART_ask_display_part();  
        while (prompt_for_an_integer("Highlight", "Type number", &type))  
        {  
            n_objs = ask_all_of_type(part, type, &objs);  
            WRITE_D(n_objs);  
            if (n_objs > 0)  
            {  
                set_highlight_object_array(n_objs, objs, TRUE);  
                uc1601("Objects Highlighted", TRUE);  
                set_highlight_object_array(n_objs, objs, FALSE);  
                UF_free(objs);  
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

> 这段代码是一个NX Open API二次开发程序，主要用于在NX中高亮显示指定类型的对象。
>
> 主要功能包括：
>
> 1. 定义了一个报告错误的函数report_error，用于打印错误信息。
> 2. 定义了一个内存分配的函数allocate_memory，用于分配内存。
> 3. 定义了一个从列表创建数组的函数make_an_array，用于从对象列表创建对象数组。
> 4. 定义了一个查询指定类型对象的函数ask_all_of_type，用于查询指定类型对象并返回对象数组。
> 5. 定义了一个设置对象高亮的函数set_highlight_object_array，用于设置对象的高亮显示。
> 6. 定义了一个提示输入整数的函数prompt_for_an_integer，用于提示用户输入一个整数。
> 7. 定义了一个将整数写入日志窗口的函数write_integer_to_listing_window，用于将整数写入日志窗口。
> 8. 定义了一个主函数do_it，用于循环提示用户输入对象类型，然后高亮显示该类型的所有对象。
> 9. 定义了ufusr函数，作为程序的入口，初始化并调用do_it函数。
> 10. 定义了ufusr_ask_unload函数，用于卸载程序。
>
> 通过这个程序，用户可以方便地在NX中高亮显示指定类型的对象，以进行查看和分析。
>
