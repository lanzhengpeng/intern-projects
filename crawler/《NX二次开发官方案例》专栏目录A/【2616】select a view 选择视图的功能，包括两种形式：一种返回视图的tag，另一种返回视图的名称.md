### 【2616】select a view 选择视图的功能，包括两种形式：一种返回视图的tag，另一种返回视图的名称

#### 代码

```cpp
    /* There are two forms of the function       *  
     * select_a_view included in this file.      *  
     * One returns the tag of the view and       *  
     * the other returns  the view name (char*). *  
     * Be sure to delete the one you             *  
     * don't want to include in your program.    */  
    static tag_t select_a_view(char *prompt)  
    {  
        int  
            view_of_cursor,  
            resp;  
        double  
            pos[3];  
        char  
            vname[MAX_ENTITY_NAME_SIZE+1];  
        tag_t  
            view = NULL_TAG;  
        UF_CALL( UF_UI_ask_cursor_view( &view_of_cursor));  
        UF_CALL( UF_UI_set_cursor_view( 0));  
        resp = uc1615(prompt, pos);  
        UF_CALL( UF_UI_set_cursor_view( view_of_cursor));  
        if (resp == 5)  
        {  
            UF_CALL( uc1653( vname ));  
            UF_CALL( UF_VIEW_ask_tag_of_view_name(vname, &view));  
            return view;  
        }  
        else  
            return NULL_TAG;  
    }  
    static logical select_a_view( char *prompt, char *view_name)  
    {  
        int  
            ret_code,  
            view_of_cursor;  
        double  
            pos[3];  
        UF_CALL( UF_UI_ask_cursor_view( &view_of_cursor));  
        UF_CALL( UF_UI_set_cursor_view( 0));  
        ret_code = uc1615(prompt, pos);  
        UF_CALL( UF_UI_set_cursor_view( view_of_cursor));  
        if (ret_code == 5)  
        {  
            UF_CALL( uc1653( view_name ));  
            return TRUE;  
        }  
        else  
            return FALSE;  
    }

```

#### 代码解析

> 这段代码是NX Open API开发的两个函数，用于在NX中选择一个视图。
>
> 1. 第一个函数select_a_view(char *prompt)用于选择视图，返回视图的tag。其工作流程如下：获取当前光标所在的视图，并保存。设置光标视图为0，即选择世界坐标系视图。弹出提示框，让用户选择视图。如果用户选择了视图，则获取选中的视图名称。根据视图名称查询视图的tag，并返回。
> 2. 获取当前光标所在的视图，并保存。
> 3. 设置光标视图为0，即选择世界坐标系视图。
> 4. 弹出提示框，让用户选择视图。
> 5. 如果用户选择了视图，则获取选中的视图名称。
> 6. 根据视图名称查询视图的tag，并返回。
> 7. 第二个函数select_a_view(char *prompt, char *view_name)同样用于选择视图，但返回视图的名称。其工作流程如下：获取当前光标所在的视图，并保存。设置光标视图为0，即选择世界坐标系视图。弹出提示框，让用户选择视图。如果用户选择了视图，则获取选中的视图名称，并返回视图名称。如果用户未选择视图，则返回FALSE。
> 8. 获取当前光标所在的视图，并保存。
> 9. 设置光标视图为0，即选择世界坐标系视图。
> 10. 弹出提示框，让用户选择视图。
> 11. 如果用户选择了视图，则获取选中的视图名称，并返回视图名称。
> 12. 如果用户未选择视图，则返回FALSE。
>
> 这两个函数的主要区别在于返回值，一个返回视图的tag，一个返回视图的名称。用户可以根据需要选择使用。
>
