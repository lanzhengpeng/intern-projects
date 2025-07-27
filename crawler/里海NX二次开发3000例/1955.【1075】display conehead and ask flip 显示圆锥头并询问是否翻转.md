### 【1075】display conehead and ask flip 显示圆锥头并询问是否翻转

#### 代码

```cpp
    /*HEAD DISPLAY_CONEHEAD_AND_ASK_FLIP CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_view.h>  
    #include <uf_disp.h>  
    #include <uf_vec.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的函数，用于打印系统日志信息。 */  
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
    static void display_conehead(double origin[3], double vec[3])  
    {  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
    }  
    static tag_t ask_next_visible_solid(tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(uc6457("", 1, &body)) && (body != NULL_TAG))  
        {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
                if ((type == UF_solid_type) && (subtype == UF_solid_body_subtype))  
                    return body;  
        }  
        return NULL_TAG;  
    }  
    static void refresh_all_solids(void)  
    {  
        tag_t  
            solid = NULL_TAG;  
        while ((solid = ask_next_visible_solid(solid)) != NULL_TAG)  
            UF_CALL(UF_DISP_add_item_to_display(solid));  
    }  
    static void erase_conehead(double origin[3], double vec[3])  
    {  
        int  
            old_color;  
        UF_DISP_conehead_attrb_s  
            attributes;  
        UF_VIEW_visualization_t  
            view_data;  
        UF_DISP_get_conehead_attrb(&attributes);  
        old_color = attributes.color;  
        attributes.color = -1;  
        UF_DISP_set_conehead_attrb(&attributes);  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
        UF_CALL(UF_VIEW_ask_visualization(NULL_TAG, &view_data));  
        if (view_data.display_mode != UF_VIEW_NOT_SHADED) refresh_all_solids();  
        attributes.color = old_color;  
        UF_DISP_set_conehead_attrb(&attributes);  
    }  
    static logical ask_yes_or_no(char *prompt, logical *deflt)  
    {  
        int  
            resp;  
        char  
            options[2][38] = { "Yes", "No" };  
        resp = 2 - *deflt;  
        resp = uc1603(prompt, resp, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
            *deflt = 6 - resp;  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        double  
            anchor[3] = {0.0, 0.0, 0.0},  
            vector[3] = {1.0, 1.0, 1.0};  
        logical  
            flip = TRUE;  
        display_conehead(anchor, vector);  
        while (ask_yes_or_no("Flip conehead", &flip))  
        {  
            erase_conehead(anchor, vector);  
            if( flip ) UF_VEC3_negate(vector, vector);  
            display_conehead(anchor, vector);  
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

> 这段代码是一个NX二次开发的用户函数，其主要功能是显示和隐藏一个圆锥头，并询问用户是否要翻转圆锥头。下面是代码的主要逻辑：
>
> 1. 定义了错误报告函数report_error，用于在调用NX API函数出错时打印错误信息。
> 2. 定义了显示圆锥头的函数display_conehead，使用NX的UF_DISP_conehead函数在当前活动视图中显示一个圆锥头。
> 3. 定义了询问下一个可见实体的函数ask_next_visible_solid，用于遍历模型树，找到下一个可见的实体。
> 4. 定义了刷新所有实体的函数refresh_all_solids，用于在视图模式改变后刷新所有实体的显示。
> 5. 定义了擦除圆锥头的函数erase_conehead，用于擦除已显示的圆锥头。
> 6. 定义了询问用户是否的函数ask_yes_or_no，用于以对话框形式询问用户问题，并返回用户的选择。
> 7. 定义了主函数do_it，用于显示圆锥头，并循环询问用户是否要翻转圆锥头，根据用户的选择翻转或保持圆锥头，并重新显示。
> 8. 定义了ufusr函数，作为用户函数的入口点，初始化NX环境，调用主函数，然后终止NX环境。
> 9. 定义了卸载函数ufusr_ask_unload，用于在卸载用户函数时返回立即卸载。
>
> 总的来说，这段代码实现了在NX中显示圆锥头，并根据用户的选择翻转或保持圆锥头的功能。
>
