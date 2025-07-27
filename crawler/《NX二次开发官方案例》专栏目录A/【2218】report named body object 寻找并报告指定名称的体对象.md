### 【2218】report named body object 寻找并报告指定名称的体对象

#### 代码

```cpp
    /*HEAD REPORT_NAMED_BODY_OBJECT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，可以翻译为：

UF_print_syslog是V18版本新增的函数，请只提供翻译，无需添加其他无关内容。 */  
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
    /* qq3123197280 */  
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
    static logical pick_one_of_two_choices(char *prompt, char *option_one,  
        char *option_two, int *choice)  
    {  
        int  
            resp;  
        char  
            options[2][38];  
        strncpy(&options[0][0], option_one, 37);  
        strncpy(&options[1][0], option_two, 37);  
        options[0][37] = '\0';  
        options[1][37] = '\0';  
        resp = uc1603(prompt, *choice, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
            *choice = resp - 4;  /* returns 1 or 2 里海译:返回1或2 */  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    #define SAY(X)    (report_an_integer(#X, X))  
    static void report_an_integer(char *variable, int value)  
    {  
        char  
            msg[133];  
        sprintf(msg, "%s = %d\n", variable, value);  
        uc1601(msg, TRUE);  
    }  
    static void do_it(void)  
    {  
        int  
            type = 1,  
            subtype;  
        tag_t  
            body,  
            object;  
        char  
            name[MAX_ENTITY_NAME_SIZE+1] = { "" };  
        while (((body = select_a_body("Find named faces/edges")) != NULL_TAG) &&  
            prompt_for_text("Enter name to find", name) &&  
            pick_one_of_two_choices("Type to find", "Face", "Edge", &type))  
        {  
            subtype = type + 1;  
            if (!UF_CALL(UF_MODL_ask_named_body_object(&body, &subtype, name,  
                &object)) && (object != NULL_TAG))  
            {  
                UF_CALL(UF_DISP_set_highlight(object, TRUE));  
                SAY(object);  
                UF_CALL(UF_DISP_set_highlight(object, FALSE));  
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

> 根据代码内容，这是一段用于在NX中查找特定名称的面或边的二次开发代码。以下是代码的主要功能：
>
> 1. 定义了一个错误报告函数report_error，用于在出现UF函数调用错误时输出错误信息。
> 2. 定义了一个函数mask_for_bodies，用于设置选择掩码，只允许选择实体体。
> 3. 定义了一个函数select_a_body，用于弹出一个对话框，让用户选择一个实体体。
> 4. 定义了一个函数prompt_for_text，用于提示用户输入文本。
> 5. 定义了一个函数pick_one_of_two_choices，用于让用户在两个选项中选择一个。
> 6. 定义了一个函数report_an_integer，用于输出一个整数值。
> 7. 定义了一个函数do_it，是主要逻辑函数。该函数首先循环调用select_a_body获取用户选择的实体体，然后调用prompt_for_text获取用户输入的名称，再调用pick_one_of_two_choices让用户选择要查找的面或边，最后调用UF_MODL_ask_named_body_object函数查找名称对应的对象，并高亮显示。
> 8. ufusr函数是NX二次开发程序的入口函数，在初始化后调用do_it函数，并在结束时进行终止。
> 9. ufusr_ask_unload函数用于设置程序卸载方式。
>
> 整体来看，这段代码实现了在NX中通过名称查找面或边的功能，用户可以通过交互方式选择实体体、输入名称以及选择要查找的面或边，程序会找到对应的对象并高亮显示。
>
