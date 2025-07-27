### 【3069】write parts list to a text file 将零件列表写入文本文件

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_plist.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
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
    static void build_similar_temp_filespec(char *fspec, int ftype, char *new_spec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            temp_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            *tmp_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4576(fspec, 0, dspec, temp_name));  
        if (strrchr(temp_name, '.')) strcpy(strrchr(temp_name, '.'), "");  
        UF_CALL(uc4575(tmp_dir, ftype, temp_name, new_spec));  
    }  
    static void build_similar_filespec(char *fspec, int ftype)  
    {  
        logical  
            is_ugmgr;  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1];  
        UF_CALL(UF_is_ugmanager_active(&is_ugmgr));  
        UF_PART_ask_part_name(part, part_fspec);  
        if (!is_ugmgr)  
        {  
            UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
            UF_CALL(uc4578(part_name, 2, part_name));  
            UF_CALL(uc4575(dspec, ftype, part_name, fspec));  
        }  
        else  
        {  
            UF_CALL(UF_UGMGR_decode_part_filename(part_fspec, part_number,  
                part_revision, part_file_type, part_file_name));  
            build_similar_temp_filespec(part_number, ftype, fspec);  
        }  
    }  
    #include <uf_undo.h>  
    static UF_UNDO_mark_id_t set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
        return mark_id;  
    }  
    static void roll_back_and_delete_undo_mark(UF_UNDO_mark_id_t mark_id)  
    {  
        UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
        UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));  
    }  
    static void do_it(void)  
    {  
        int  
            n_pls;  
        tag_t  
            pl,  
            *pls;  
        double  
            zero[3] = { 0,0,0 };  
        char  
            tfn[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_PLIST_prefs_t  
            prefs;  
        UF_UNDO_mark_id_t  
            mark1;  
        build_similar_filespec(tfn, 79);  
        UF_CALL(UF_PLIST_ask_tags(&pls, &n_pls));  
        if (n_pls > 0)  
        {  
            UF_CALL(UF_PLIST_list_to_file(pls[0], tfn, 1, ""));  
            UF_free(pls);  
        }  
        else  
        {  
            mark1 = set_undo_mark("");  
            UF_CALL(UF_PLIST_ask_default_prefs(&prefs));  
            UF_CALL(UF_PLIST_create(&prefs, zero, &pl));  
            UF_CALL(UF_PLIST_list_to_file(pl, tfn, 1, ""));  
            roll_back_and_delete_undo_mark(mark1);  
        }  
        ECHO("Parts list written to %s\n", tfn);  
    }  
    /* qq3123197280 */  
    extern void ufusr(char *param, int *retcode, int paramLen)  
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

> 这段代码是一个NX Open的UF程序，其主要功能是将NX部件列表保存到文件中。具体包括以下部分：
>
> 1. 错误处理宏：通过宏UF_CALL来调用UF函数，并在失败时输出错误信息。
> 2. 构建临时文件路径：通过build_similar_temp_filespec函数，根据当前打开的部件文件构建一个临时的文件路径。
> 3. 构建类似文件路径：通过build_similar_filespec函数，根据当前打开的部件构建一个类似的文件路径。
> 4. 设置和回滚撤销标记：通过set_undo_mark和roll_back_and_delete_undo_mark函数设置撤销标记，并在需要时回滚到该标记。
> 5. 写入部件列表到文件：do_it函数会获取当前打开的部件列表，并保存到文件中。如果没有打开部件，则创建一个空列表并保存。
> 6. UF程序入口：ufusr函数是程序的入口，初始化UF后调用do_it，然后终止UF。
> 7. 卸载请求：ufusr_ask_unload函数请求立即卸载该UF程序。
>
> 综上所述，这段代码实现了将NX部件列表保存到文件的功能，并具有错误处理和撤销重做功能。
>
