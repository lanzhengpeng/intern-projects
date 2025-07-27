### 【1878】reparent broken wave links in external mode 修复断开的波连接，外部模式

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_wave.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_so.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_text.h>  
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
    static tag_t ask_next_wave_linked_feature(tag_t part, tag_t feat)  
    {  
        char  
            *this_type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
        {  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE)  
            {  
                UF_CALL(UF_MODL_ask_feat_type(feat, &this_type));  
                if (strstr(this_type, "LINK"))  
                {  
                    UF_free(this_type);  
                    return feat;  
                }  
                UF_free(this_type);  
            }  
        }  
        return NULL_TAG;  
    }  
    static tag_t ask_first_other_part(tag_t except_part)  
    {  
        int  
            ii,  
            n = UF_PART_ask_num_parts();  
        tag_t  
            a_part;  
        if (n > 1)  
        {  
            for (ii = 0; ii < n; ii++)  
            {  
                a_part = UF_PART_ask_nth_part(ii);  
                if (a_part != except_part) return a_part;  
            }  
        }  
        return NULL_TAG;  
    }  
    static void do_it(tag_t part, tag_t other_part)  
    {  
        logical  
            is_broken;  
        tag_t  
            feat = NULL_TAG,  
            source,  
            xform;  
        char  
            *name;  
        while ((feat = ask_next_wave_linked_feature(part, feat)) != NULL_TAG)  
        {  
            if (UF_CALL(UF_WAVE_is_link_broken(feat, &is_broken)) || !is_broken)  
                continue;  
            UF_CALL(UF_MODL_ask_feat_name(feat, &name));  
            ECHO(" reparenting %s\n", name);  
            name = strtok(name, "(");  
            source = NULL_TAG;  
            UF_CALL(UF_ASSEM_set_work_part_quietly(other_part, &part));  
            UF_CALL(UF_OBJ_cycle_by_name(name, &source));  
            UF_free(name);  
            UF_CALL(UF_ASSEM_set_work_part_quietly(part, &other_part));  
            if (source == NULL_TAG) continue;  
            if (UF_ASSEM_is_occurrence(source))  
            {  
                UF_CALL(UF_SO_create_xform_assy_ctxt(source,  
                    UF_ASSEM_ask_part_occurrence(source), NULL_TAG, &xform));  
                source = UF_ASSEM_ask_prototype_of_occ(source);  
            }  
            else  
                xform = NULL_TAG;  
            UF_CALL(UF_MODL_ask_feat_sysname(feat, &name));  
            if (strstr(name, "Linked Point"))  
                UF_CALL(UF_WAVE_set_linked_pt_point(feat, source, xform));  
            else  
                UF_CALL(UF_WAVE_set_link_data(feat, source, xform, TRUE));  
            UF_free(name);  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        tag_t  
            from_part,  
            to_part;  
        if (UF_CALL(UF_initialize())) return;  
        to_part = UF_PART_ask_display_part();  
        from_part = ask_first_other_part(to_part);  
        do_it(to_part, from_part);  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        int  
            ii;  
        char  
            msg[133];  
        if (status->n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_get_fail_message(status->statuses[ii], msg);  
                ECHO("  %s - %s", status->file_names[ii], msg);  
            }  
            ECHO("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void build_copy_partname(char *copy_fspec)  
    {  
        logical  
            tceng;  
        char  
            dspec[MAX_FSPEC_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_fspec[MAX_FSPEC_SIZE+1],  
            part_name[MAX_ENTITY_NAME_SIZE+1],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            *tmp_dir;  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(UF_is_ugmanager_active(&tceng));  
        if (!tceng)  
        {  
            UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
            UF_CALL(uc4578(part_name, 2, part_name));  
            strcat(part_name, "-new");  
            UF_CALL(uc4575(dspec, 2, part_name, copy_fspec));  
        }  
        else  
        {  
            UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
            UF_CALL(UF_UGMGR_decode_part_filename(part_fspec, part_number,  
                part_revision, part_file_type, part_file_name));  
            UF_CALL(uc4575(tmp_dir, 2, part_number, copy_fspec));  
        }  
    }  
    /*  To run in external native mode NX:  
            program.exe <part_with_broken_links.prt> <part_with_replacements.prt>  
        To run in external NX Manager mode:  
            program.exe -pim=yes @DB/partno-broken/rev @DB/partno-replace/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        tag_t  
            from_part,  
            to_part;  
        char  
            new_fspec[MAX_FSPEC_SIZE+1],  
            from_part_name[MAX_FSPEC_SIZE+1],  
            to_part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            UF_CALL(UF_TEXT_init_native_lang_support());  
            if ((uc4621(to_part_name) == 1) && (uc4621(from_part_name) == 1))  
            {  
                ECHO("reparenting to %s from %s\n", to_part_name, from_part_name);  
                UF_CALL(UF_PART_open(from_part_name, &from_part, &status));  
                if (status.n_parts > 0) report_load_status(&status);  
                if (status.failed) return 1;  
                UF_CALL(UF_PART_open(to_part_name, &to_part, &status));  
                if (status.n_parts > 0) report_load_status(&status);  
                if (status.failed) return 1;  
                UF_CALL(UF_WAVE_freeze(1, &to_part));  
                do_it(to_part, from_part);  
                UF_CALL(UF_WAVE_update_parts(1, &to_part));  
                build_copy_partname(new_fspec);  
                if (!UF_CALL(UF_PART_save_as(new_fspec)))  
                    ECHO("saved %s as %s\n", to_part_name, new_fspec);  
                UF_CALL(UF_PART_close_all());  
            }  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是修复NX装配体中损坏的链接特征。
>
> 具体来说，它实现了以下功能：
>
> 1. 识别损坏的链接特征：通过循环遍历当前显示部件中的特征，并判断哪些特征是损坏的链接特征。
> 2. 修复损坏的链接特征：通过在另一个部件中查找同名特征，并设置正确的链接关系，来修复损坏的链接特征。
> 3. 处理命令行参数：支持从命令行参数指定要修复的部件，以及要从中复制特征的另一个部件。
> 4. 保存修复后的部件：修复完毕后，将当前显示的部件另存为新文件。
> 5. 错误处理和日志输出：代码中包含了错误处理和日志输出的功能，用于记录程序运行过程中的错误和关键信息。
> 6. 初始化和终止NX环境：在程序开始和结束时，初始化和终止NX环境。
>
> 总的来说，这段代码实现了在NX装配体中自动修复损坏链接特征的功能，并提供了完整的错误处理和日志输出。
>
