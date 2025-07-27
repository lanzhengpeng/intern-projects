### 【1393】highlight object of parasolid tag 高亮显示Parasolid标签的对象

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ps.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
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
        tag_t  
            ps_tag = 0,  
            obj_tag;  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        while (prompt_for_an_integer("Parasolid", "Tag", &ps_tag))  
        {  
            UF_CALL(UF_PS_ask_object_of_ps_tag(ps_tag, &obj_tag));  
            sprintf(name, "PS_TAG_%d", ps_tag);  
            UF_CALL(UF_OBJ_set_name(obj_tag, name));  
            UF_CALL(UF_DISP_set_highlight(obj_tag, TRUE));  
            uc1601(name, TRUE);  
            UF_CALL(UF_DISP_set_highlight(obj_tag, FALSE));  
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

> 这段代码是用于NX(前称UG)的二次开发代码，主要功能是遍历指定类型的Parasolid标签，并给这些标签对应的对象设置名字、高亮显示。
>
> 主要流程如下：
>
> 1. 初始化NX环境
> 2. 循环遍历，获取用户输入的Parasolid标签
> 3. 根据标签获取对应的对象
> 4. 为对象设置名字，格式为"PS_TAG_标签号"
> 5. 高亮显示对象
> 6. 弹出对话框显示对象名称
> 7. 去除高亮显示
> 8. 结束循环
> 9. 终止NX环境
>
> 关键函数包括：
>
> 此外，代码中还定义了错误报告函数report_error，用于在函数调用出错时输出错误码和错误信息。通过这段代码，用户可以便捷地为Parasolid标签对应的对象设置名称和高亮显示，方便后续的识别和处理。
>
