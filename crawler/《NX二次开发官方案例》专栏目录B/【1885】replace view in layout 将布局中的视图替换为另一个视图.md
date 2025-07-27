### 【1885】replace view in layout 将布局中的视图替换为另一个视图

#### 代码

```cpp
    /*HEAD REPLACE_VIEW_IN_LAYOUT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_layout.h>  
    #include <uf_view.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog是V18版本新增的功能，只回答翻译，不要发表无关的评论。 */  
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
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
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
    static tag_t ask_next_view(tag_t part, tag_t view)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_view_type, &view));  
        return (view);  
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
    static int ask_all_modeling_views(tag_t part, tag_t **views)  
    {  
        tag_t  
            view = NULL_TAG;  
        uf_list_p_t  
            view_list;  
        UF_VIEW_type_t  
            type;  
        UF_VIEW_subtype_t  
            subtype;  
        UF_CALL(UF_MODL_create_list(&view_list));  
        while ((view = ask_next_view(part, view)) != NULL_TAG)  
        {  
            UF_CALL(UF_VIEW_ask_type(view, &type, &subtype));  
            if (type == UF_VIEW_MODEL_TYPE)  
                UF_CALL(UF_MODL_put_list_item(view_list, view));  
        }  
        return (make_an_array(&view_list, views));  
    }  
    static int allocate_string_array(int n_elements, int n_chars, char ***where)  
    {  
        int  
            ii,  
            resp;  
        *where = UF_allocate_memory(n_elements * sizeof(char *), &resp);  
        for (ii = 0; ii < n_elements; ii++)  
        {  
            (*where)[ii] = UF_allocate_memory(n_chars * sizeof(char), &resp);  
            if (resp) return resp;  
        }  
        return 0;  
    }  
    static int choose_one_item(char *prompt, char **choices, int n_choices)  
    {  
        int  
            a = 0,  
            ii,  
            resp,  
            z;  
        char  
            options[14][38];  
        do  
        {  
            z = (n_choices - a < 14) ? n_choices - a : 14;  
            for (ii = 0; ii < z; ii++)  
            {  
                strncpy(&options[ii][0], choices[a+ii], 37);  
                options[ii][37] = '\0';  
            }  
            if ((z == 14) && ((a + z) < n_choices))  
                strcpy(&options[13][0], "More...");  
            switch (resp = uc1603(prompt, 0, options, z))  
            {  
                case 1:                     /* Back 里海译:Back */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 里海译:取消 */  
                    return -1;  
                    break;  
                case 18:                    /* More... 里海译:更多... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 里海译:选择一个 */  
                    return a + resp - 5;  
                    break;  
            }  
        } while (TRUE);  
    }  
    static tag_t choose_a_modeling_view(char *prompt)  
    {  
        int  
            ii,  
            n_views,  
            which;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            the_view,  
            *views;  
        char  
            **view_names;  
        n_views = ask_all_modeling_views(part, &views);  
        UF_CALL(allocate_string_array(n_views, MAX_ENTITY_NAME_SIZE+1, &view_names));  
        for (ii = 0; ii < n_views; ii++)  
            UF_CALL(UF_OBJ_ask_name(views[ii], view_names[ii]));  
        if ((which = choose_one_item(prompt, view_names, n_views)) == -1)  
            the_view = NULL_TAG;  
        else  
            the_view = views[which];  
        UF_free(views);  
        UF_free_string_array(n_views, view_names);  
        return the_view;  
    }  
    static tag_t choose_view_in_current_layout(char *prompt)  
    {  
        int  
            ii,  
            n_views,  
            which;  
        tag_t  
            the_view;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1],  
            **view_names;  
        WRITENZ(uc6472("", &n_views));  
        if (n_views == 1)  
        {  
            UF_CALL(UF_VIEW_ask_work_view(&the_view));  
        }  
        else  
        {  
            UF_CALL(allocate_string_array(n_views, MAX_ENTITY_NAME_SIZE+1, &view_names));  
            strcpy(view_name, "");  
            for (ii = 0; ii < n_views; ii++)  
            {  
                WRITENZ(uc6473("", view_name));  
                strcpy(view_names[ii], view_name);  
            }  
            if ((which = choose_one_item(prompt, view_names, n_views)) == -1)  
                the_view = NULL_TAG;  
            else  
            {  
                UF_CALL(UF_VIEW_ask_tag_of_view_name(view_names[which], &the_view));  
            }  
            UF_free_string_array(n_views, view_names);  
        }  
        return the_view;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            target,  
            tool;  
        char  
            replace[MAX_ENTITY_NAME_SIZE+1],  
            with[MAX_ENTITY_NAME_SIZE+1];  
        while (((target = choose_view_in_current_layout("View to Replace")) != NULL_TAG) &&  
               ((tool = choose_a_modeling_view("Replace with view")) != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_name(target, replace));  
            UF_CALL(UF_OBJ_ask_name(tool, with));  
            WRITE_S(replace);  
            WRITE_S(with);  
            WRITENZ(uc6464("", replace, with));  
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

> 根据代码内容，这段代码是用于NX的二次开发，主要实现了在当前布局中替换视图的功能。主要步骤包括：
>
> 1. 获取当前布局中的所有视图，并让用户选择要替换的视图。
> 2. 获取模型中的所有建模视图，并让用户选择用于替换的视图。
> 3. 使用UF_MODL_replace_view函数替换当前布局中的视图。
> 4. 重复上述过程，直到用户选择退出。
> 5. 辅助函数包括获取错误信息、分配内存、创建数组等。
> 6. ufusr函数是程序的入口，初始化NX环境，调用do_it函数执行替换逻辑，然后结束NX环境。
> 7. ufusr_ask_unload函数用于立即卸载程序。
>
> 总体来说，这段代码实现了在NX布局中替换视图的功能，让用户可以方便地选择视图进行替换。
>
