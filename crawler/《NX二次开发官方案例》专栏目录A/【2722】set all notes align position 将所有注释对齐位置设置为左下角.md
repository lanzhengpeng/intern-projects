### 【2722】set all notes align position 将所有注释对齐位置设置为左下角

#### 代码

```cpp
    /*HEAD SET_ALL_NOTES_ALIGN_POSITION CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，可以得知UF_print_syslog是V18版本中的新功能。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
            WRITE("Note was set to ");  
            WRITE_D(mpi[30]);  
            mpi[30] = 7;  /* Bottom Left 里海译:Bottom Left的翻译是左下角。 */  
            WRITE("Setting note set to ");  
            WRITE_D(mpi[30]);  
            UF_CALL(UF_DRF_set_object_preferences(note, mpi, mpr, rad, dia));  
            UF_CALL(UF_DRF_ask_object_preferences(note, mpi, mpr, rad, dia));  
            WRITE("Note is now set to ");  
            WRITE_D(mpi[30]);  
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

> 这是一段NX的二次开发代码，其主要功能是设置所有注释的排列位置为左下角。
>
> 代码的主要流程包括：
>
> 1. 包含必要的头文件，并定义了宏来报告错误。
> 2. 定义了一个report_error函数，用于报告错误信息。
> 3. 定义了一个write_integer_to_listing_window函数，用于向列表窗口写入整数信息。
> 4. 定义了一个ask_next_note函数，用于遍历零件中的所有注释，并返回下一个注释的tag。
> 5. 定义了do_it函数，用于遍历所有注释，并设置其排列位置为左下角(7)。
> 6. 定义了ufusr函数，是NX二次开发程序的入口函数，其中调用了do_it函数。
> 7. 定义了ufusr_ask_unload函数，用于卸载用户自定义函数。
>
> 通过这个代码，作者实现了对零件中所有注释的排列位置进行统一设置的功能。
>
