### 【2724】set all notes thick 设置所有注释加粗

#### 代码

```cpp
    /*HEAD SET_ALL_NOTES_THICK CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的功能。因此，在回答问题时，只需简明扼要地表示“UF_print_syslog是V18版本新增的功能”，无需添加其他无关内容。 */  
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
    static tag_t ask_next_note(tag_t part, tag_t a_note)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drafting_entity_type,  
            &a_note)) && (a_note != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(a_note, &type, &subtype));  
            if (subtype == UF_draft_note_subtype) return a_note;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            mpi[100];  
        tag_t  
            note = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
            mpr[70];  
        char  
            rad[27],  
            dia[27];  
        while ((note = ask_next_note(part, note)) != NULL_TAG)  
        {  
            UF_CALL(UF_DRF_ask_object_preferences(note, mpi, mpr, rad, dia));  
            mpi[89] = UF_OBJ_WIDTH_THICK;  
            UF_CALL(UF_DRF_set_object_preferences(note, mpi, mpr, rad, dia));  
        }  
    }  
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

> 这段代码是NX的二次开发代码，其主要功能是设置当前打开部件中的所有注释线的线宽为“加粗”。具体来说：
>
> 1. 包含了NX二次开发所需的头文件。
> 2. 定义了一个错误报告函数report_error，用于打印错误信息。
> 3. 定义了一个函数ask_next_note，用于遍历部件中的注释对象，并返回下一个注释对象。
> 4. 定义了一个函数do_it，用于设置所有注释线的线宽为加粗。
> 5. ufusr函数是NX二次开发的入口函数，首先初始化，然后调用do_it函数，最后终止。
> 6. ufusr_ask_unload函数用于设置用户程序的卸载方式为立即卸载。
> 7. 代码通过循环遍历当前打开部件中的所有注释对象，并设置其线宽为加粗。
> 8. 使用了UF_OBJ、UF_DRF等NX的二次开发API。
> 9. 通过UF_CALL宏来调用NX的API，并检查错误。
>
> 总体来说，这段代码实现了设置当前打开部件中的所有注释线线宽为加粗的功能，并通过二次开发接口与NX进行了交互。
>
