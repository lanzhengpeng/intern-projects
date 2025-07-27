### 【0787】create non-modifiable notes 创建不可修改的注释

#### 代码

```cpp
    /*HEAD CREATE_NON-MODIFIABLE_NOTES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_udobj.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_exit.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本中的新功能。

翻译：

UF_print_syslog是V18版本中的新功能。 */  
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
    static UF_UDOBJ_class_t NT_class_id;  
    DllExport extern UF_UDOBJ_class_t get_NT_class_id(void)  
    {  
        return NT_class_id;  
    }  
    static void register_udo_class(void)  
    {  
        UF_CALL(UF_UDOBJ_create_class("Note_UDO", "Note UDO", &NT_class_id));  
    }  
    /* 里海 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        register_udo_class();  
        UF_terminate();  
    }  
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
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
    static tag_t ask_next_udo(tag_t part, UF_UDOBJ_class_t class_id, tag_t udo)  
    {  
        UF_UDOBJ_class_t  
            this_class_id;  
        UF_UDOBJ_all_data_t  
            udo_data;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part,  
            UF_user_defined_object_type, &udo)) && (udo != NULL_TAG))  
        {  
            UF_CALL(UF_UDOBJ_ask_udo_data(udo, &udo_data));  
            this_class_id = udo_data.class_id;  
            UF_CALL(UF_UDOBJ_free_udo_data(&udo_data));  
            if (this_class_id == class_id) return udo;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            a_note,  
            *the_notes,  
            nt_udo = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
            loc[3];  
        char  
            text[133],  
            *text_p[1];  
        UF_UDOBJ_class_t  
            nt_class_id = get_NT_class_id();  
        if (nt_class_id == 0)  
        {  
            register_udo_class();  
            nt_class_id = get_NT_class_id();  
        }  
        text_p[0] = text;  
        while ((nt_udo = ask_next_udo(part, nt_class_id, nt_udo)) != NULL_TAG)  
        {  
            UF_CALL(UF_UDOBJ_ask_owned_objects(nt_udo, &n, &the_notes));  
            for (ii = 0; ii < n; ii++)  
                UF_CALL(UF_DISP_set_highlight(the_notes[ii], TRUE));  
            sprintf(text, "UDO has %d owned notes", n);  
            uc1601(text, TRUE);  
            for (ii = 0; ii < n; ii++)  
                UF_CALL(UF_DISP_set_highlight(the_notes[ii], FALSE));  
            UF_free(the_notes);  
        }  
        text[0] = '\0';  
        UF_CALL(UF_UDOBJ_create_udo(nt_class_id, &nt_udo));  
        while (specify_point("Indicate note position", loc) &&  
            prompt_for_text("Enter note text", text))  
        {  
            UF_CALL(UF_DRF_create_note(1, text_p, loc, 0, &a_note));  
            UF_CALL(UF_UDOBJ_add_owning_links(nt_udo, 1, &a_note));  
        }  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发程序，其主要功能是创建和管理用户定义的对象(UDO)来存储注释。以下是该代码的主要功能和关键点：
>
> 1. 初始化和注册UDO类：在初始化阶段，会调用register_udo_class函数来注册一个名为"Note_UDO"的UDO类，用于存储注释。
> 2. 创建和管理注释：主函数do_it会遍历当前显示部件中的所有"Note_UDO"对象，高亮显示其拥有的注释，并统计注释数量。然后提示用户输入注释文本和位置，并创建新的注释对象，将其添加到UDO对象中。
> 3. 错误处理：使用宏UF_CALL包装UF函数调用，并在出错时报告错误信息。
> 4. 用户交互：使用UF_UI函数来提示用户输入注释文本和指定注释位置。
> 5. 注释对象和UDO对象关联：通过调用UF_UDOBJ_add_owning_links将注释对象关联到UDO对象，实现注释的管理。
> 6. 注释的遍历：通过调用UF_OBJ_cycle_objs_in_part和UF_UDOBJ_ask_udo_data遍历部件中的所有UDO对象，并判断UDO对象的类型。
> 7. 注释的显示：通过调用UF_DISP_set_highlight来高亮显示注释，以突出显示。
> 8. 注释的创建：通过调用UF_DRF_create_note来创建注释对象。
>
> 总的来说，这段代码实现了创建和管理注释的功能，并且通过用户定义的对象来存储注释，具有良好的可扩展性。
>
