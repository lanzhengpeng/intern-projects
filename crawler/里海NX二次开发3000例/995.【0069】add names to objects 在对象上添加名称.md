### 【0069】add names to objects 在对象上添加名称

#### 代码

```cpp
    /*HEAD ADD_NAMES_TO_OBJECTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是在V18版本中新增的函数。 */  
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
    static tag_t select_an_object(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an object", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, NULL,NULL,&resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
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
    static void do_it(void)  
    {  
        tag_t  
            object;  
        char  
            name[MAX_ENTITY_NAME_SIZE+1] = { "" };  
        while ((object = select_an_object("Rename")) != NULL_TAG)  
        {  
            UF_OBJ_ask_name(object, name);  
            if (prompt_for_text("Object name", name))  
                UF_CALL(UF_OBJ_set_name(object, name));  
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

> 这段代码是用于NX的二次开发，其主要功能是给装配体中的对象添加或修改名称。
>
> 代码主要包含以下几个部分：
>
> 1. 错误处理函数 report_error()：用于打印错误信息到日志窗口和syslog。
> 2. 选择对象函数 select_an_object()：通过对话框让用户选择一个对象，并返回其tag。
> 3. 提示输入文本函数 prompt_for_text()：用于提示用户输入文本，并返回输入结果。
> 4. 主功能函数 do_it()：循环调用 select_an_object() 函数选择对象，然后获取对象名称，提示用户修改名称，并调用 UF_OBJ_set_name() 函数设置新名称。
> 5. ufusr() 函数：初始化和终止 NX Open API，调用 do_it() 函数实现功能。
> 6. ufusr_ask_unload() 函数：返回立即卸载标志，表示在卸载该UF时不需要等待。
>
> 总体来说，这段代码实现了给 NX 装配体中的对象添加或修改名称的功能，通过用户交互的方式提高了易用性。
>
