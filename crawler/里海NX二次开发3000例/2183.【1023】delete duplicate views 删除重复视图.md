### 【1023】delete duplicate views 删除重复视图

#### 代码

```cpp
    /*HEAD DELETE_DUPLICATE_VIEWS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是V18版本中新增的函数。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
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
    static int delete_all_duplicate_views(tag_t part)  
    {  
        int  
            ii,  
            jj,  
            dupes = 0,  
            mm = 0,  
            n,  
            error_flag,  
            type = UF_view_type;  
        tag_t  
            dupvws[100],   
            *objects;  
        char  
            view[MAX_ENTITY_NAME_SIZE+1],  
            iname[MAX_ENTITY_NAME_SIZE+1],  
            jname[MAX_ENTITY_NAME_SIZE+1],  
            msg[133];  
        uf_list_p_t  
            object_list;  
        logical  
            already_in_array;  
            n = ask_all_of_type(part, UF_view_type, &objects);  
            sprintf(msg, "%d objects of type %d", n, type);  
            uc1601(msg, TRUE);  
            for(ii = 0; ii < n; ii++)  
            {  
                strcpy(iname, "");  
                UF_OBJ_ask_name(objects[ii], iname);  
                UF_CALL(UF_MODL_create_list(&object_list));  
                for(jj = ii+1; jj < n; jj++)  
                {  
                    strcpy(jname, "");  
                    UF_OBJ_ask_name(objects[jj], jname);  
                    if((strcmp(jname,iname)) == 0)  
                    {  
                        already_in_array = FALSE;  
                        for(mm = 0; mm < jj; mm++)  
                        {  
                            if(objects[jj] == dupvws[mm])  
                            {  
                                already_in_array = TRUE;  
                            }  
                        }  
                        if(!already_in_array)  
                        {  
                            dupvws[dupes] = objects[jj];  
                            dupes++;  
                        }  
                    }  
                }  
            }  
            if (dupes > 0)  
            {  
                sprintf(msg, "delete %d duplicate views?", dupes);  
                if (ask_yes_or_no("Confirm", msg))  
                {  
                    for (ii = 0; ii < dupes; ii++)  
                    {  
                        UF_CALL(UF_OBJ_ask_name(dupvws[ii], view));  
                        WRITE("Deleting ");  
                        sprintf(msg, "%s, TAG: %d", view, dupvws[ii]);  
                        WRITE_S(msg);  
                        UF_VIEW_delete(dupvws[ii], &error_flag);  
                        if(error_flag != 0)  
                            WRITE_D(error_flag);  
                    }  
                }  
            }  
            else  
            {  
                uc1601("No Duplicate Views", TRUE);  
            }  
            return dupes;  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            msg[133];  
        n = delete_all_duplicate_views(part);  
        sprintf(msg, "%d duplicate views processed\n", n);  
        uc1601(msg, TRUE);  
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

> 根据代码内容，这是NX Open C++ API编写的NX二次开发代码，主要用于删除NX部件中的重复视图。以下是主要功能：
>
> 1. 定义了UF_CALL宏，用于调用NX API函数，并在出错时记录错误信息。
> 2. 实现了report_error函数，用于输出错误信息。
> 3. 定义了WRITE、WRITE_D、WRITE_S等宏，用于向日志窗口输出信息。
> 4. 实现了allocate_memory、make_an_array、ask_all_of_type等辅助函数。
> 5. 实现了delete_all_duplicate_views函数，用于删除重复视图。首先遍历所有视图，如果发现重复名称的视图，则将其添加到dupvws数组中。然后提示用户确认删除，如果确认，则调用UF_VIEW_delete函数删除重复视图。
> 6. 实现了do_it函数，用于获取当前显示的部件，并调用delete_all_duplicate_views删除重复视图。
> 7. ufusr函数为NX二次开发的入口函数，用于初始化NX API，调用do_it执行删除重复视图的功能，然后终止NX API。
> 8. ufusr_ask_unload函数用于卸载二次开发程序。
>
> 总体来说，该代码实现了遍历部件视图、检测重复视图、删除重复视图的功能，并提供了友好的用户提示。
>
