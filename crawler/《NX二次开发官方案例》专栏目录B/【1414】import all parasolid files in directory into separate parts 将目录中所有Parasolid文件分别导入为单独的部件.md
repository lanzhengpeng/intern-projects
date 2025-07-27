### 【1414】import all parasolid files in directory into separate parts 将目录中所有Parasolid文件分别导入为单独的部件

#### 代码

```cpp
    /*HEAD IMPORT_ALL_PARASOLID_FILES_IN_DIRECTORY_INTO_SEPARATE_PARTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ps.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 新增的函数。 */  
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
    static void ask_import_directory(char *fspec)  
    {  
        char  
            *p_dir;  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_IMPORT_DIR, &p_dir));  
        strcpy(fspec, p_dir);  
        UF_free(p_dir);  
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
    static void convert_to_new_partname(char *old_fspec, int old_type, char *addon, char *copy_fspec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE];  
        UF_CALL(uc4576(old_fspec, old_type, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        strcat(part_name, addon);  
        UF_CALL(uc4575(dspec, 2, part_name, copy_fspec));  
    }  
    static void do_it(void)  
    {  
        int  
            cnt = 0;  
        uf_list_p_t  
            body_list;  
        char  
            dirspec[MAX_FSPEC_SIZE+1] = { "" },  
            fname[MAX_FSPEC_SIZE+1],  
            lbb[MAX_FSPEC_SIZE+1],  
            fspec[MAX_FSPEC_SIZE+1],  
            messg[150];  
        tag_t  
            part = NULL_TAG;  
        ask_import_directory(dirspec);  
        if (prompt_for_text("Import parasolid files*Enter directory path", dirspec))  
        {  
            uc4508(dirspec, (1<<11)|(1<<13), 0, "*.x*_*");  
            while (!uc4518())  
            {  
                uc4600(fname);  
                uc4575(dirspec, 0, fname, fspec);  
                sprintf(messg, "(%d) Importing from %s...\n", ++cnt, fspec);  
                WRITE(messg);  
                convert_to_new_partname(fspec, 0, "", lbb);  
                UF_CALL(UF_PART_new(lbb,UF_PART_ENGLISH,&part));  
                if (!UF_CALL(UF_PS_import_data(fspec, &body_list)))  
                {  
                    UF_CALL(UF_MODL_delete_list(&body_list));  
                    sprintf(messg, "(%d) Saving  %s...\n", cnt, lbb);  
                    WRITE(messg);  
                    UF_CALL(UF_PART_save());  
                    UF_CALL(UF_PART_close(part, 0, 0));  
                }  
                else  
                    break;  
            }  
            UF_CALL(uc4548());  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这是用于在NX中批量导入Parasolid文件并保存为NX部件的二次开发代码。主要功能包括：
>
> 1. 使用UF_UI函数弹窗提示用户选择包含Parasolid文件的目录。
> 2. 遍历指定目录下的所有Parasolid文件，并使用UF_PART函数创建新的NX部件。
> 3. 使用UF_PS函数将Parasolid文件导入到新创建的NX部件中。
> 4. 如果导入成功，则保存NX部件并关闭。
> 5. 在导入失败时，会打印错误信息。
> 6. 使用UF_terminate函数终止NX的二次开发环境。
> 7. 使用UF_UNLOAD_IMMEDIATELY标志表示二次开发环境可以立即卸载。
>
> 整个代码流程清晰，通过UF_PART、UF_PS等NX提供的二次开发接口实现了自动化批量导入Parasolid文件的功能。
>
