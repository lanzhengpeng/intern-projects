### 【1160】edit selected part attribute 编辑选定的零件属性

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_attr.h>  
    #include <uf_assem.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
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
                case 1:                     /* Back 里海译:back */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 里海译:Cancel的翻译是“取消”。 */  
                    return -1;  
                    break;  
                case 18:                    /* More... 里海译:更多... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 里海译:根据上下文，"Picked one"可以翻译为“精选之一”。 */  
                    return a + resp - 5;  
                    break;  
            }  
        } while (TRUE);  
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
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strncpy(&menu[0][0], item, 16);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
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
        logical  
            locking_allowed = false,  
            is_locked,  
            was_locked;  
        int  
            ii,  
            n,  
            now[2] = { -1, -1 },  
            which;  
        tag_t  
            part = UF_ASSEM_ask_work_part();  
        char  
            date_string[21],  
            *default_value,  
            time_string[21],  
            **titles;  
        UF_ATTR_part_attr_p_t  
            attrs;  
        UF_CALL(UF_get_customer_default("UG_AllowAttributeLockingInNXOpen", 0,  
            &default_value));  
        ECHO("File -> Utilities -> Customer Defaults -> Gateway -> User Attributes -> Security\n");  
        ECHO("Attribute Locking via NX Open = %s\n", default_value);  
        if (!strcmp( default_value, "true")) locking_allowed = true;  
        UF_free(default_value);  
        UF_CALL(UF_ATTR_ask_part_attrs(part, &n, &attrs));  
        if (n == 0)  
        {  
            ECHO("%s\n", "This part doesn't have any part attributes!");  
            return;  
        }  
        UF_CALL(allocate_string_array(n, UF_ATTR_MAX_TITLE_LEN+1, &titles));  
        for (ii = 0; ii < n; ii++)  
            sprintf(titles[ii], attrs[ii].title);  
        while ((which = choose_one_item("Edit part attribute", titles, n)) > -1)  
        {  
            UF_CALL(UF_ATTR_ask_locked(part, titles[which], &was_locked));  
            if (was_locked)  
            {  
                if (locking_allowed)  
                {  
                    UF_CALL(UF_ATTR_set_locked(part, titles[which], false));  
                    UF_CALL(UF_ATTR_ask_locked(part, titles[which], &is_locked));  
                    if (is_locked)  
                    {  
                        ECHO("Failed to unlock %s\n", titles[which]);  
                        continue;  
                    }  
                }  
                else  
                {  
                    ECHO("%s is locked and cannot be edited\n", titles[which]);  
                    continue;  
                }  
            }  
            switch (attrs[which].value.type)  
            {  
                case UF_ATTR_bool:  
                    attrs[which].value.value.boolean =  
                        !attrs[which].value.value.boolean;  
                    break;  
                case UF_ATTR_integer:  
                    prompt_for_an_integer(titles[which], "New integer value",  
                        &attrs[which].value.value.integer);  
                    break;  
                case UF_ATTR_real:  
                    prompt_for_a_number(titles[which], "New real value",  
                        &attrs[which].value.value.real);  
                    break;  
                case UF_ATTR_time:  
                    UF_CALL(uc4582(now, 1, date_string, time_string));  
                    UF_CALL(uc4583(date_string, time_string,  
                        attrs[which].value.value.time));  
                    break;  
                case UF_ATTR_string:  
                    prompt_for_text("New string value",  
                        attrs[which].value.value.string);  
                    break;  
                default:  
                    break;  
            }  
            UF_CALL(UF_ATTR_assign(part, titles[which], attrs[which].value));  
            if (was_locked)  
                UF_CALL(UF_ATTR_set_locked(part, titles[which], true));  
        }  
        UF_free(attrs);  
        UF_free_string_array(n, titles);  
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

> 这段NX二次开发代码的主要功能是允许用户编辑NX部件的属性，同时提供了对属性进行锁定和解锁的功能。具体来说：
>
> 1. 通过NX的UF函数获取当前部件的属性列表，并显示给用户选择要编辑的属性。
> 2. 用户选择要编辑的属性后，代码会检查该属性是否被锁定。如果被锁定，则尝试解锁，以便进行编辑。
> 3. 根据属性的类型（整数、实数、字符串等），使用不同的对话框获取用户输入的新值。
> 4. 将用户输入的新值赋给该属性，并更新到NX部件中。
> 5. 如果属性之前被锁定，编辑完成后需要重新锁定。
> 6. 代码提供了错误处理机制，在调用NX API时捕获错误并报告。
> 7. 用户可以通过NX的“工具”->“实用程序”->“客户默认”->“网关”->“用户属性”->“安全”设置来启用或禁用属性锁定功能。
> 8. 该代码通过NX的ufusr()函数作为二次开发的入口点。
> 9. 提供了ufusr_ask_unload()函数，以便在卸载二次开发应用程序时进行清理。
>
> 总体而言，这段代码通过对话框交互方式，使用户能够方便地编辑NX部件的属性，并提供了灵活的属性锁定机制，以控制属性的编辑权限。
>
