### 【1021】delete by eid 删除通过EID

#### 代码

```cpp
    /*HEAD DELETE_BY_EID CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：注意：UF_print_syslog在V18版本中是新增的。 */  
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
    static logical prompt_for_an_integer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
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
        logical  
            d_flag = TRUE;  
        tag_t  
            object = NULL_TAG;  
        char  
            msg[MAX_LINE_SIZE+1];  
        while (prompt_for_an_integer("Highlight/Delete?", "EID", (int *)&object))  
        {  
            sprintf(msg, "Delete highlighted object (EID = %d)", object);  
            if (!UF_CALL(UF_DISP_set_highlight(object, TRUE)) &&  
                ask_yes_or_no(msg, &d_flag) && d_flag)  
                UF_CALL(UF_OBJ_delete_object(object));  
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

> 这段代码是一个NX Open的二次开发程序，用于交互式删除指定EID的对象。
>
> 主要功能包括：
>
> 1. 提示用户输入要删除的对象的EID，并高亮显示。
> 2. 询问用户是否确认删除，如果确认，则删除该对象。
> 3. 循环执行以上操作，直到用户取消输入EID。
>
> 代码的关键部分包括：
>
> 整体而言，这段代码实现了基本的NX对象删除功能，通过交互式对话框提高了用户体验。
>
