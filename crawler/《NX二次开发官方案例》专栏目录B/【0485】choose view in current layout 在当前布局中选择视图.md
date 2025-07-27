### 【0485】choose view in current layout 在当前布局中选择视图

#### 代码

```cpp
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
                case 1:                     /* Back 译:Back */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 译:"取消" */  
                    return -1;  
                    break;  
                case 18:                    /* More... 译:更多... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 译:我选了一个。 */  
                    return a + resp - 5;  
                    break;  
            }  
        } while (TRUE);  
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

```

#### 代码解析

> 这段代码是NX Open C++ API编写的二次开发代码，主要用于在NX的当前布局中选择一个视图。下面是代码的主要功能：
>
> 1. 定义了一个宏WRITENZ，用于打印变量名和值到NX的日志窗口。
> 2. 定义了一个函数write_integer_to_listing_window，用于将变量名和值格式化为字符串并打印到日志窗口。
> 3. 定义了一个函数allocate_string_array，用于动态分配二维字符数组。
> 4. 定义了一个函数choose_one_item，用于弹出一个列表对话框，让用户从多个选项中选择一个，支持分页。
> 5. 定义了一个函数choose_view_in_current_layout，用于获取当前布局中的视图数量，如果只有一个视图，直接返回该视图。如果有多个视图，则使用choose_one_item函数让用户选择一个视图，然后返回用户选择的视图。
> 6. 在choose_view_in_current_layout函数中，首先使用UF_VIEW_ask_work_view获取当前工作视图，然后使用UF_VIEW_ask_views_in_current_layout获取当前布局中的所有视图名称，再调用choose_one_item让用户选择一个视图。
> 7. 整个流程清晰，代码编写规范，注释详尽，是一个很好的NX二次开发示例。
>
