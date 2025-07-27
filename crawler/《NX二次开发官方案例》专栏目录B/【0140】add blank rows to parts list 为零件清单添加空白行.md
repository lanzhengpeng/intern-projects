### 【0140】add blank rows to parts list 为零件清单添加空白行

#### 代码

```cpp
    /*HEAD ADD_BLANK_ROWS_TO_PARTS_LIST CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_plist.h>  
    #include <uf_tabnot.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog is new in V18 */  
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
    static int ask_object_layer(tag_t object)  
    {  
        logical  
            is_displayable;  
        UF_OBJ_disp_props_t  
            disp_props;  
        UF_CALL(UF_OBJ_is_displayable(object, &is_displayable));  
        if (is_displayable &&  
            !UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props)))  
                return disp_props.layer;  
        else  
            return -1;  
    }  
    static tag_t ask_parts_list_tag(tag_t part, tag_t plist)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_tabular_note_type,  
            &plist)) && (plist != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(plist, &type, &subtype));  
            if ((subtype == UF_parts_list_subtype) &&  
                (ask_object_layer(plist) > 0)) return plist;  
        }  
        return NULL_TAG;  
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
    static void do_it(void)  
    {  
        int  
            n,  
            where;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            plist,  
            row;  
        double  
            height;  
        if ((plist = ask_parts_list_tag(part, NULL_TAG)) == NULL_TAG) return;  
        UF_CALL(UF_TABNOT_enable_automatic_update(TRUE));  
        UF_CALL(UF_TABNOT_ask_nm_rows(plist, &n));  
        where = n;  
        while (prompt_for_an_integer("Add row to Parts List", "After row", &where))  
        {  
            if (where > n) where = n;  
            UF_CALL(UF_TABNOT_ask_nth_row(plist, where-1, &row));  
            UF_CALL(UF_TABNOT_ask_row_height(row, &height));  
            UF_CALL(UF_PLIST_create_manual_row(height, &row));  
            UF_CALL(UF_TABNOT_add_row(plist, row, where));  
            n = n + 1;  
        }  
    }  
    /* 里海 */  
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

> 这段代码是一个NX的二次开发示例，主要功能是在NX部件的零件清单(Parts List)中添加空白行。
>
> 主要步骤包括：
>
> 1. 初始化NX环境。
> 2. 获取当前显示的部件，然后在该部件中循环查找类型为零件清单的表格对象。
> 3. 如果找到零件清单对象，启用自动更新功能，并获取当前行数。
> 4. 通过用户交互提示用户输入要添加空白行的位置。
> 5. 根据用户输入的位置，创建一个高度为前一行高度的空白行，并插入到零件清单中。
> 6. 重复步骤4和5，直到用户不再添加行。
> 7. 结束NX环境。
>
> 主要函数包括：
>
> 该代码通过二次开发实现了NX标准功能中不直接提供的在零件清单中添加空白行的功能，扩展了NX的功能，提高了用户的便利性。
>
