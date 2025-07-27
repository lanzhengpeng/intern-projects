### 【0395】assign string attributes to selected faces 将字符串属性分配给选定的面

#### 代码

```cpp
    /*HEAD ASSIGN_STRING_ATTRIBUTES_TO_SELECTED_FACES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_attr.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档描述，UF_print_syslog是在V18版本中新增的。 */  
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
    /* 里海 */  
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_faces(char *prompt, tag_t **faces)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select faces", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &cnt, faces));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*faces)[ii], FALSE));  
        return cnt;  
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
            ii,  
            n_faces;  
        tag_t  
            *faces;  
        char  
            str[UF_ATTR_MAX_STRING_LEN+1] = { "" },  
            title[UF_ATTR_MAX_TITLE_LEN+1] = { "" };  
        UF_ATTR_value_t  
            value;  
        value.type = UF_ATTR_string;  
    /*  put the space for the characters into the structure. 译:将字符的空间放入结构中。 */  
        value.value.string = str;  
        while ((n_faces = select_faces("Assign attributes", &faces)) > 0)  
        {  
            while ((prompt_for_text("Attribute title", title)) &&  
                   (prompt_for_text("Attribute value", value.value.string)))  
            {  
                for (ii = 0; ii < n_faces; ii++)  
                    UF_CALL(UF_ATTR_assign(faces[ii], title, value));  
            }  
            UF_free(faces);  
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

> 这段代码是用于NX软件的二次开发，其主要功能是让用户选择模型上的面，然后给这些面赋予用户指定的属性名和属性值。
>
> 主要流程如下：
>
> 1. 定义错误报告函数report_error，用于在出错时输出错误信息。
> 2. 定义面选择函数mask_for_faces，用于在用户选择时只允许选择模型的面。
> 3. 定义选择面的函数select_faces，用于弹出选择对话框，让用户选择面，并返回选中的面的数量和数组。
> 4. 定义提示输入字符串的函数prompt_for_text，用于获取用户输入的属性名和属性值。
> 5. 定义主函数do_it，用于不断提示用户选择面，然后为这些面赋予属性名和属性值。
> 6. 定义UF初始化和UF终止的ufusr函数。
> 7. 定义卸载函数ufusr_ask_unload。
>
> 通过以上流程，用户可以方便地为模型上的面添加自定义的属性信息，以便后续的查询和使用。该代码通过二次开发扩展了NX软件的功能，提高了建模的灵活性。
>
