### 【1391】highlight named faces 高亮指定名称的面

#### 代码

```cpp
    /*HEAD HIGHLIGHT_NAMED_FACES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，可以翻译如下：

注意：UF_print_syslog 是 V18 的新功能，请只回答与功能相关的翻译，不要添加其他内容。 */  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static tag_t cycle_by_name_type_and_subtype(tag_t part, char *name, int type,  
        int subtype, tag_t object)  
    {  
        int  
            this_type,  
            this_subtype;  
        while (!UF_CALL(UF_OBJ_cycle_by_name_and_type(part, name, type, TRUE,  
            &object)) && (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &this_type, &this_subtype));  
            if (this_subtype == subtype) return object;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            name[MAX_ENTITY_NAME_SIZE+1] = { "" };  
        while (prompt_for_text("Enter name of face to highlight", name))  
        {  
            while ((face = cycle_by_name_type_and_subtype(part, name,  
                UF_solid_type, UF_solid_face_subtype, face)) != NULL_TAG)  
                UF_CALL(UF_DISP_set_highlight(face, TRUE));  
        }  
        UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));  
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

> 这段代码是一个NX二次开发的示例，其主要功能是高亮显示用户指定名称的面。
>
> 代码逻辑如下：
>
> 1. 首先定义了一个错误报告函数report_error，用于打印函数调用失败的错误信息。
> 2. 定义了一个提示输入文本的函数prompt_for_text，用于提示用户输入面的名称。
> 3. 定义了一个根据名称、类型和子类型循环遍历对象的函数cycle_by_name_type_and_subtype，用于找到用户指定名称的面。
> 4. 定义了一个主函数do_it，循环提示用户输入面的名称，然后找到该面并设置高亮显示。
> 5. ufusr是NX调用的主函数，首先初始化NX环境，然后调用do_it函数，最后清理环境。
> 6. ufusr_ask_unload函数用于卸载用户自定义的库。
>
> 通过以上函数，该代码实现了根据用户输入的面名称，找到对应的面并高亮显示的功能。
>
